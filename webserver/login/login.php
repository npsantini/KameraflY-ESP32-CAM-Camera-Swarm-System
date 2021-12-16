<?php 
    session_start(); /* Starts the session */
    /* Check Login form submitted */
    if(isset($_POST['Submit'])){
        /* Define username and associated password array */
        $logins = array('admin' => 'password','user' => 'password');

        /* Check and assign submitted Username and Password to new variable */
        $Username = isset($_POST['Username']) ? $_POST['Username'] : '';
        $Password = isset($_POST['Password']) ? $_POST['Password'] : '';

        /* Check Username and Password existence in defined array */
        if (isset($logins[$Username]) && $logins[$Username] == $Password){
            /* Success: Set session variables and redirect to Protected page  */
            $_SESSION['UserData']['Username']=$logins[$Username];
            header("location:../index.php");
            exit;
        } else {
            /*Unsuccessful attempt: Set error message */
            $msg="<span style='color:red'>Invalid Login Details</span>";
        }
    }
?>
<!doctype html>

<html>
    <title>Camerafly - Login</title>
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
                    <div class='col-xl-4 col-lg-4 col-md-4 col-sm-4 col-xs-12 mb-4 mt-4'></div>
                    <div class='col-xl-4 col-lg-4 col-md-4 col-xs-12 mb-4 mt-4'>
                        <div class="bg-white rounded shadow-sm">
                            <div class="logo">
                                <img src="https://camerafly.app/camerafly-logo-v2.jpg" alt="camerafly logo" class="logo">
                            </div>
                            <div class='p-2'>
                                <h2 class="text-dark text-center">Camerafly Login</h2>
                            </div>
                        </div>
                    </div>
                    <div class='col-xl-4 col-lg-4 col-md-4 col-sm-4 col-xs-12 mb-4 mt-4'></div>
                </div>
                <div class="row">
                    <div class='col-xl-4 col-lg-4 col-md-4 col-sm-4 col-xs-12 mb-4 mt-4'></div>
                    <div class='col-xl-4 col-lg-4 col-md-4 col-sm-4 col-xs-12 mb-4 mt-4'>
                        <div class="bg-white rounded shadow-sm">
                            <form action="" method="post" name="Login_Form">
                                <table border="0" align="center" cellpadding="5" cellspacing="1" class="Table">
                                    <?php if(isset($msg)){?>
                                    <tr>
                                    <td colspan="2" align="center" valign="top"><?php echo $msg;?></td>
                                    </tr>
                                    <?php } ?>
                                    <tr>
                                    <td valign="top">Username</td>
                                    <td><input name="Username" type="text" class="Input"></td>
                                    </tr>
                                    <tr>
                                    <td>Password</td>
                                    <td><input name="Password" type="password" class="Input"></td>
                                    </tr>
                                    <tr>
                                    <td> </td>
                                    <td><input name="Submit" type="submit" value="Login" class="Button3"></td>
                                    </tr>
                                    <tr>
                                        <td><hr><u>Default Login</u><br>Username: <b>user</b><br> Password: <b>password</b></td>
                                    </tr>
                                </table>
                            </form>
                        </div>
                    </div>
                    <div class='col-xl-4 col-lg-4 col-md-4 col-sm-4 col-xs-12 mb-4 mt-4'></div>
                </div>
            </div>
        </div>
    </body>
</html>