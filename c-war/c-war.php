<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>War Card Game</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>War Card Game</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
This program simulates the playing of the card game War through random number generation.  The
game plays itself (no interaction from the user), and the results of the game are printed to the
screen.  The deck of cards is modeled by using a self-made queue constainer struct that has all
of the basic operations of the STL queue container, including the ability to grow dynamically.
At runtime, the "deck" is a queue is filled with the numbers 0 through 52 in random order.  Then,
the contents of the queue are "dealt" to the two player queues.  Every card from the player
queues are compared against each other to simulate the playing of War.  Each number corresponds to a specific
card through the use of two handy #define statements and card index arrays.
</div>

<h2>Code Snippet</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre>
static const char *card[] =       //holds names of cards
{
    "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
    "Nine", "Ten", "Jack", "Queen", "King", "Ace"
};
static const char *suit[] =       //holds suits of cards
{
    "Diamonds", "Clubs", "Hearts", "Spades"
};
</pre></td></tr>
</table>
<p class="subtext">Card index arrays</p>

<table class="codebox" >
<tr><td><pre>
#define CARD(x) (card[ (x) % 13 ])
#define SUIT(x) (suit[ (x) / 13 ])
</pre></td></tr>
</table>
<p class="subtext">#define statements</p>
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in C</li>
<li>Mixture of functional and OO design</li>
<li>Usage of #define statements</li>
<li>Demonstrates nicely formatted output</li>
<li>Features self-made queue container struct</li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre>
<?php
include("sample.txt");
?>
</pre></td></tr>
</table>
</div>

<?php
include("../footer.php");
?>

</body>
</html>