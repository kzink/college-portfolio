<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Airplane</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Airplane</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
This program is an OpenGL graphics application that flies an airplane across the screen.  However, it
does so in a very unique manner: the image is translated (moved), expanded, and rotated in 3D.  Airplane
also contains a simple GUI to manipulate the program to a small degree.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in C++</li>
<li>OO approach</li>
<li>Demonstrates advanced graphical manipulation (in 3D)</li>
<li>Optimized algorithm allows for high framerate</li>
</ul>
</div>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="airplane.jpg" alt="airplane"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
