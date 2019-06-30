<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Account Gateway</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Account Gateway</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
The account gateway utilizes the <i>last</i> command-line tool included in unix distributions, which
reports the number of logins for each user.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in Perl as a CGI script</li>
<li>Regular expressions and pattern matching</li>
<li>Interfaces with shell program</li>
</ul></div>

<h2><a href="./program15a.cgi">Try out the Account Gateway</a></h2>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="accountsummary.jpg" alt="accountsummary"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
