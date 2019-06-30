<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Blackjack Autoplayer</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Blackjack Autoplayer</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
This program was created to interface with my 
<a href="../blackjack-telnet/blackjack.php">Blackjack telnet server</a>.  
Once launched and connected to the server, it proceeds to play blackjack autonomously through 
the use of regular expression pattern matching and packet generation.  The autoplayer's 
AI is derived from a table, which depicts when it should hit, stand, double, etc.  
based on card counting and the money it has available.  The autoplayer can even be designated 
to quit when it has lost or earned a certain amount of cash.  At the end each round, the 
autoplayer reports its findings regarding the various aspects of the game that it tracks.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in C</li>
<li>Utilizes sockets</li>
<li>Functional programming approach</li>
<li>Grasps complicated ruleset of blackjack</li>
<li>Implements regular expression and pattern matching</li>
<li><a href="test.txt">View full sample run</a></li>
</ul>
</div>

<h2>Sample Run</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre>
kevin@LightningJoe:~/public_html/portfolio/blackjack-telnet$ ./bjack 1500
Waiting for data on port TCP 1500
</pre></td></tr>
</table>
<p class="subtext">Server launching</p>

<table class="codebox" >
<tr><td><pre>
kevin@LightningJoe:~/public_html/portfolio/blackjack-auto$ ./bjackc localhost 1500
Insert base bet amount: 10
Insert maximum number of hands to play: 5
Insert dollar amount to stop at: 500
Enter starting amount of cash: 400
Enter number of decks: 1

client: Hand #0 Data
client: ------------------
client: running count: 0
client: Remaining cards: 52
client: True Count is 0.000000 --> Multiplier is 1

. . .

client: Hand #5 Data
client: ------------------
client: running count: -3
client: Remaining cards: 28

client: Program Finished
client: 5 total hands played
Your net cash change was: $20
</pre></td></tr>
</table>
<p class="subtext">Client interface</p>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
