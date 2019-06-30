<?
	include "info.inc";
	session_register('pickedup');
	session_register('var2');
	$db = connect();

	if(isset($_POST['BREAK'])) {
		echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
		mysql_query("update pumps set Status='notok' where PumpNumber='2'") or die("Error: ". mysql_error());
	} else {
	$result = mysql_query("select AmountAuthorized, Halted, Reset, Status from pumps where PumpNumber='2'", $db) 
		or die("Error: ".mysql_error());
	$row = mysql_fetch_assoc($result);
	if($row['Halted'] == "false" && $row['Reset'] == "false" && $row['Status'] == "ok") {
		$Authorized = $row['AmountAuthorized'];
		if(isset($_SESSION['var2']) && $Authorized <> 0 && !isset($_POST['gallons'])) {
			unset($_SESSION['pickedup']);
			if(isset($_POST['submit'])) {
				$_SESSION['var1'] = $_POST['submit'];
			}
			$result = mysql_query("select Price from items where ItemName='".$_SESSION['var1']."'");
			$result1 = mysql_query("select AmountAuthorized from pumps where PumpNumber='2'");
			$price = mysql_fetch_array($result);
			$authorized = mysql_fetch_array($result1);
			$gallons = $authorized[0] / $price[0];
			echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';
			echo '<input type="hidden" name="type" value="'.$_POST['submit'].'">';
			echo '<input type="hidden" name="gallons" value="'.$gallons.'">';
			echo 'You are authorized to pump '.$row['AmountAuthorized'].' or '.round($gallons,2).' gallons of this gas, to continue, press Pump<br>';
			echo '<input type="submit" value="Pump">';
			echo '</form>';
		}
		else if((!isset($_POST['submit']) || $_POST['submit'] <> 'Pump') && !isset($_SESSION['var1']) && ($Authorized == 0 || $_SESSION['pickedup'] == 1 || isset($_POST['pickedup']))) {
			if(isset($_POST['putback'])) {
				echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
				unset($_SESSION['pickedup']);
				echo 'Thank you.';
			}
			else if(isset($_POST['pickedup']) || isset($_SESSION['pickedup'])) {
				if($Authorized == 0) {
					echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
					echo '<b>Please prepay inside before pumping!</b><br>';
					echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';
					echo '<input type="submit" name="putback" value="Replace Nozzle">';
					echo '</form>';
					$_SESSION['pickedup'] = 1;
					die();
				}
				else {
					echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';	
					echo '<table>';
					$result = mysql_query("select * from items, gas where gas.Type=items.ItemName",$db);
					while($row = mysql_fetch_assoc($result)) {
						echo '<tr><td><input type="submit" name="submit" value="'.$row['ItemName'].'"> $'.$row['Price'].' per gallon</td></tr>';
					}
					echo '</table>';
					$_SESSION['var2'] = 1;
				}
			}
			else {
				echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';
				echo '<input type="submit" name="pickedup" value="Lift Nozzle"><br><br>';
				echo '<input type="submit" name="BREAK" value="!">';
				echo '</form>';
			}
		}
		else {
			unset($_SESSION['var2']);
			session_register('total_gallons');
			session_register('gallons');
			session_register('type');
		
			if(!isset($_SESSION['total_gallons'])) {
				$_SESSION['total_gallons'] = $_POST['gallons'];
				$_SESSION['gallons'] = $_POST['gallons'];
				$_SESSION['type'] = $_POST['type'];
			}

			$result = mysql_query("select Reset from pumps where PumpNumber='2'") 
				or die("Error: ".mysql_error());
			$halted = mysql_fetch_assoc($result);

			$amount_to_pump_this_second = ($_SESSION['gallons'] > 1) ? 1 : $_SESSION['gallons'];
			$_SESSION['gallons'] -= $amount_to_pump_this_second;
			$result = mysql_query("select Quantity from items where ItemName='".$_SESSION['type']."'")
				or die("Error: ".mysql_error());
			$row = mysql_fetch_array($result);
			mysql_query("update items set Quantity='".($row[0] - $amount_to_pump_this_second)."' where ItemName='".$_SESSION['type']."'", $db) 
				or die("Error: ".mysql_error());
			mysql_query("update pumps set AmountAuthorized='".$_SESSION['gallons']."' WHERE PumpNumber='2'", $db) or die("Error: ".mysql_error());
			if(($row[0] > 250) && ($row[0] - $_SESSION['gallons'] < 250)) {
				alert($_SESSION['type']);
			}
			echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
			if($_SESSION['gallons'] < .01) {
				mysql_query("update pumps set InUse='no' where PumpNumber='2'", $db) or die("Error: ".mysql_error());
				echo round(($_SESSION['total_gallons'] - $_SESSION['gallons']),2).' gallons of gas pumped<br>';
				echo 'Thank You.<br>';
				session_destroy();
				die();
			}
			echo round($_SESSION['total_gallons'] - $_SESSION['gallons'],2).' gallons of gas pumped<br>';
				
		}

	}
	else if($row['Status'] == "notok") {
		echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
		echo '<b>PUMP 2 BROKEN</b>';
		die();
	}
	else if($row['Halted'] == "true") {
		echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
		echo '<b>PUMP 2 DISABLED</b>';
		die();
	}
	else {
		echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
		echo '<b>PUMP 2 RESET</b>';
		session_destroy();
		mysql_query("update pumps set InUse='no', AmountAuthorized='0', Reset='false'", $db)
			or die("Error: ". mysql_error());
	} // RESET!
	}
?>
