<?php


$db_name = "esp";  
 $mysql_user = "root";  
 $mysql_pass = "xxxxxxxx";  
 $server_name = "localhost:xxxx";

//ABC

$con = mysqli_connect($server_name,$mysql_user,$mysql_pass,$db_name); 


/*
$id ="1"; 
$temp ="29.90";
$hum ="89.88";
*/

$mac = ($_GET['mac']); 
$pm25 = ($_GET['pm25']);
$co2 = ($_GET['co2']);
$temp = ($_GET['t']);
$hum = ($_GET['h']);

date_default_timezone_set('Asia/Taipei');  #設定時區
$nowdt = date("Y-m-d H:i:s");               #儲存時間
 
//UPDATE `esp2`
$sql_check_mac = "SELECT COUNT(1) FROM `esp2` WHERE `mac` = '$mac'";
$check_result = mysqli_query($con, $sql_check_mac) or die("Error in Selecting " . mysqli_error($con));
$check_result = mysqli_fetch_row($check_result);
$check_result = $check_result[0];
	
if($check_result == 0){
	$sql_esp2 = "INSERT INTO `esp2` ( `time`, `temp`, `hum`, `mac`, `pm25`, `co2`) VALUES('$nowdt','$temp','$hum','$mac','$pm25','$co2')";
}else{
	if($check_result == 1){
		$sql_esp2 = "UPDATE `esp2` SET `time` = '$nowdt',`temp`='$temp',`hum`='$hum',`pm25`='$pm25',`co2`='$co2' WHERE `mac`='$mac'"; 
	}else{
		echo "Please check mac address.";
	}
}
	$result_esp2 = mysqli_query($con, $sql_esp2) or die("Error in Selecting " . mysqli_error($con));

//INSERT `esp1`
$sql= " INSERT INTO `esp1` ( `time`, `temp`, `hum`, `mac`, `pm25`, `co2`) 
   VALUES('$nowdt ','$temp','$hum','$mac','$pm25','$co2');"; 

$result = mysqli_query($con, $sql) or die("Error in Selecting " . mysqli_error($con));

 
if($result_esp2){
 echo "esp 2 Insert Success....";
}else{
echo "esp2 Data insert error.....".mysqli_error($con);
}
if($result){
 echo "esp 1 Insert Success....";
}else{
echo "esp1  Data insert error.....".mysqli_error($con);
}
mysqli_close($con);



//$id = ($_GET['id']); 
//$temp = ($_GET['t']);
//$hum = ($_GET['h']);
 
 /*
 
if ($id == '1') {
	$total = mysql_result(mysql_query("SELECT count(*) FROM `esp1`"),0);
	mysql_query("INSERT INTO `esp1` (time,temp,hum) values ('".$nowdt."','".$temp."','".$hum."') ");
}
if ($id == '2') {
	$total = mysql_result(mysql_query("SELECT count(*) FROM `esp2`"),0);
	mysql_query("INSERT INTO `esp2` (time,temp,hum) values ('".$nowdt."','".$temp."','".$hum."') ");
}
if ($id == '3') {
	$total = mysql_result(mysql_query("SELECT count(*) FROM `esp3`"),0);
	mysql_query("INSERT INTO `esp3` (temp,hum) values ('".$nowdt."','".$temp."','".$hum."') ");
}

*/
//echo 'Ок';
?>
