/***************************************************************************************************************
FTP Credit to Leonardo Bispo (https://github.com/ldab/ESP32-CAM-Picture-Sharing)
Live Stream Functions Credit to Anatoli Arkhipenko (https://github.com/arkhipenko/esp32-cam-mjpeg)
SD Card Functions Credit to Rui Santos (https://RandomNerdTutorials.com/esp32-cam-take-photo-save-microsd-card)
****************************************************************************************************************/
#include "Arduino.h"
#include "src/secrets.h"
/* Comment this out to disable prints and save space */
#define BLYNK_DEBUG
#define BLYNK_PRINT Serial
#define DEBUG_ESP

// Blynk related libraries
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

// Live stream related libraries
#include "src/OV2640.h"
#include <WebServer.h>

#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"

// SD Card related libraries
#include "FS.h"
#include "SD_MMC.h"
#include <EEPROM.h>

// In BLYNK add a device (ESP32 DEV, WIFI) and copy the authentication token for each unit
// 
// Chose the module you are programming and uncomment the auth token for that module
String cam_module = "4"; 
//char auth[] = "MODULE 1 AUTH TOKEN HERE"; //1
//char auth[] = "MODULE 2 AUTH TOKEN HERE"; //2
//char auth[] = "MODULE 3 AUTH TOKEN HERE"; //3
char auth[] = "MODULE 3 AUTH TOKEN HERE"; //4
//char auth[] = "MODULE 5 AUTH TOKEN HERE"; //5
// etc...

// Your WiFi credentials.
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASSWORD;

// FTP Client Lib
#include "ESP32_FTPClient.h"

// FTP Server credentials
char ftp_server[] = SECRET_FTP_SERVER;
char ftp_user[]   = SECRET_FTP_USERNAME;
char ftp_pass[]   = SECRET_FTP_PASSWORD;

// SD card initialize variables
int pictureNumber = 0; // Initialize picture number for SD Card
#define EEPROM_SIZE 1 // define the number of bytes you want to access

// Live stream server setup
#define CAMERA_MODEL_AI_THINKER
OV2640 cam;
WebServer server(80);

const char HEADER[] = "HTTP/1.1 200 OK\r\n" \
                      "Access-Control-Allow-Origin: *\r\n" \
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);

void handle_jpg_stream(void) {
  char buf[32];
  int s;

  WiFiClient client = server.client();

  client.write(HEADER, hdrLen);
  client.write(BOUNDARY, bdrLen);

  while (true) {
    if (!client.connected()) break;
    cam.run();
    s = cam.getSize();
    client.write(CTNTTYPE, cntLen);
    sprintf( buf, "%d\r\n\r\n", s );
    client.write(buf, strlen(buf));
    client.write((char *)cam.getfb(), s);
    client.write(BOUNDARY, bdrLen);
  }
}

const char JHEADER[] = "HTTP/1.1 200 OK\r\n" \
                       "Content-disposition: inline; filename=capture.jpg\r\n" \
                       "Content-type: image/jpeg\r\n\r\n";
const int jhdLen = strlen(JHEADER);

// handle_jpg() allows you to access a single jpg taken at
// the time of the request. An example use is like the one
// commented out later in the program, but could be useful.
// Example: server.on("/jpg", HTTP_GET, handle_jpg);
/*void handle_jpg(void) {
  WiFiClient client = server.client();

  cam.run();
  if (!client.connected()) return;

  client.write(JHEADER, jhdLen);
  client.write((char *)cam.getfb(), cam.getSize());
}*/

void handleNotFound() {
  String message = "Server is running!\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  server.send(200, "text / plain", message);
}

// Add Blynk Virtual Terminal Widget
WidgetTerminal terminal(V0);

// Connection timeout;
#define CON_TIMEOUT   10*1000 // milliseconds

// Not using Deep Sleep on PCB because TPL5110 timer takes over.
#define TIME_TO_SLEEP (uint64_t)60*60*1000*1000   // microseconds

// Camera buffer, URL and picture name
camera_fb_t *fb = NULL;

//Set Strings for the picture name this will be used as the ftp uploaded file name
String pic_name = "Module " + cam_module + " - ";
String sd_pic_name = "Module " + cam_module;  
String pic_url  = ""; //only used for debugging in Blynk app

// Variable marked with this attribute will keep its value during a deep sleep / wake cycle.
RTC_DATA_ATTR uint64_t bootCount = 0;

WidgetRTC rtc;
ESP32_FTPClient ftp (ftp_server, ftp_user, ftp_pass);

void deep_sleep(void);
void FTP_upload(void);
bool take_picture(void);

bool go = false;
bool stop = false;

//LED setup 
int freq = 5000;
int ledCHannel = 1;
int res = 8;
const int ledPin = 4;
int brightness;


BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
  Blynk.virtualWrite(V0, "------------------------------");
  Blynk.virtualWrite(V0, "Module " + cam_module);
  Blynk.virtualWrite(V0, " ");
  Blynk.virtualWrite(V0, "MAC Address:  ");
  Blynk.virtualWrite(V0, WiFi.macAddress());
  Blynk.virtualWrite(V0, " ");
  Blynk.virtualWrite(V0, "IP Address: ");
  Blynk.virtualWrite(V0, ipAddress2String(WiFi.localIP()));
  Blynk.virtualWrite(V0, "------------------------------");
}

// Pin     Blynk Object
//-------------------------------------------
// V0      Terminal
// V5      Shutter (Take Picture Button)
// V10     Flash Slider
// V12     Start Stream Button
// V13     Video Stream
// V14     Stop Stream Button (Must be done from a different module)

void setup() {
  setSyncInterval(30); // RTC sync interval
  ledcSetup(ledCHannel, freq, res);
  ledcAttachPin(ledPin, ledCHannel);
  
  #ifdef DEBUG_ESP
    Serial.begin(115200);
    Serial.setDebugOutput(true);
  #endif

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer   = LEDC_TIMER_0;
  config.pin_d0       = 5;
  config.pin_d1       = 18;
  config.pin_d2       = 19;
  config.pin_d3       = 21;
  config.pin_d4       = 36;
  config.pin_d5       = 39;
  config.pin_d6       = 34;
  config.pin_d7       = 35;
  config.pin_xclk     = 0;
  config.pin_pclk     = 22;
  config.pin_vsync    = 25;
  config.pin_href     = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn     = 32;
  config.pin_reset    = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  config.frame_size = FRAMESIZE_VGA; // set picture size, FRAMESIZE_XGA = 1024x768
  config.jpeg_quality = 15;          // quality of JPEG output. 0-63 lower means higher quality
  config.fb_count = 1;

//  camera init
//  esp_err_t err = esp_camera_init(&config);
//  if (err != ESP_OK) {
//    Serial.print("Camera init failed with error 0x%x");
//    Blynk.virtualWrite(V0, err);
//    return;
//  }

//  Change extra settings if required
//  sensor_t * s = esp_camera_sensor_get();
//  s->set_vflip(s, 0);       //flip it back
//  s->set_brightness(s, 1);  //up the blightness just a bit
//  s->set_saturation(s, -2); //lower the saturation
  
  // SD CARD INITIALIZE
  if(!SD_MMC.begin()){
    Serial.print("SD Card Mount Failed");
    Blynk.virtualWrite (V0, "SD Card Mount Failed");
    //return;
  }
  
  uint8_t cardType = SD_MMC.cardType();
  if(cardType == CARD_NONE){
    Serial.print("No SD Card attached");
    Blynk.virtualWrite (V0, "No SD Card attached");
  }
  
  // Enable timer wakeup for ESP32 sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP);

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

  WiFi.begin(ssid, pass);
  Blynk.virtualWrite(V0, "\nConnecting to WiFi");

  while (WiFi.status() != WL_CONNECTED && millis() < CON_TIMEOUT) {
    delay(2000);
    Serial.print(".");
  }

  if (WiFi.isConnected()) {
    Blynk.config(auth);
    Serial.println("IP Address: ");
    Serial.println(WiFi.localIP());
  }

  if (!WiFi.isConnected()) {
    Blynk.virtualWrite(V0, "Failed to connect to WiFi, going to sleep");
    ESP.restart();
  }

  cam.init(config);
  IPAddress ip = WiFi.localIP();
  server.on("/stream", HTTP_GET, handle_jpg_stream); // Could change directory to whatever you want
  // server.on("/jpg", HTTP_GET, handle_jpg); // Uncomment if you would like to access a single jpg
  server.onNotFound(handleNotFound);
  server.begin();
}

// BLYNK virutal pin for led brightness (V10)
//Doesn't seem to work after one of the included libraries
BLYNK_WRITE(V10){
  int pinValue = param.asInt();
  brightness = pinValue;
  Blynk.syncVirtual(V10);
}

//BLYNK virtual pin for shutter "Take Picture" button
BLYNK_WRITE(V5){ 
  int pinValue = param.asInt();
  Serial.println(pinValue);
  go=true;
}

// Convert ip address to a string for use in live stream
String ipAddress2String(const IPAddress& ipAddress) {
  return String(ipAddress[0]) + String(".") +\
  String(ipAddress[1]) + String(".") +\
  String(ipAddress[2]) + String(".") +\
  String(ipAddress[3]); 
}

// Live Stream feed virtual pin and setup
BLYNK_WRITE(V12){
  Blynk.setProperty(V13, "url", ""); //  set the stream url to blank initially
  IPAddress ip = WiFi.localIP();
  String ipaddress = ipAddress2String(ip);
  String streamUrl = "http://" + ipaddress + "/stream";
  delay(1000); // might not be necessary
  // Write url to terminal
  Blynk.virtualWrite(V0, streamUrl);
  // Set the live stream url
  Blynk.setProperty(V13, "url", streamUrl);
  Blynk.syncVirtual(V13);
}

// Button to close/clear live stream url (must be done from different module in  Blynk app)
BLYNK_WRITE(V14){
  Blynk.setProperty(V13, "url", "http://example.com");
}

void loop() {
  Blynk.run();
  ledcWrite(ledCHannel, brightness);
  server.handleClient();
  if (Blynk.connected() && timeStatus() == 2 && go== true) {
    //time_t t = now();

    // Take picture
    if (take_picture()) {
      FTP_upload();
      go = false;
      return;
    } else {
      Blynk.virtualWrite(V0, "Capture failed, sleeping");
      return;
    }
  }
  // Enable deep sleep
  /* if( millis() > CON_TIMEOUT)
  {
    Blynk.virtualWrite(V0, "Timeout");
    deep_sleep();
  }
 */
}

void deep_sleep() {
  Blynk.virtualWrite(V0, "Going to sleep after: " + String( millis() ) + "ms");
  Serial.flush();

  esp_deep_sleep_start();
}

// Get the time in ms
time_t milliSec() { now(); return(millis()); }

bool take_picture() {
  Blynk.virtualWrite(V0, "Taking picture now");
  fb = esp_camera_fb_get();

  if(!fb) {
    Blynk.virtualWrite(V0, "Camera capture failed");
    return false;
  }
  
  // Rename the picture with the time string
  pic_name += String( now() ) + " - " + String(milliSec()) + ".jpg";
  Blynk.virtualWrite (V0, "Camera capture success, saved as:");
  Blynk.virtualWrite (V0, pic_name);

  // ---------------------------------------------
  // ----------- START SD CARD CODE --------------
  // ---------------------------------------------
  
  // initialize EEPROM with predefined size
  EEPROM.begin(EEPROM_SIZE);
  pictureNumber = EEPROM.read(0) + 1;

  // Path where new picture will be saved in SD Card
  String path = "/picture" + String(pictureNumber) +".jpg";

  // Set SD_MMC Object as the file system
  fs::FS &fs = SD_MMC; 
  
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Blynk.virtualWrite(V0, "Failed to open file in writing mode");
  } else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Blynk.virtualWrite(V0, "Saved file to path: " + String(path));
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
  }
  file.close();
  return true;
  // ---------------------------------------------
  // ------------- END SD CARD CODE --------------
  // ---------------------------------------------
}

void FTP_upload() {
  Blynk.virtualWrite(V0, "Uploading via FTP");
  ftp.OpenConnection();
  
  //Create a file and write the image data to it;
  ftp.InitFile("Type I");
  //ftp.ChangeWorkDir("/main/images"); // change it to reflect your directory
  const char *f_name = pic_name.c_str();
  ftp.NewFile( f_name );
  ftp.WriteData(fb->buf, fb->len);
  ftp.CloseFile();

  // Change URL on Blynk App
  pic_url += "https://camerafly.app/images/";
  pic_name.replace(" ", "%20");
  pic_url += pic_name;
  Blynk.virtualWrite(V0, "Change App URL to: ");
  Blynk.virtualWrite(V0,  pic_url);
  Blynk.setProperty(V1, "url", cam_module.toInt(), pic_url);

  Blynk.virtualWrite(V0, "restarting...");
  // Breath, without delay URL failed to update.
  delay(100);

  ESP.restart();
}
