<?php
    session_start(); /* Starts the session */
    session_destroy(); /* Destroy started session */
    $_SESSION = [];
    header("location:login.php");  /* Redirect to login page */
    exit;
?>