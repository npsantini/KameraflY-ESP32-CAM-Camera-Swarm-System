<?php include 'login/login-conf.php' ?>
<!doctype html>

<html>
    <title>Camerafly - Camera Swarm</title>
    <head>
        <!-- Stylesheets -->
        <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.5.2/css/bootstrap.min.css" integrity="sha384-JcKb8q3iqJ61gNV9KGb8thSsNjpSL0n8PARn9HuZOnIxN0hoP+VmmDGMN5t9UJ0Z" crossorigin="anonymous">
        <!-- END Stylesheets -->
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
            .logo {background: #b0b0b0;text-align:center;}
            .logo img{max-width: 200px;}
        </style>
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/5.15.4/css/all.min.css" integrity="sha512-1ycn6IcaQQ40/MKBW2W4Rhis/DbILU74C1vSrLJxCq57o941Ym01SwNsOMqvEBFlcgUa6xLiPY/NS5R+E6ztJQ==" crossorigin="anonymous" referrerpolicy="no-referrer" />
    </head>
	<body class="bg-dark">
        <div class="container-fluid">
            <div class="px-lg-5">
                <div class="row">
                    <div class='col-xl-12 col-lg-12 col-md-12 mb-4 mt-4'>
                        <div class="bg-white rounded shadow-sm">
                            <div class="logo">
                                <img src="https://camerafly.app/camerafly-logo-v2.jpg" alt="camerafly logo" class="logo">
                            </div>
                            <div class='p-2'>
                                <h2 class="text-dark text-center">Swarm Image Gallery</h2>
                            </div>
                        </div>
                    </div>
                </div>
                <!-- <div class="row">
                    <div class='col-xl-12 col-lg-12 col-md-12 mb-4 mt-4'>
                        <div class="bg-white rounded shadow-sm">
                            <div class='p-2'>
                                <a href="#module1">Sorted by Modules</a> | 
                                <a href="#capture1">Sorted by capture</a>
                            </div>
                        </div>
                    </div>
                </div> -->
                <div class="row">
                    <div class='col-xl-3 col-lg-4 col-md-6 mb-4'>
                        <?php 
                            echo "<form method='post' action=''>
                            <input type='submit' name='create' value='Create Zip' class='btn btn-primary'/>&nbsp;
                        </form>";
                            echo "<form method='post' action=''>
                                <input type='submit' name='deleteAll' value='Delete All' class='btn btn-danger'/>&nbsp;
                            </form>";
                        // Create ZIP file
                        define('IMAGEPATH', '/home/nicksantini/public_html/camerafly.app/images/');
                        if(isset($_POST['create'])){
                            $files = [];
                            foreach(glob(IMAGEPATH.'*'.'jpg') as $filename){
                                $files[] =  basename($filename);
                            }
                            $zip = new ZipArchive();
                            $zip_name = "Camerafly-All-Images.zip"; // Zip name
                            $zip->open($zip_name,  ZipArchive::CREATE);
                            foreach ($files as $file) {
                                $path = "/home/nicksantini/public_html/camerafly.app/images/".$file;
                                if(file_exists($path)){
                                    $zip->addFromString(basename($path),  file_get_contents($path));  
                                } else{
                                    echo"file does not exist";
                                }
                            }
                            $zip->close();
                            echo "<a href='Camerafly-All-Images.zip' class='btn btn-primary' download>Download All Images</a>";
                        }

                        // Create zip
                        function createZip($zip,$dir){
                            if (is_dir($dir)){
                                if ($dh = opendir($dir)){
                                    while (($file = readdir($dh)) !== false){
                                        // If file
                                        if (is_file($dir.$file)) {
                                            if($file != '' && $file != '.' && $file != '..'){
                                                $zip->addFile($dir.$file);
                                            }
                                        }else{
                                            // If directory
                                            if(is_dir($dir.$file) ){
                                                if($file != '' && $file != '.' && $file != '..'){
                                                    // Add empty directory
                                                    $zip->addEmptyDir($dir.$file);
                                                    $folder = $dir.$file.'/';
                                                    // Read data of the folder
                                                    createZip($zip,$folder);
                                                }
                                            }
                                
                                        }
                                    }
                                    closedir($dh);
                                }
                            }
                        }
                    ?>
                    </div>
                </div>
                    <?php define('IMAGEPATH', '/home/nicksantini/public_html/camerafly.app/images/');
                    foreach(glob(IMAGEPATH.'*'.'jpg') as $filename){
                        $imag[] =  basename($filename);
                    }
                    function startsWith ($string, $startString){
                        $len = strlen($startString);
                        return (substr($string, 0, $len) === $startString);
                    }
                    function deleteImage($filename) {
                        if (file_exists($filename) && ($_SESSION['UserData']['Username']=='Camerafly#123')) {
                            unlink($filename);
                        } else {
                            echo "<div class='row'>
                                    <div class='col-xl-12 col-lg-12 col-md-12 mb-4 mt-4'>
                                        <div class='alert alert-danger rounded shadow-sm p-2'>
                                            <span>You don't have permission to delete the file!</span>
                                        </div>
                                    </div>
                                </div>";
                        }
                    }
                    
                    if (isset($_GET['delete'])) {
                        deleteImage($_GET['delete']);
                    }

                    function deleteAll() {
                        if ($_SESSION['UserData']['Username']=='Camerafly#123') {

                            $files = glob(IMAGEPATH.'*'.'jpg'); // get all file names
                            foreach($files as $file){ // iterate files
                                if(is_file($file)) {
                                    unlink($file); // delete file
                                }
                            }
                            echo "<div class='row'>
                                    <div class='col-xl-12 col-lg-12 col-md-12 mb-4 mt-4'>
                                        <div class='alert alert-danger rounded shadow-sm p-2'>
                                            <span>You deleted all of the files!</span>
                                        </div>
                                    </div>
                                </div>";
                        } else {
                            echo "<div class='row'>
                                    <div class='col-xl-12 col-lg-12 col-md-12 mb-4 mt-4'>
                                        <div class='alert alert-danger rounded shadow-sm p-2'>
                                            <span>You don't have permission to delete the file!</span>
                                        </div>
                                    </div>
                                </div>";
                        }
                    }

                    if(isset($_POST['deleteAll'])){
                        deleteAll();
                    }


                    $totalModules = 16;
                    $module = 0;
                    echo "<div class='row'>";
                    for($x = 0; $x < count($imag); $x++){
                            echo "<!-- Gallery item -->
                            <div class='col-xl-3 col-lg-4 col-md-6 mb-4'>
                                <div class='bg-white rounded shadow-sm'>
                                    <a href='https://camerafly.app/images/",$imag[$x],"' target='_blank'>
                                        <img src='https://camerafly.app/images/", $imag[$x], "' alt='' class='img-fluid card-img-top'>
                                    </a>
                                    <div class='p-2'>
                                        <a href='https://camerafly.app/images/",$imag[$x],"' class='text-dark' target='_blank'>", $imag[$x],"</a>
                                    </div>
                                    <div class='p-2 row'>
                                        <div class='col text-center'>
                                            <a download='",$imag[$x],"' href='https://camerafly.app/images/",$imag[$x],"' title='",$imag[$x],"'>
                                                <i class='fas fa-download'></i>
                                            </a>
                                        </div>
                                        <div class='col text-center'>
                                            <a href='?delete=images/",$imag[$x],"'>
                                                <i class='fas fa-trash-alt'></i>
                                            </a>
                                        </div>
                                    </div>
                                </div>
                            </div>
                            <!-- End -->";
                        }
                        echo "</div>";
                    
                    ?>
                </div>
            </div>
        </div>
        <script src="https://code.jquery.com/jquery-3.6.0.min.js" integrity="sha256-/xUj+3OJU5yExlq6GSYGSHk7tPXikynS7ogEvDej/m4=" crossorigin="anonymous"></script>
        <script>
        $(document).ready(function(){
            for (i = 1; i < 17; i++) {
                var className = "#module" + i;
                var isEmpty = $(className).children().length === 0;
                if (isEmpty){
                    $(className).prev().css("display", "none");
                }
            }
        });
        $(document).ready(function(){
            for (i = 0; i < 500; i++) {
                var className = "#capture" + i;
                var isEmpty = $(className).children().length === 0;
                if (isEmpty){
                    $(className).prev().css("display", "none");
                }
            }
        });
        </script>
    </body>
</html>