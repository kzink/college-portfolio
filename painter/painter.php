<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Painter</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Painter</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
This is an OpenGL program designed to a be a simple drawing application.  Lines are drawn by using
the mouse.  The color and the width of the line can be adjusted.  In addition,
lines can be selected, moved, and deleted after their original creation, adding a flavor 
of customization.  The drawing panel can also be saved to a flat-file database, which makes clever
use of space-saving techniques.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in C++/OpenGL</li>
<li>Decoupled OO approach</li>
<li>Effective approach to new/delete paradigm</li>
<li>Pointer Manipulation</li>
<li>Flat-file database with space-saving algorithm</li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="painter.jpg" alt="airplane"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
