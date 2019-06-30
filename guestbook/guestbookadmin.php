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
I wrote this program to interface with my <a href="./guestbook.php">MySQL/PHP Guestbook</a>.  It is
written in HTML embedded ruby, or eRuby.  It interfaces with the same MySQL database that the guestbook
sign-up program communicates with.  Its only feature is the ability to delete entries from the guestbook.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in eRuby and CSS</li>
<li>Interfaces with MySQL database</li>
</ul></div>

<h2><a href="/~kevin/guestbook/guestadmin.rhtml">Try out the eRuby Guestbook Admin</a></h2>

<?php
include("../footer.php");
?>

</body>
</html>
