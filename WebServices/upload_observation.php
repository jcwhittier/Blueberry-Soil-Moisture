
<?php
include_once('config.php');

/*
var_dump($_REQUEST);
var_dump($_POST);
*/

$json = "";

if($_SERVER['REQUEST_METHOD'] == "POST"){	
		
	if ($_POST['key'] == $passkey){

		// Get data from POST response
		$node_id 		= isset($_POST['node_id']) ?
							intval($_POST['node_id']):					 
							//pg_escape_literal($dbconn, $_POST['node_id']) :
						 'NULL';
		/*	
		printf("%s\n", $node_id);
		printf("%s\n",(string)$node_id);
		printf("%d\n",(int)$node_id);
		*/

		$observation_time 	= isset($_POST['observation_time']) ?
							 pg_escape_literal($dbconn, $_POST['observation_time']) :
							 'NULL';
		
		$upload_time 		= isset($_POST['gateway_time']) ?
							pg_escape_literal($dbconn, $_POST['gateway_time']) :
							'NULL';


		$moisture		= isset($_POST['moisture']) ?
							pg_escape_literal($dbconn, $_POST['moisture']) :
							'NULL';
							
		$voltage		= isset($_POST['voltage']) ?
							pg_escape_literal($dbconn, $_POST['voltage']) :
							'NULL';
							
		$analog_value		= isset($_POST['analog_value']) ?
							pg_escape_literal($dbconn, $_POST['analog_value']) :
							'NULL';					
							
		
	
		$dataToInsert = array(	
					"analog_value" => array ( (($node_id * 100) + 2),
									$observation_time,
									$upload_time,
									$analog_value),
						
					"voltage" => array ( (($node_id * 100) + 1),
									$observation_time,
									$upload_time,
									$voltage),
														
					"moisture" => array ( (($node_id * 100) + 0),
									$observation_time,
									$upload_time,
									$moisture)
					);
	
		// prepare SQL insert query
		$sql = "INSERT INTO observations \n"
			."\t\t(sensor_id, observation_time, gateway_time, value_decimal) \n"
			."\tVALUES";
		
		foreach ($dataToInsert as &$row){
		
			
			$sql .=  "\n\t\t(".$row[0]." , "
					.$row[1].", "
					.$row[2]." ,"
					.$row[3]." ),";
		
		}
		$sql = rtrim($sql, ",");
		$sql .= ";";
		

		//printf("value_string: %s\n",$value_string);	
		printf("SQL: %s\n",$sql);	
		
		$qur = pg_query($dbconn, $sql);
		
		if ($qur){
			$json = array("status" => 1, "msg" => "Inserted observation!");
 		} else {
 			$json = array("status" => 0, "msg" => "Error inserting observation!");
 		}
	} else {
		$json = array("status" => 0, "msg" => "invalid key");
	}

} else {
 	$json = array("status" => 0, "msg" => "Request method not accepted");
}


 
/* Output header */
header('Content-type: application/json');
echo json_encode($json);
 
 
 ?>
