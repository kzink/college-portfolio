<?

#	include("screens.inc");
	$host = "localhost";
    $db_name = "gas";
    $db_user = "root";
    $db_pass = "vivocal";

	
	function connect()
	{
		$host = "localhost";
		$db_name = "gas";
		$db_user = "root";
		$db_pass = "vivocal";
		$db = mysql_connect($host, $db_user, $db_pass);
		mysql_select_db($db_name, $db) or die ("Error: ". mysql_error());
		return($db);
	}

	function verify()
	{
        $host = "localhost";
        $db_name = "gas";
        $db_user = "root";
        $db_pass = "vivocal";
		session_register("login");
		session_register("password");
		#session_register("level");
		$db = mysql_connect($host, $db_user, $db_pass);
		mysql_select_db($db_name, $db) or die ("Error: ". mysql_error());

		$result = mysql_query("SELECT Level FROM logins where LoginName='".$_SESSION['login'].
					"' AND PassPhrase='".$_SESSION['password'].
					"'", $db); 
		if(mysql_num_rows($result) == 1) {
			$row = mysql_fetch_assoc($result);
			$_SESSION['level'] = $row['Level'];
			$returnval = true;
		}
		else
			$returnval = false;
		return $returnval;
	}

	function alert($type) {
		$db = connect();
		$result = mysql_query("select Quantity from items where ItemName='".$type."'",$db)
			or die("Error: ".mysql_error());
		$row = mysql_fetch_array($result);
		$quantity = $row[0];
		$result = mysql_query("select EmailAddress from logins where EmailAddress!='NULL'",$db); # AND Level='2'",$db);
		while($row = mysql_fetch_array($result)) {
			mail($row[0], "LOW GAS!", 
				"Mr. Manager must order more gas of type: $type".
				" \nat the time of this email being sent out, the Quantity was: $quantity",
				"From: Gas.Pump@gasstation.com"); 
		}
	}
	session_start();
	header("Cache-control: private");
	session_register("login");
	session_register("password");
	session_register("level");
	session_register("order");
	
	session_register("var1");
?>
