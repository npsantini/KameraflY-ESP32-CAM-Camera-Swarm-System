<?php
    session_start(); /* Starts the session */
    if(!isset($_SESSION['UserData']['Username'])){
        header("location:login/login.php");
        exit;
    }
?>

<div class="loginMessage bg-white rounded shadow-sm">
    <span><a href="login/logout.php">Click here</a> to Logout.</span>
</div>