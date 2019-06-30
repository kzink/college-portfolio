<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Prime Number Generator with Processes</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Prime Number Generator with Processes</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
<p>This program demonstrates how to communicate between processes using pipes.  More specifically,
it generates prime numbers through the Sieve of Eros algorithm.</p>  
<p>The program spawns 4 processes--the first of which simply sends all odd numbers to the second process.
The second process then attempts to divide the numbers it receives by 3 or 5.  Numbers that
pass are consequently sent to process 3, which attempts to divide the received numbers by 7, 11, 13, and 17.
Lastly, any numbers passing this test are then sent to the final process.  This process tests
each received number against all known prime numbers, and if the number passes, it is added to
the list of primes.</p>
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in C++</li>
<li>Functional programming approach</li>
<li>Demonstrates usage of Fork()</li>
<li>Shows communicate between separate processes via pipes</li>
<li>Utilizes error checking for pipes and forking</li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre> 
kevin@LightningJoe:~/public_html/portfolio/png-pipes$ ./png-pipes 75

Known Prime: 3
Known Prime: 5
Known Prime: 7
Known Prime: 11
Known Prime: 13
Known Prime: 17
Prime: 19
Prime: 23
Prime: 29
Prime: 31
Prime: 37
Prime: 41
Prime: 43
Prime: 47
Prime: 53
Prime: 59
Prime: 61
Prime: 67
Prime: 71
Prime: 73
</pre></td></tr>
</table>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
