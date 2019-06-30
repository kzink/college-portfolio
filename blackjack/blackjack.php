<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Blackjack</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Blackjack</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
<p>This is a fully-featured Blackjack program.  It keeps track of the amount 
of money you have and makes use multiple decks (card counting will work).  It addresses 
practically every standard rule of blackjack, such as splits, doubles, and insurance.</p>

<p>Tackling this program was troublesome for me because I did not have a good fundamental understanding
of the game of blackjack when I wrote it. This provided quite a lesson to me: know your
program's requirements before you attempt to write it.</p>
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in C</li>
<li>Functional programming approach</li>
<li>Grasps complicated ruleset of blackjack</li>
<li>Realistically simulates multiple decks</li>
<li>Modularized code via subroutines</li>
</ul>
</div>

<h2>Sample Run</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre>
kevin@LightningJoe:~/public_html/portfolio/blackjack$ ./bjack

Enter starting amount of cash: 500
Enter number of decks: 1
Starting money - 500
Number of decks - 1
Number of total cards - 52

Shuffling shoe...
Starting game...

Place bet ($500 available, 0 to quit): 500
Dealer shows : 7
You have : 7 T = 17
(H)it (S)tand : h
You have : 7 T 2 = 19
(H)it (S)tand: s
Dealer has : 7 6 = 13 - hit
Dealer has : 7 6 Q = 23 - Bust
You win $500

Place bet ($1000 available, 0 to quit): 0
Your net cash change was: $500
</pre></td></tr>
</table>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
