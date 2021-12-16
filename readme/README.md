<div id="top"></div>

# camerafly

<table width="100%">
    <tr>
	    <td width="33.33%"></td>
	    <td width="33.33%">
		    <img src="https://github.com/npsantini/camerafly/blob/main/camerafly-logo-v2.jpg" width="100%" alt="camerafly logo">
	    </td>
	    <td width="33.33%"></td>
    </tr>
</table>

## Contents
- <a href="#firmware">Overview</a>
- <a href="#firmware">ESP32-CAM Firmware</a>
    - <a href="#arduino">Arduino IDE</a>
    - <a href="#libraries">Libraries</a>
    - <a href="#functions">Functions</a>
    - <a href="#firmware-improvement">Potential Improvements</a>
- <a href="#blynk">Blynk App</a>
    - <a href="#setup">Setup</a>
    - <a href="#components">Components</a>
    - <a href="#blynk-improvement">Potential Improvements</a>
- <a href="#web">Web App</a>
    - <a href="#requirements">Requirements</a>
    - <a href="#web-improvement">Potential Improvements</a>
- <a href="#pcb">PCB Info</a>
	- <a href="#pcb-improvement">Potential Improvements</a>
- <a href="#housing">Housing</a>
	- <a href="#housing-improvement">Potential Improvements</a>
- <a href="#parts">Parts</a>
	- <a href="#partslist">Parts List</a>
	- <a href="#parts-improvement">Potential Improvements</a>

<hr>
<!-- Overview -->
<div id="overview"></div>

### Overview
<hr>
<p>
Our current ESP32-CAM camera swarm is a completly wireless swarm that is capable of caturing synchronized images down to 10ms.
The system we have created allows a user to use and Android or iOS app to control the camera swarm. When images are captured they 
are sent to a web server where all of the images can be downloaded at once. The images collected from the web server can be used 
in many different applications like 3D modeling or for use in 3D point cloud software.</p>

<p>Each of our completed modules costs only $13.43. When scaled to 256 modules the cost $6.96 per module. This makes this camera swarm 
an extremely cost effective tool that is a lot more accessible than traditional DSLR focused camera swarms.</p>

<p align="right">(<a href="#top">back to top</a>)</p>
<hr>
<!-- ESP32-CAM Firmware -->
<div id="firmware"></div>

### ESP32-CAM Firmware
<hr>
The ESP32-CAM modules in our swarm were all programmed to work with the Blynk app. This was mainly due to time constraints and the ease of use. It is setup in a way to perform only a few actions like connect to the Blynk app, save images to an SD card, upload an image via FTP, and live stream video. There are improvements that can be made and it's far from perfect. It satisfied our engineering requirements, which is all we had set out to accomplish.

I encourage anyone that would like to continue this to fork this repository or make pull requests.

<div id="arduino"></div>

#### Arduino IDE
The Arduino IDE is what was used for ease of use. It is possible to also use <a href="https://platform.io" target="_blank">PlatformIO</a> although we have not tested it.
It's important to include all of the libraries that you will see in the next section. It is also important that in the change the information in the <a href="#">secrets.h</a> file.
```C++
// secrets.h file
#define SECRET_SSID "INSERT WIFI SSID HERE"
#define SECRET_PASSWORD "INSERT WIFI PASSWORD HERE"

#define SECRET_FTP_SERVER "INSERT FTP SERVER IP ADDRESS HERE"
#define SECRET_FTP_USERNAME "INSERT FTP USERNAME HERE" // ex. user@example.com
#define SECRET_FTP_PASSWORD "INSERT FTP PASSWORD"
```
<div id="libraries"></div>

#### Libraries
In order to include all of the necessary libraries for the ESP32-CAM's you need to 
add the following additional board manager in the Arduino IDE under `File > Preferences`:
<br>
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

##### Included packages from the source above:
- SD_MMC
- WebServer
- EEPROM
- FS
- WiFi
- And more.

You might have conflicting wifi libraries. You will have to fix this by removing one of the libraries that the error message lists.

##### Libraries in Arduino Library Manager
- Blynk (Volodymyr Shymanskyy) v0.6.1
- esp32_ftpclient (Leonardo Bispo) v0.1.4
- SD (Arduino) v1.2.4
- Time (Paul Stoffregen) v1.6.1

<div id="functions"></div>

#### Functions
- Live Stream Functions (Anatoli Arkhipenko, https://github.com/arkhipenko/esp32-cam-mjpeg)
	- handle_jpg_stream(void)
	- handle_jpg()
	- handleNotFound()
- SD Card Functions (Rui Santos, https://RandomNerdTutorials.com/esp32-cam-take-photo-save-microsd-card)
	- SD_MMC.begin() // Check to see if sd card is mounted
	- SD_MMC.cardType() // Check card type
- FTP Functions
	- FTP_upload(void)
- Main Logic Functions
	- setup()
	- loop()
	- take_picture()
- Blynk Functions/Methods
	- Blynk.run()
	- BLYNK_CONNECTED()
	- BLYNK_WRITE(V5)
	- BLYNK_WRITE(V10)
	- BLYNK_WRITE(V12)
	- BLYNK_WRITE(V14)
- Misc. Functions
	- deep_sleep()
	- ipAddress2String(const IPAddress& ipAddress)

<div id="firmware-improvement"></div>

#### Potential Improvements

<p>
This firmware for the ESP32-CAM is not perfect. We encourage people make improvements as they see fit. Most of the code is to be specifically used with the Blynk Legacy mobile app. If you wish to use your own app it would need to be changed substantially.
</p>
<p>
One big improvement would be programming the ESP32-CAM modules to take an image at a specified time. This is something we were not able to accomplish with the current version of the Blynk app, but will be coming soon as dicussed in more detail below.
</p>

<p align="right">(<a href="#top">back to top</a>)</p>
<hr>

<!-- Blynk App -->
<div id="blynk"></div>

### Blynk App
<hr>
<p>
We chose to use the Legacy Blynk app to control our ESP32-CAM camera swarm. It's not a perfect solution, but is much more convenient than writing your own app to fulfill the same functionality. There are a lot of great things you can do with the Blynk app, however we have chosen to use what we believe to be the bare essentials necessary to control and effectively use the camera swarm.
</p>

<div id="setup"></div>

#### Setup
<p>Below are some images of the basic setup of each ESP32-CAM module and how to add all of the modules to one tag. In this case, we named the tag "All",
but you could name it whatever you like. Having all of the modules with the same tag allows us to use one button to capture all of the images from each of the cameras in the camera swarm simultaneously.</p>

<table>
    <tr>
        <td width="20%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/blynk-2.JPG" alt=""></td>
        <td width="20%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/blynk-4.JPG" alt=""></td>
        <td width="20%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/blynk-5.JPG" alt=""></td>
        <td width="20%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/blynk-6.JPG" alt=""></td>
        <td width="20%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/blynk-7.JPG" alt=""></td>
    </tr>
    <tr>
	    <td valign="top" width="20%">
		    <p>- Go to "My Devices."</p>
		    <p>- Scroll down and click "+ New Device."</p>
	    </td>
	    <td valign="top" width="20%">
		    <p>- Name the module (numbers help identify the module later).</p>
		    <p>- Select "ESP32 Dev Board.</p>
		    <p>- Choose "Wi-Fi".</p>
		    <p>- Click "Create".</p>
	    </td>
	    <td valign="top" width="20%">
		    <p>- You will then see a list of modules that you have created.</p>
		    <p>- Now create a new tag by clicking "+ New Tag"</p>
	    </td>
	    <td valign="top" width="20%">
		    <p>- Name the tag "All"</p>
		    <p>- Once created check off all of the modules you want to add to the tag.</p>
	    </td>
	    <td valign="top" width="20%">
		    <p>- In the project settings click "Email all" to get all of the Authorization tokens that you will need to include in the firmware.</p>
	    </td>
    </tr>
</table>
<p>
The most important part of the setup is that you get all of the Authorization tokens (emailing them from the app is easiest) and add them to the section of code below. Make sure you only have one authorization token uncommented at a time. For each ESP32-CAM that you program you need to uncomment a new token and change the `cam_module` number to the current module you are using. This will allow you to identify which module each picture came from and helps with troubleshooting the modules.
</p>

```c++
String cam_module = "4"; 
//char auth[] = "MODULE 1 AUTH TOKEN HERE"; //1
//char auth[] = "MODULE 2 AUTH TOKEN HERE"; //2
//char auth[] = "MODULE 3 AUTH TOKEN HERE"; //3
char auth[] = "MODULE 4 AUTH TOKEN HERE"; //4
//char auth[] = "MODULE 5 AUTH TOKEN HERE"; //5
// etc...
```
<div id="components"></div>

#### Components
<p>While we chose to use these specific components to accomplish what we needed to accomplish, you can choose what you you need and can remove the rest. Just make sure to remove the code for components that you aren't using.</p>
<table>
	<tr>
		<td width="40%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/blynk-8.JPG" alt=""></td>
		<td width="60%">
	
- Device Selector

- Slider
	- Target: selector
	- Output: 0 - 4
	- Pin: V10

- Button
	- Target: All tag
	- Output: 0 - 1
	- Pin: V5

- Terminal
	- Target: Selector
	- Add Newline: Yes
	- Autoscroll: Yes
	- Pin: V0

- Video Stream
	- Target: Selector
	- URL: 
	- Pin: V13

- Styled Button (Video Stream Start)
	- Target: Selector
	- Output: 0 - 1
	- Pin: V12


- Styled Button (Video Stream Stop)
	- Target: All tag
	- Output: 0 - 1
	- Pin: V14

- Real-time Clock
	- Timezone: (Whatever you wish to use)

		</td>
	</tr>
</table>

<div id="blynk-improvement"></div>

#### Potential Improvements
<p>There are improvements coming to the Blynk app that will allow for all of the components to be controlled via the Blynk Cloud web dashboard. This would greatly improve the usability and would elimintate the need to use a mobile phone. Being able to send specific signals such as a time in order to schedule an image capture would greatly improve the synchronization times of the images. We have been told that a lot of this functionality should arrive in Q2 2022.
</p>
<p>Until then we highly recommend using the Legacy Blynk app and not the Blynk IoT app.</p>

<p align="right">(<a href="#top">back to top</a>)</p>
<hr>

<!-- Web App -->
<div id="web"></div>

### Web App
<hr>
<p>
The web app that we have included in this is a PHP site displays the last images captured as well as images by each module. It is very basic, but allows you to collect all of the images in a zip file which is extremely useful when you need to import the images in other programs.
</p>

<p>
The login system is something that was added to make sure that people couldn't see the images that were captured unless they had a login. For this specific demo we have a default user and an admin user. Only the admin user has the ability to delete images.
</p>

<p>
<b>It is highly recommended that you change these usernames and passwords!</b>
</p>

- Default User
	- Username: user
	- Password: password

- Admin User
	- Username: admin
	- Password: password

<p>The usernames and passwords are defined as seen below in an array in the `login.php` page and should be changed.</p>

```PHP
$logins = array('admin' => 'password','user' => 'password');
```

<div id="requirements"></div>

#### Requirements

- Web Server
	- Apache
	- PHP v7.0 (or later)
- Optional
	- Web hosting
	- Domain name


<div id="web-improvement"></div>

#### Potential Improvements
<p>A better way of sorting the images would make this a lot better. Right now it relies on the file name of the images to sort them. This is obviously not ideal.</p>

<p align="right">(<a href="#top">back to top</a>)</p>
<hr>

<!-- PCB Info -->
<div id="pcb"></div>

### PCB Info
<hr>
<table>
	<tr>
		<td width="25%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/pcb.jpg" alt=""></td>
		<td width="75%">
			<p>The PCB subsystem was created to connect all components used for each module together so they are compact, and set up and transport easily. The PCB reduces the need for wires allowing the casing to be smaller. The PCB has dimensions of 50mm X 55mm. There is a switch in the circuit that will turn power off to the ESP32-CAM saving power.</p>
			<p>The routing on the board connects the output of the battery charger to the battery and then to the boost converter. This will increase the voltage from ~3.7V, the voltage of the battery, to 5V, a useful voltage for the ESP32-CAM. This circuit is also ran through a switch that allows the unit to be put in standby mode to save charge. Another switch is connected to two pins of the ESP32-CAM so they can be shorted out to place the camera in programming mode. The 2-pin header is to provide access to the camera for programming purposes.</p>
		</td>
	</tr>
</table>

<div id="pcb-improvement"></div>

#### Potential Improvements

<p>
We went through 5 different iterations of the PCB which can be found in the `pcb-shematic` folder. Many of the changes from version to version were to make the footprint as small as possible and to make changes to the battery connector. The battery connector is a part that likely needs to be changed with a better connector for the battery we used. Since we couldn't find the exact battery connector we made our own part in Autodesk Eagle.
</p>
<p>
It's highly recommended that you use these PCB schematics as a starting point for your own, unless you use the exact same parts as we did.
</p>
<p align="right">(<a href="#top">back to top</a>)</p>
<hr>

<!-- Housing -->
<div id="housing"></div>

### Housing
<hr>

<table>
	<tr>
		<td width="100%">
			<p>The housing is designed so the ESP32-CAM modules can be easily transported and handled comfortably. The housing includes at least 2 different types of mounting to allow for a variety of ways to setup the system. The two chosen were a Keyhole hanger and two tripod mounts. The case allows for access to the removable battery, the removable SD Card, the charging port for the battery, and the various switches used for power and low power mode. The most recent housing is 72 x 58 x 25 mm which is considered "handheld" and fits the components comfortably.
			</p>
			<p>The drafts of the housing and Autodesk Inventor files can be found in the `ESP32-CAM Housing` folder. 
			</p>
		</td>
	</tr>
	<tr>
		<td width="50%"><img src="https://github.com/npsantini/camerafly/blob/main/readme/housing.jpg" alt=""></td>
	<tr>
</table>

<div id="housing-improvement"></div>

#### Potential Improvements

<p>
The tolerances are really tight with the PCB. This is intentional as it allows the battery to fit against the leaf springs in the battery connector. It is however not as reliable as we would have liked it to be and improvements to the how the battery sits in the housing can and probably should be made.
</p>
<p>
The tolerances between the two pieces of the case are also tight and will likely vary with different 3D printers. This is an area which may need improvement.
</p>

<p align="right">(<a href="#top">back to top</a>)</p>
<hr>

<!-- parts -->
<div id="parts"></div>

### Parts
<hr>
<p>Below is a parts list for all of the different parts required to make the completed product. Most of the items can be found on Amazon, but some are specialty parts like the PCB and battery connector.</p>

<div id="partslist"></div>

<table>
	<tr>
		<th>Device</th>
		<th>Manufacturer</th>
		<th>Item</th>
	</tr>
	<tr>
		<td><a href="https://amzn.to/3pZpIXn" target="_blank">ESP32-CAM</a></td>
		<td>espressif</td>
		<td>Camera Module</td>
	</tr>
	<tr>
		<td><a href="https://amzn.to/3GQk8xg" target="_blank">BP-70A Battery</a></td>
		<td>Kastar</td>
		<td>3.7v Lithium-Ion Camera Battery</td>
	</tr>
	<tr>
		<td><a href="https://amzn.to/3GPXchW" target="_blank">DC-DC Step Up Power Module</a></td>
		<td>Weewooday</td>
		<td>Boost Converter</td>
	</tr>
		<tr>
		<td><a href="https://amzn.to/33BLKIn" target="_blank">TP4056 Charging Module</a></td>
		<td>Weewooday</td>
		<td>Battery charging circuit</td>
	</tr>
	<tr>
		<td><a href="https://amzn.to/3mbGQbh" target="_blank">1.75mm PLA Filament</a></td>
		<td>SUNLU</td>
		<td>Product Housing</td>
	</tr>
	<tr>
		<td><a href="https://amzn.to/3GG7lO2" target="_blank">2 Position Slide Switch</a></td>
		<td>Tnuocke</td>
		<td>Power/Programming switch</td>
	</tr>
	<tr>
		<td><a href="https://jlcpcb.com" target="_blank">Custom PCB</a></td>
		<td>jlcpcb.com</td>
		<td>PCB for each module</td>
	</tr>
	<tr>
		<td><a href="https://amzn.to/3mbpr2y" target="_blank">8-Pin Socket Connectors</a></td>
		<td>Comidox</td>
		<td>Headers for ESP32-CAM</td>
	</tr>
		<tr>
		<td><a href="https://www.te.com/usa-en/product-1746142-1.html" target="_blank">3-Pin Leaf Type Connector (1746142-1)</a></td>
		<td>TE Connectivity</td>
		<td>Battery Connector</td>
	</tr>
</table>

<div id="parts-improvement"></div>

#### Potential Improvements

<p>The boost converter that we used easy extremly sensitive to heat therefor hard to solder without destroying it. It is recommended that is be switched out for a more durable and reliable boost converter.</p>
<p>
The battery connector is not meant to be used with the specific battery we are using. It is close to being the right one and will work, but it is recommneded that you find a better connector if you plan to use that same battery.
</p>

<p align="right">(<a href="#top">back to top</a>)</p>