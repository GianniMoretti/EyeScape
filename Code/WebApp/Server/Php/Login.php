<?php
    session_start();
	require_once("Config.php");
    $id = $_POST["user"];
    $psw = $_POST["psw"];
    global $mysqli;

	$query_string = "SELECT * FROM Users WHERE name='".$id."' AND password='".$psw."'";

    $result = $mysqli->query($query_string);

	while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
       $_SESSION["userID"] = $row;
	}

	if(isset($_SESSION["userID"]))
	    $response = array('login' => "true");
	else
	    $response = array('login' => "false");


    echo json_encode($response);

?>