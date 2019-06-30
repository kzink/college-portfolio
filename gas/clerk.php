<?
	class purchase {
		var $Items;
		var $Quantities;	# array for the quantities of each item
		var $Prices;		# array for the prices of each item
		var $Total;
		var $Taxable;
		var $SubTotal;
		var $count;
		var $Pump;
		var $GasAmount;
		var $payment;

		function purchase() {
			$this->SubTotal = 0;
			$this->Total = 0;
			$this->GasAmount = 0;
			$this->count = 0;
			$this->payment = 0;
			$this->Taxable = 0;
		}

		function addGas($pumpnumber, $amount) {
			if($this->GasAmount > 0) {
				$this->SubTotal -= $this->GasAmount;
				$this->Total -= $this->GasAmount;
			}
			$this->Pump = $pumpnumber;
			$this->GasAmount = $amount;
			$this->SubTotal += $amount;
			$this->Total += $amount;
		}
		function addItem($name, $qty) {
			$db = connect();
			$result = mysql_query("select * from items where ItemName='".$name."'");
			if(mysql_num_rows($result) == 0) {
				add_item($name, $qty);
				die();
			}
			else {
				$row = mysql_fetch_assoc($result);
				$this->Items[] = $row['ItemName'];
				$this->Quantities[] = $qty;
				$this->Prices[] = ($row['Price'] * $qty);
				$this->SubTotal += ($row['Price'] * $qty);
				$this->Total += (($row['Taxable'] == 'yes') ? round(($row['Price'] * $qty * 1.07),2) : ($row['Price'] * $qty)); 
				if($row['Taxable'] == 'yes'){
					$this->Taxable += round($row['Price'] * $qty * .07,2); 
				}
				$this->count++;
			}
		}

		function removeItem($name, $qty) {
			for($cnt = 0; $this->Items[$cnt] != $name && $this->Quantities[$cnt] != $qty && $cnt != $this->count; $cnt++) {}
			if($name == $this->Items[$cnt] && $this->Quantities[$cnt] == $qty) {
				$this->SubTotal -= ($this->Prices[$cnt]*$qty);
				$db = connect();
				$result = mysql_query("select Taxable from items where ItemName='".$name."'", $db);
				$row = mysql_fetch_assoc($result);
				if($row['Taxable'] == 'yes') {
					$this->Total -= ($this->Prices[$cnt] * $qty * 1.07);
				}
				else $this->Total -= ($this->Prices[$cnt] * $qty);
				unset($this->Items[$cnt]);
				unset($this->Quantities[$cnt]);
				unset($this->Prices[$cnt]);
			}
		}

		function list_order() {
			echo '<table border="1">';
			for($cnt = 0; $cnt < $this->count; $cnt++) {
				if(isset($this->Items[$cnt])) {
					echo '<tr>';
					echo '<td>'. $this->Items[$cnt].'</td>';
					echo '<td>'. $this->Quantities[$cnt]. '</td>';
					echo '<td>'. $this->Prices[$cnt]. '</td>';
					echo '</tr>';
				}
			}

			if($this->GasAmount <> 0) {
				echo '<tr>';
				echo '<td>GAS (pump '.$this->Pump.')</td>';
				echo '<td>'. $this->GasAmount.'</td>';
				echo '<td>'. $this->GasAmount.'</td>';
				echo '</tr>';
			}

			echo '<tr><td>SubTotal:</td><td></td>';
			echo '<td>'.$this->SubTotal.'</td></tr>';
			echo '<tr><td>Tax:</td><td></td>';
			echo '<td>+ '.($this->Taxable).'</td></tr>';

			if($this->payment <> 0) {
				echo '<tr><td><b>Paid:</b></td><td></td>';
				echo '<td><b>-'.$this->payment.'</b></td><tr>';
			}

			echo '<tr><td><b>Total:</b></td><td></td>';
			echo '<td><b>'.$this->Total.'</b></td></tr>';
			echo '</table>';
		}

		function ringup($item = "", $qty = 0) {
			if(strlen($item) != 0) {
				$this->addItem($item, $qty);
			}
			else if(isset($_POST['itemname'])) {
				$this->addItem($_POST['itemname'], $_POST['quantity']);
			}
			else if(isset($_POST['pumpnumber'])) {
				$this->addGas($_POST['pumpnumber'], $_POST['gasamount']);
			}

			$this->list_order();
			echo '<form name="buyitem" method="post" action="'.$_SERVER['PHP_SELF'].'">';
			echo '<table> <tr>';
			echo '<td><b>Item Name<b></td>';
			echo '<td><b>Quantity<b></td>';
			echo '</tr><tr>';
			echo '<td><input type="text" name="itemname" maxlength="25"></td>';
			echo '<td><input type="text" name="quantity" maxlength="5"></td>';
			echo '</tr><tr>';
			echo '<td><input type="submit" name="Submit" value="Buy Item"></td></tr>';
			echo '</table>';
			echo '</form>';

			$db = connect();
			$result = mysql_query("select InUse, PumpNumber from pumps");
			echo '<table> <tr>';
			echo '<form name="buygas" method="post" action="'.$_SERVER['PHP_SELF'].'">';
			echo '<td><b>From Pump<b></td>';
			echo '<td><b>Amount<b></td>';
			echo '</tr><tr>';
			echo '<td>';
			while($row = mysql_fetch_assoc($result)) {
				echo $row['PumpNumber'].'<input type="radio" name="pumpnumber" value="'.$row['PumpNumber'].'" '.
					(($row['InUse']=="yes") ? 'disabled' : '').'>';
			}
			echo '</td>';
			echo '<td><input type="text" name="gasamount" maxlength="5"></td>';
			echo '</tr><tr>';
			echo '<td><input type="submit" name="GasSubmit" value="Buy Gas"></td>';
			echo '<td><input type="submit" name="PumpOptions" value="Other Gas Options"></td></tr>';
			echo '</table>';

			echo '<input type="submit" name="submit" value="Check Out">';
		}
	}
	
	include("screens.inc");
	include("info.inc");

	$db = connect();

	if(isset($_POST['name']) && isset($_POST['pass'])) {
		$_SESSION['login'] = $_POST['name'];
		$_SESSION['password'] = $_POST['pass'];
	}	
		
	if(verify() <> true) 
	{
		login();
		$_SESSION['order'] = new purchase();
	}
	else if($_SESSION['level'] == 1) {
		if(isset($_POST['PumpOptions']) || isset($_POST['wasthere'])) {
			if(isset($_POST['wasthere'])) {
				if(isset($_POST['Halt'])) {
					$field = "Halted='true'";
				}
				else if(isset($_POST['UnHalt'])) {
					$field = "Halted='false'";
				}
				else {
					$query = "select InUse, AmountAuthorized from pumps where PumpNumber='".$_POST['pumpnum']."'";
					$result = mysql_query($query, $db);
					$row = mysql_fetch_assoc($result);

					/***************************************************/
					/*	In case pump doesn't have money on it or   */
					/*	it isn't being used no point to reset it   */
					/***************************************************/
					if($row['InUse'] <> "no" && $row['AmountAuthorized'] <> 0) {
						$field = "Reset='true'";
					} else {
						$field = "Reset='false'";
					}
				}
				$query = "update pumps set $field where PumpNumber='".$_POST['pumpnum']."'";
				mysql_query($query, $db) or die("Error ". mysql_error());
				unset($_POST['wasthere']);
				echo '<META http-equiv="REFRESH" content="1;url='.$_SERVER['PHP_SELF'].'">';
			} else {
				pumpoptions();
			}
		} else {
			if(isset($_POST['additem'])) {
				$query = "insert into items (ItemName, Price, Taxable) values ('".$_POST['itemname']."','".$_POST['itemprice']."','".$_POST['taxable']."')";
				mysql_query($query, $db) or die("Error: ".mysql_error());
				$_SESSION['order']->ringup($_POST['itemname'], $_POST['buyqty']);
			}
			else if($_POST['submit'] == 'Check Out') {
				payment();
			}
			else if($_POST['submit'] == 'Pay') {
				collectinfo($_POST['amount'], $_POST['paymenttype']);
			}
			else if($_POST['submit'] == 'Change') {
				end_transaction($_POST['amount'], $_POST['paymenttype']);
				echo '<a href="'.$_SERVER['PHP_SELF'].'">Return to Login Screen</a>';
			}
			else {
				$_SESSION['order']->ringup();
			}
		}
	}
	else if($_SESSION['level'] == 2) {
		if($_POST['submit'] == "Add User" || $_POST['submit'] == "Add") {
			if(strlen($_POST['fullname']) <> 0 &&
				strlen($_POST['username']) <> 0 &&
				strlen($_POST['password']) <> 0 &&
				strlen($_POST['email']) <> 0) {

				$db = connect();
				$query = "INSERT INTO logins (LoginName, PassPhrase, UserName, EmailAddress, Level) ";
				$query .= "values ('".$_POST['username']."', '".$_POST['password']."', '".
						$_POST['fullname']."', '". $_POST['email']."', '".$_POST['status']."')";
				mysql_query($query, $db) or die("Error: ".mysql_error());
				echo '<b>'.$_POST['fullname'].' Added.</b><br>';
				echo '<a href="'.$_SERVER['PHP_SELF'].'">Return to Main Menu</a>';
			}
			else {
				echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';
				echo '<table>';
				echo '<tr><td>Full Name:</td>';
				echo '<td><input type="text" maxlength="80" name="fullname" value="'.$_POST['fullname'].'"></td>';
				echo '</tr>';
				echo '<tr><td>User Name:</td>';
				echo '<td><input type="text" maxlength="25" name="username" value="'.$_POST['username'].'"></td>';
				echo '</tr>';
				echo '<tr><td>Password:</td>';
				echo '<td><input type="password" maxlength="80" name="password"></td>';
				echo '</tr>';
				echo '<tr>';
				echo '<td>User Status:</td>';
				echo '<td>Clerk</td><td>Manager</td>';
				echo '</tr> <tr><td></td>';
				echo '<td><input type="radio" name="status" value="1" checked></td>';
				echo '<td><input type="radio" name="status" value="2"></td>';
				echo '</tr> <tr>';
				echo '<td>Email Address:</td>';
				echo '<td><input type="text" maxlength="80" name="email" value="'.$_POST['email'].'"></td>';
				echo '</tr><tr>';
				echo '<td><input type="submit" name="submit" value="Add"></td>';
				echo '</tr></table>';
			}
		}
		else if($_POST['submit'] == "Logout") {
			session_destroy();
			header("Location:".$_SERVER['PHP_SELF']);
		}
		else if($_POST['submit'] == "Refill Gas") {
			$result = mysql_query("select Type, Quantity from gas, items WHERE gas.Type=items.ItemName");
			echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';
			echo '<input type="hidden" name="refill" value="1">';
			while($row = mysql_fetch_row($result)) {
				echo '<input type="submit" name="submit" value="'.$row[0].'"> Current Qty: '.$row[1].'<br>';
			}
			echo '</form>';
			
		}
		else if($_POST['submit'] == "Fix Pump") {
			echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';
			echo 'Pump to fix<br>';
			$query = "select PumpNumber from pumps where Status='notok'";
			$result = mysql_query($query, $db);
			while($row = mysql_fetch_assoc($result)) {
				$counter = 1;
				echo $row['PumpNumber']. ' <input type="radio" name="pumpnumber" value="'.$row['PumpNumber'].'"> ';
			}
			if(isset($counter)) {
				$name = "whichpump";
				$value = "Fix";
			} else {
				echo '<p>There are no pumps broken at the time.</p>';
				$value = "<Back";
				$name = "nonebroken";
			}
			echo '<br><input type="submit" name="'.$name.'" value="'.$value.'"></form>';
		}
		else if(isset($_POST['whichpump'])) {
			mysql_query("update pumps set Status='ok' where PumpNumber='".$_POST['pumpnumber']."'")
				or die("Error: ". mysql_error());
			echo 'Fixed pump '. $_POST['pumpnumber'];
			echo '<br><form method="post" action="'.$_SERVER['PHP_SELF'].'"><input type="submit" name="nonebroken" value="<Back"></form>';
		}
		else if($_POST['submit'] == "Daily report") {
			salesreport();
			echo '<br><form method="post" action="'.$_SERVER['PHP_SELF'].'"><input type="submit" name="nonebroken" value="<Back"></form>';
		}
		else if(isset($_POST['refill'])) {
			mysql_query("UPDATE items SET Quantity='1000' WHERE ItemName='".$_POST['submit']."'",$db)
				or die("Error: ".mysql_error());
			echo 'Filled the tank of '.$_POST['submit'].' gas to 1000 gallons (the max capacity)<br>';
			echo '<a href="'.$_SERVER['PHP_SELF'].'">Return to Main Menu</a>';
		}
		else {
			echo '<form method="post" action="'.$_SERVER['PHP_SELF'].'">';
			echo '<input type="submit" name="submit" value="Add User"><br>';
			echo '<input type="submit" name="submit" value="Refill Gas"><br>';
			echo '<input type="submit" name="submit" value="Fix Pump"><br>';
			echo '<input type="submit" name="submit" value="Daily report"><br>';
			echo '<input type="submit" name="submit" value="Logout">';
			echo '</form>';
		}
	}

	mysql_close($db);
?>
	
