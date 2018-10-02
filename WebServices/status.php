<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html xmlns="http://www.w3.org/1999/xhtml">

<head>
	<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
	<meta http-equiv="refresh" content="5" />
	<title>Status</title>
	<style type="text/css" media="screen">
		table, th, td {
    			border: 1px solid black;
    			border-collapse: collapse;
		}
		th, td {
   			 padding: 7px;
		}
	</style>
</head>

<body>



<?php

include_once('config.php');


$query = 
	"SELECT o.observation_id, o.sensor_id, o.gateway_time, o.insert_time, value_decimal
 		FROM 
			observations o
 		WHERE
			o.sensor_id >= 100 AND
			o.sensor_id < 500 AND
			o.observation_id in 
				(SELECT MAX(o2.observation_id) FROM observations o2 GROUP BY o2.sensor_id)
  		ORDER BY 
			o.sensor_id ASC;";

$result=pg_query($dbconn, $query);

if  (!$result) {
	echo "query did not execute";
}
if (pg_num_rows($result) == 0) {
   echo "0 records";
} else {
	echo "<table>\n";
	//echo "<table style=\"width:700\".>\n";
	echo "\t<tr>".
		"<th>"."sensor_id"."</th>".
		"<th>"."gateway_time"."</th>".
		"<th>"."insert_time"."</th>".
		"<th>"."value_decimal"."</th>".
		"</tr>\n";	
    	while ($row = pg_fetch_array($result)) {
		echo "\t<tr>".
			//"<td>".$row["observation_id"]."</td>".
			"<td>".$row["sensor_id"]."</td>".
			"<td>".$row["gateway_time"]."</td>".
			"<td>".$row["insert_time"]."</td>".
			"<td>".$row["value_decimal"]."</td>".
			"</tr>\n";	
      		
    	} 
	echo "</table>\n";
  } 
 ?>

<div id="validators">
<br />
<br />
<hr />
<a href="http://validator.w3.org/check?uri=referer"><img
          src="http://www.w3.org/Icons/valid-xhtml10-blue"
          alt="Valid XHTML 1.0!" height="31" width="88" /></a>

<a href="http://jigsaw.w3.org/css-validator/check/referer">
    <img style="border:0;width:88px;height:31px"
        src="http://jigsaw.w3.org/css-validator/images/vcss-blue"
        alt="Valid CSS!" />
</a>
</div>

</body>
</html>
