<?php

session_start();
header('Content-Type: text/json');
require_once("Config.php");
$action = $_POST['action'];
$query_string = "";

switch ($action) {

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
    case "getAquariumData":
        getAquarium();
        break;
    case "getID":
        getID();
        break;
}

function loadData()
{
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
    $response = array('user' => $user, 'names' => $names, 'id' => $id, 'type' => 'load');
    echo json_encode($response);

}

function insertData()
{
    if (isset($_POST['name'])) {
        $aquarium_name = $_POST['name'];
    } else {
        echo "you didn't specify a text";
        return;
    }
    if (isset($_POST['description'])) {
        $aquarium_description = $_POST['description'];
    } else {
        echo "you didn't specify a description";
        return;
    }
    if (isset($_POST['timeStart'])) {
        $aquarium_time_start = $_POST['timeStart'];
    } else {
        echo "you didn't specify a time";
        return;
    }
    if (isset($_POST['timeDuration'])) {
        $aquarium_duration = $_POST['timeDuration'];
    } else {
        echo "you didn't specify a time";
        return;
    }
    if (isset($_POST['brightness'])) {
        $aquarium_brightness = $_POST['brightness'];
    } else {
        echo "you didn't specify a text";
        return;
    }

    $aquarium_autoRefill = 0;
    $aquarium_onOff = 0;

    global $mysqli;
    $query_string = "INSERT INTO `aquarium`(`aquaID`, `name`, `description`, `startLight`, `lightDuration`, `luminosityPercentage`, `automaticRefill`, `onOffLight`) VALUES ('0','" . htmlspecialchars($aquarium_name) . "','" . htmlspecialchars($aquarium_description) . "','" . htmlspecialchars($aquarium_time_start) . "','" . htmlspecialchars($aquarium_duration) . "','" . htmlspecialchars($aquarium_brightness) . "','" . htmlspecialchars($aquarium_autoRefill) . "','" . htmlspecialchars($aquarium_onOff) . "')";

    $mysqli->query($query_string);
    echo json_encode("ok");

}


function getID()
{
    if (isset($_POST['name'])) {
        $aquarium_name = $_POST['name'];
    } else {
        echo "you didn't specify a text";
        return;
    }
    global $mysqli;
    $query_string = "SELECT aquaID FROM aquarium WHERE name = '" . $aquarium_name . "'";;

    $result = $mysqli->query($query_string);
    $id = array();
    while($row = $result->fetch_array(MYSQLI_ASSOC)){
        $id = $row["aquaID"];
    }

    $response = array( 'id' => $id );
    echo json_encode($response);

}

function deleteData()
{
    global $mysqli;
    if (isset($_POST['id'])) $id = $_POST['id'];
    $pieces = explode("_", $id);
    $query_string = 'DELETE FROM to_do WHERE ID=' . $pieces[0];
    $result = $mysqli->query($query_string);


    if ($mysqli->affected_rows > 0) {
        $response = array('deleted' => true, 'id' => $id, 'type' => 'delete');
    } else {
        $response = array('deleted' => false, 'id' => $id, 'type' => 'delete');
    }

    echo json_encode($query_string);
}

function deviceUpdate()
{
    global $mysqli;
    if (isset($_POST['id'])) $id = $_POST['id'];
    if (isset($_POST['status'])) $status = $_POST['status'];

    $query_string = 'UPDATE device SET status="' . $status . '" WHERE devID=' . $id;
    $mysqli->query($query_string);

    echo json_encode("ok");
}

function deviceLoad()
{
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

function updateData()
{
    global $mysqli;

    if (isset($_POST['id'])) $id = $_POST['id'];
    if (isset($_POST['field'])) $field = $_POST['field'];
    if (isset($_POST['modify'])) $modify = $_POST['modify'];

    $query_string = "UPDATE `aquarium` SET " . $field . " = '" . $modify . "' WHERE aquaID =  '" . $id . "'";
    $mysqli->query($query_string);

    echo json_encode("ok");
}

function getAquarium()
{
    global $mysqli, $_SESSION;

    if (isset($_POST['id'])) $id = $_POST['id'];
    $query_string = "SELECT * FROM aquarium WHERE aquaID = '" . $id . "'";
    $result = $mysqli->query($query_string);
    $names = array();
    $id = array();
    $description = array();
    $startLight = array();
    $lightDuration = array();
    $luminosityPercentage = array();
    $autoRefill = array();
    $onOffLight = array();

    while ($row = $result->fetch_array(MYSQLI_ASSOC)) {
        $names[] = $row["name"];
        $id[] = $row["aquaID"];
        $description[] = $row["description"];
        $startLight[] = $row["startLight"];
        $lightDuration[] = $row["lightDuration"];
        $luminosityPercentage[] = $row["luminosityPercentage"];
        $autoRefill[] = $row["automaticRefill"];
        $onOffLight[] = $row["onOffLight"];
    }
    $response = array('names' => $names, 'id' => $id, 'description' => $description, 'startLight' => $startLight,
        'lightDuration' => $lightDuration, 'luminosityPercentage' => $luminosityPercentage, 'automaticRefill' => $autoRefill,
        'onOffLight'=> $onOffLight, 'type' => 'load');
    echo json_encode($response);

}
?>