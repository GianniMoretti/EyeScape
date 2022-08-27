<?php
	/* REQUIRE DB
		CREATE TABLE to_do (
  			id int(11) NOT NULL AUTO_INCREMENT,
  			text text NOT NULL,
  			completed tinyint(255) NOT NULL DEFAULT '0',
  			date timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  			PRIMARY KEY (id)
		) ENGINE=MyISAM  DEFAULT CHARSET=latin1 AUTO_INCREMENT=109 ;
	*/
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
/*
	function loadData() {

		$query_string = 'SELECT * FROM aquarium ORDER BY date DESC';
		$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
		$result = $mysqli->query($query_string);
    	$aquariums = array();

		while ($row = $result->fetch_array(MYSQLI_ASSOC)) {

			$aquarium_id = $row['id'];
			$aquarium_name = $row['name'];
			$todo = array('id' => $aquarium_id,'name' =>$aquarium_name);
			array_push($aquariums, $aquarium);

		}

    	$response = array('aquarium' => $aquariums, 'type' => 'load');
		echo json_encode($response);

}*/

	function insertData() {

		if (isset($_POST['name'])) {
			$aquarium_name = $_POST['name'];
		} else {
			echo "you didn't specify a name";
			return;
		}

		$query_string = "INSERT INTO aquarium (text) values ('". htmlspecialchars($aquarium_name) . "')";
		$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
		$result = $mysqli->query($query_string);
    	$query_string = 'SELECT * FROM aquarium WHERE ID=' . $mysqli->insert_id;
		$result = $mysqli->query($query_string);
    	$aquariums = array();

		while ($row = $result->fetch_array(MYSQLI_ASSOC)) {

			$aquarium_id = $row['id'];
  			$aquarium_name = $row['name'];
			$aquarium = array('id' => $aquarium_id,'name' =>$aquarium_name);
			array_push($aquariums, $aquarium);

		}

    	$response = array('aquariums' => $aquariums, 'type' => 'insert');
		echo json_encode($response);

	}

	/*function updateData() {
		if (isset($_POST['id'])) $id = $_POST['id'];
		if (isset($_POST['status'])) $status = $_POST['status'];

		$pieces = explode("_", $id);
		$query_string = 'UPDATE aquarium SET completed=' . $status . ' WHERE ID=' . $pieces[1];

		$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);

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

}*/


	/*function deleteData() {

		if (isset($_POST['id'])) $id = $_POST['id'];
			$pieces = explode("_", $id);
			$query_string = 'DELETE FROM aquarium WHERE ID=' . $pieces[1];
			$mysqli = new mysqli(DB_HOST, DB_USER, DB_PASSWORD, DB_DATABASE);
			$result = $mysqli->query($query_string);

    		if($mysqli->affected_rows > 0) {
	  			$response = array('deleted' => true, 'id' => $id, 'type' => 'delete');
			} else {
	  			$response = array('deleted' => false, 'id' => $id, 'type' => 'delete');
	  		}

			echo json_encode($response);
	}

?>*/