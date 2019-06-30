<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Apache Logger</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Apache Logger</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
Apache Logger is a log-parsing program written in Perl.  Its purpose is to parse apache webserver
logs and generate a report.  The report depicts such things as a list of the hosts which have
visited the webserver, which URLs were visited, the visitor's browser type, referers, and the 
total number of bytes served.  The logger is capable of parsing multiple log files at once to
generate a single,  massive report.
</div>

<h2>Code Snippet</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre>
my ($ipaddy, $date, $time, $url, $statuscode, $bytes, $referer, $useragent) =
($line =~ /([^ ]*) - - \[([^:]*):(\d*):[^"]*"[^ ]* ([^ ]*)[^"]*" (.*) (.*) "([^"]*)" "([^"]*)"/);
</pre></td></tr>
</table>
<p class="subtext">A... complicated... Regular Expression search pattern from this program</p>
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in Perl</li>
<li>Intense regular expressions and pattern matching</li>
<li>Multiple file inputs/outputs</li>
<li>Generates a <a href="./Calvino.analysis">nicely formatted report</a> 
from a <a href="./tiny.log">complex log file</a></li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre>
Processed 200 lines from the following files:
tiny.log 

==============================================================================
HOSTNAMES
==============================================================================

Hits   %-age    Resource
----   -----    --------
13     6.50     137.48.130.113
1      0.50     137.48.130.14
19     9.50     137.48.130.18
12     6.00     66.37.237.128
13     6.50     adsl-18-106-138.sdf.bellsouth.net
1      0.50     crawler10.googlebot.com
1      0.50     crawler11.googlebot.com
1      0.50     crawler14.googlebot.com
1      0.50     crawlers.looksmart.com
3      1.50     dialup-65.58.122.249.Dial1.Omaha1.Level3.net
2      1.00     figaro.verity.com
9      4.50     ip68-106-212-208.om.om.cox.net
3      1.50     ip68-13-162-19.om.om.cox.net
13     6.50     ip68-13-163-55.om.om.cox.net
35     17.50    ip68-13-66-249.om.om.cox.net
26     13.00    ip68-13-78-171.om.om.cox.net
9      4.50     ip68-224-161-39.om.om.cox.net
6      3.00     ip68-225-175-203.om.om.cox.net
1      0.50     j3111.inktomisearch.com
1      0.50     j3154.inktomisearch.com
1      0.50     j3155.inktomisearch.com
28     14.00    pc021807.dsc.unomaha.edu
1      0.50     si1000.inktomisearch.com
-----
200 entries displayed
</pre></td></tr>
</table>
<p class="subtext">A small portion of a report</p>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
