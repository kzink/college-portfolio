<?
	session_start();
	session_register("login");
	session_register("password");
	session_register("level");
	session_register("order");

	function login()
	{
		echo '<form name="form1" method="post" action="'.$_SERVER['PHP_SELF'].'">';
		echo 'Login Name: <input name="name" type="text" size="16" maxlength="25" value='.$_POST['name'].'><br>';
		echo 'Password: <input name="pass" type="password" size="16" maxlength="25"><br>';
		echo '<input type="submit">';
		echo '</form>';
	}

	function add_item($name="", $qty="")
	{
		echo '<form name="form2" method="post" action="'.$_SERVER['PHP_SELF'].'">';
		echo '<input type="hidden" name="additem" value="additem">';
		echo 'Item Name <input type="text" name="itemname" value="'.$name.'"><br>';
		echo 'Item Price <input type="text" name="itemprice"><br>';
		echo 'Quantity To Buy <input type="text" name="buyqty" value="'.$qty.'"><br>';
		echo 'Is this item taxable? ';
		echo 'yes <input type="radio" name="taxable" value="yes" checked>';
		echo 'no <input type="radio" name="taxable" value="no"><br>';
		echo '<input type="submit" name="Submit" value="submit">';
		echo '</form>';
	}

	function payment()
	{
		$_SESSION['order']->list_order();
		echo '<form name="form3" method="post" action="'.$_SERVER['PHP_SELF'].'">';
		echo 'Payment Type: Cash';
		echo '<input type="radio" name="paymenttype" value="cash" checked>';
		echo '/ Credit';
		echo '<input type="radio" name="paymenttype" value="credit"><br>';
		echo '<input type="submit" name="submit" value="Pay">';
		echo '</form>';
	}

	function collectinfo($amount, $paymenttype)
	{
		$_SESSION['order']->list_order();
		echo '<form name="form4" method="post" action="'.$_SERVER['PHP_SELF'].'">';
		if($paymenttype =="cash") {
			echo 'Amount Paid: ';
			echo '<input type="text" name="amount" maxlength=5><br>';
			echo '<input type="hidden" name="paymenttype" value="cash">';
			echo '<input type="submit" name="submit" value="Change">';
		} else {
			echo 'Credit Card Number: ';
			echo '<input type="text" name="amount" maxlength=16><br>';
			echo '<input type="hidden" name="amount" value="'.$_SESSION['order']->Total.'">';
			echo '<input type="hidden" name="paymenttype" value="credit">';
			echo '<input type="submit" name="submit" value="Change">';
		}
		echo '</form>';
	}

	function end_transaction($amount, $paymenttype)
	{
		$_SESSION['order']->payment += $amount;
		$difference = $_SESSION['order']->payment - $_SESSION['order']->Total;
		if($difference < 0) {
			payment();
			die();
		}
		else {
			$_SESSION['order']->list_order();
			if($paymenttype == "credit") {
				echo "CARD APPROVED<br>";
			}
			if($difference > 0) {
				echo "<b>Change: ".$difference."</b><br>";
			}
			$db = connect();
			if($_SESSION['order']->GasAmount <> 0) {
				$query = "UPDATE pumps SET InUse='yes', AmountAuthorized='".$_SESSION['order']->GasAmount."' WHERE PumpNumber='".$_SESSION['order']->Pump."'"; 
				mysql_query($query, $db) or die("Error: ".mysql_error());
			}
			$query = "INSERT INTO purchases (LoginName, Date, StoreNumber, PaymentType, SubTotal, Total) values (".
				"'".$_SESSION['login']."',".
				"'".date("Y/m/d")."',".
				"'1',".
				"'".$paymenttype."',".
				"'".$_SESSION['order']->SubTotal."',".
				"'".$_SESSION['order']->Total."')";
			mysql_query($query, $db) or die("Error: " . mysql_error());
			session_destroy();
			mysql_close($db);
		}
	}		

	function salesreport() {
		$db = connect();
		$totalsales = 0;
		$totalsubsales = 0;
		$query = "SELECT * from purchases Where Date=CURDATE() ORDER BY Date";
		$result = mysql_query($query, $db) or die("Error: ". mysql_error());
		echo '<table border="1"><tr>';
		echo '<td><b>Date</b></td>'.
			'<td><b>Clerk Name</b></td>'.
			'<td><b>Store Number</b></td>'.
			'<td><b>Subtotal</b></td>'.
			'<td><b>Total</b></td>'.
			'<td><b>Payment Type</b></td></tr>';
		while($row = mysql_fetch_assoc($result)) {
			echo '<tr>';
			echo '<td>'.$row['Date'].'</td>';
			echo '<td>'.$row['LoginName'].'</td>';
			echo '<td>'.$row['StoreNumber'].'</td>';
			echo '<td>'.$row['SubTotal'].'</td>';
			echo '<td>'.$row['Total'].'</td>';
			echo '<td>'.$row['PaymentType'].'</td>';
			echo '</tr>';
			$totalsales += $row['Total'];
			$totalsubsales += $row['SubTotal'];
		}
		echo '<tr><td><b>TOTAL</b></td><td></td><td></td><td><b>'.$totalsubsales.'</b></td><td><b>'.$totalsales.'</b></td><td></td></tr>';
		echo '</table>';
	}
	function pumpoptions()
	{
		echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'"><table>';
		echo '<tr><td><b>Pump Number</b></td></tr>';
		echo '<tr><td>1 <input type="radio" name="pumpnum" value="1" checked>';
		echo ' 2 <input type="radio" name="pumpnum" value="2">';
		echo ' 3 <input type="radio" name="pumpnum" value="3">';
		echo '<input type="hidden" name="wasthere" value="wasthere">';
		echo '</td></tr>';
		echo '<tr><td><input type="submit" name="PumpOptions" value="Reset"></td>';
		echo '<td><input type="submit" name="UnHalt" value="UnHalt"></td>';
		echo '<td><input type="submit" name="Halt" value="Halt"></td></tr></table></form>';
	}
?>
