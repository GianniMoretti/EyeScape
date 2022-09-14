<?php

	header('Content-Type: text/json');
	require_once("Config.php");
	$action = $_POST['action'];
	$query_string = "";

	switch($action) {

		case "load" :
			loadData();
		break;
		case "insert" :
			//echo($action);
			insertData();
		break;
		case "update" :
	   		updateData();
		break;
		case "delete" :
			deleteData();
		break;
	}

	function loadData() {
		global $mysqli;
		$query_string = 'SELECT * FROM to_do ORDER BY date DESC';
		$result = $mysqli->query($query_string);
    	$todos = array();
		while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
			array_push($todos, $row);
		}
    	$response = array('todos' => $todos, 'type' => 'load');
		echo json_encode($response);

}

	function insertData() { /*il sendToDo usa questa ********************/
        global $mysqli;
		$query_string = "INSERT INTO to_do (text) values ('". htmlspecialchars($to_do_text) . "')";
		$result = $mysqli->query($query_string);

    	$query_string = 'SELECT * FROM to_do WHERE ID=' . $mysqli->insert_id;
		$result = $mysqli->query($query_string);
    	$todos = array();
		while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
			array_push($todos, $todo);
		}

    	$response = array('todos' => $todos, 'type' => 'insert');
		echo json_encode($response);

	}

	function updateData() {
		global $mysqli;
		if (isset($_POST['id'])) $id = $_POST['id'];
		if (isset($_POST['status'])) $status = $_POST['status'];

		$pieces = explode("_", $id);

		$query_string = 'UPDATE to_do SET completed=' . $status . ' WHERE ID=' . $pieces[1];

    	// esegui la query
		$result = $mysqli->query($query_string);

		//echo $query_string;

    	if($mysqli->affected_rows > 0) {
		// encodo l'array in JSON

	  		$response = array('updated' => true, 'id' => $id, 'type' => 'update');

		} else {
	  		$response = array('updated' => false, 'id' => $id, 'type' => 'update');
		}

	echo json_encode($response);

}


	function deleteData() {
        global $mysqli;
		if (isset($_POST['id'])) $id = $_POST['id'];
			$pieces = explode("_", $id);
			$query_string = 'DELETE FROM to_do WHERE ID=' . $pieces[1];
			$result = $mysqli->query($query_string);

    		if($mysqli->affected_rows > 0) {
	  			$response = array('deleted' => true, 'id' => $id, 'type' => 'delete');
			} else {
	  			$response = array('deleted' => false, 'id' => $id, 'type' => 'delete');
	  		}

			echo json_encode($response);
	}

?>