<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Scheduler</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Scheduler</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
The Scheduler is a program designed to simulate an annual schedule-keeping program for students 
at the PKI building on the UNO campus--although it would need to be expanded for real usage.  
The program allows the user to select the day, year, time, room, and who they will be meeting 
with through its relatively complex GUI.  The Scheduler also features a few options, such as 
the abilities to clear entries from the schedule and save the entire schedule to a flat-file database
that gets GZIP'd to save some space.
</div>
            
<h2>Code Snippet</h2>
<div class="gentry">
<table class="codebox">
<tr><td><pre>
try {
    output = new PrintStream(new GZIPOutputStream(new FileOutputStream(file)));
} catch (IOException iOE) {
    JOptionPane.showMessageDialog( SchedulerFrame.this, "Error Opening File",
                                   "Error", JOptionPane.ERROR_MESSAGE );
}
</pre></td></tr>
</table>
<p class="subtext">GZIP filestream wrapping - thanks Andrew</p>

</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in Java 1.4.2 Standard Edition</li>
<li>Demonstrates a complex GUI</li>
<li>Utilizes multiple GUI containers (JList, JTree, JTable, etc.)</li>
<li>Features a flat-file database compressed automatically using GZIP</li>
<li><a href="./docs/">Javadocs for Scheduler</a></li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="scheduler.jpg" alt="scheduler!"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
