<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Grapher</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Grapher</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
The grapher is an application that graphs data from a flat-file database. The graph is constructed from inputted
points, and they are represented by filled or empty boxes or circles.  The points are then connected by lines, 
which can be toggled on and off.  Operations can then be done on the graph data, such as adding, subtracting, 
normalizing, and dividing.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in Java 1.4.2 Standard Edition</li>
<li>Demonstrates use of java graphics (Swing)</li>
<li>Menu-driven GUI</li>
<li>File input/output</li>
<li>Utilizing use of check boxes and radio buttons</li>
<li><a href="./docs/">Javadocs for Grapher</a></li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="grapher.jpg" alt="grapher"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
