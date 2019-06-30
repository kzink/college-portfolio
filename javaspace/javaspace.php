<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Javaspaces Client</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Javaspaces Client</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
<p>This program dives into the realm of java RMI. Specifically, this program makes use of the 3rd party
Jini Network Technology 1.2.1 library from Sun Microsystems.  Jini is a library built upon RMI, and it
contains Javaspaces technology. More information regarding Jini can be found
<a href="http://wwws.sun.com/software/jini/">here.</a></p>

<p>The overall feature Javaspaces offers is the ability to pass autonomous objects from one host to another
using TCP/IP.  The object being sent can be stored, retrieved, etc. from the hosting machine.  This particular
program allows the user to connect to a Jini server and store or retrieve a simple object, called an entry, 
that contains location, time, and names fields.</p>  

<p>The entry fields in the Javaspace Client GUI are used to 
create an entry object when the "write" button is pressed, and the entry fields are used as search strings
when the "take" button is pressed.  If an entry is found on the Jini server that matches the text in each field,
it is removed from the server and its contents is used to populate the client GUI fields (in the case that one
of the fields is empty).</p>
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in Java 1.4.2 Standard Edition with Jini 1.2.1</li>
<li>Implements Java RMI</li>
<li>Transfers entire objects over TCP/IP</li>
<li>GUI created using Eclipse</li>
<li><a href="./docs/">Javadocs for Javaspace Client</a></li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="javaspace.jpg" alt="javaspace!"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
