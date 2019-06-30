<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Kev-Grep</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Kev-Grep</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
Kev-Grep is a program designed to emulate the standard "Grep" program included with most
linux distributions. Although it is not as advanced as the fully-featured Grep, Kev-Grep
has the ability to match regular expression patterns from standard input or from a file, as well
as invert the search pattern.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in C</li>
<li>Regular Expressions and pattern matching</li>
<li>File input</li>
<li>Command-line switches</li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<table class="codebox" >
<tr><td><pre>
kevin@LightningJoe:~/public_html/portfolio/kev-grep$ ./kevgrep

Syntax is as follows:
        ./kevgrep -icv -f [file] [pattern]

kevin@LightningJoe:~/public_html/portfolio/kev-grep$ ./kevgrep -f test.txt "Sep 13"

kevin    ftpd20306    192.168.0.100    Mon Sep 13 22:55 - 22:56  (00:00)
andrew   ftpd20305    proxy.bpsi.net   Mon Sep 13 22:51 - 22:52  (00:00)
kevin    pts/0        192.168.0.100    Mon Sep 13 21:54 - 01:09  (03:15)
andrew   pts/2        proxy.bpsi.net   Mon Sep 13 21:41 - 22:04  (00:22)
andrew   pts/1        proxy.bpsi.net   Mon Sep 13 21:36 - 22:04  (00:27)
andrew   pts/1        proxy.bpsi.net   Mon Sep 13 19:37 - 19:43  (00:05)
kevin    pts/0        192.168.0.100    Mon Sep 13 18:36 - 21:53  (03:17)
andrew   pts/0        proxy.bpsi.net   Mon Sep 13 09:09 - 09:12  (00:03)
andrew   pts/0        proxy.bpsi.net   Mon Sep 13 07:30 - 07:32  (00:01)
AnnIs#1  pts/0        proxy.bpsi.net   Mon Sep 13 09:09 - 09:12  (00:03)
AnnIs#1  pts/0        proxy.bpsi.net   Mon Sep 13 07:30 - 07:32  (00:01)
</pre></td></tr>
</table>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
