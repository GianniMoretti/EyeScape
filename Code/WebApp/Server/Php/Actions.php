<?php

    session_start();
	header('Content-Type: text/json');
	require_once("Config.php");
	$action = $_POST['action'];
	$query_string = "";
	switch($action) {


		case "load" :
			loadData();
		break;
		case "insert" :
			insertData();
		break;
		case "update" :
	   		updateData();
		break;
		case "delete" :
			deleteData();
		break;
		case "logout" :
		    session_destroy();
		break;
        case "deviceload":
            deviceLoad();
        break;
        case "deviceupdate":

            deviceUpdate();
            echo json_encode("yess");
        break;
	}



	function loadData() {
		global $mysqli, $_SESSION;
		$query_string = 'SELECT name, aquaID FROM aquarium';
		$result = $mysqli->query($query_string);
    	$names = array();
    	$id = array();
        $user = $_SESSION["userID"]["name"];
		while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
			$names[] = $row["name"];
			$id[] = $row["aquaID"];
		}
    	$response = array('user'=> $user,'names' => $names, 'id' => $id, 'type' => 'load');
		echo json_encode($response);

}

	function insertData() {
        if (isset($_POST['text'])) {
            $aquarium_name = $_POST['text'];
        } else {
            echo "you didn't specify a text";
            return;
        }
        global $mysqli;
		$query_string = "INSERT INTO aquarium (name) values ('". htmlspecialchars($aquarium_name) . "')";
		$mysqli->query($query_string);

		echo json_encode("yess");

	}

	function deleteData() {
        global $mysqli;
		if (isset($_POST['id'])) $id = $_POST['id'];
			$pieces = explode("_", $id);
			$query_string = 'DELETE FROM to_do WHERE ID=' . $pieces[0];
			$result = $mysqli->query($query_string);


    		if($mysqli->affected_rows > 0) {
	  			$response = array('deleted' => true, 'id' => $id, 'type' => 'delete');
			} else {
	  			$response = array('deleted' => false, 'id' => $id, 'type' => 'delete');
	  		}

			echo json_encode($query_string);
	}

    function deviceUpdate(){
        global $mysqli;
        if (isset($_POST['id'])) $id = $_POST['id'];
        if (isset($_POST['status'])) $status = $_POST['status'];

        $query_string = 'UPDATE device SET status="' . $status . '" WHERE devID=' . $id;
        $mysqli->query($query_string);

        echo json_encode("ok");
    }
    function deviceLoad(){
        global $mysqli, $_SESSION;
        $query_string = 'SELECT * FROM device';
        $result = $mysqli->query($query_string);
        $id = array();
        $status = array();
        while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
            $id[] = $row["devID"];
            $status[] = $row["status"];
        }
        $response = array('status' => $status, 'id' => $id, 'type' => 'deviceload');
        echo json_encode($response);
    }

?>