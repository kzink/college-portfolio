<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>MySQL/PHP Guestbook</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>MySQL/PHP Guestbook</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
I wrote this little application to learn more about MySQL, PHP, and CSS.  It is fairly simple,
but it is also fully-functional.  The guestbook entries are stored in a MySQL database and it has 
been secured so that HTML code cannot be entered into the guestbook fields (which could mess up the 
page layout).  Check out my <a href="./guestbookadmin.php">eRuby Guestbook Admin program</a>, too.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in PHP and CSS</li>
<li>Interfaces with MySQL database</li>
<li>Demonstrates web-based security</li>
</ul></div>

<h2><a href="/~kevin/guestbook/sign.php">Try out the PHP/MySQL Guestbook</a></h2>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="guestbook.jpg" alt="guestbook"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
