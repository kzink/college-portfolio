<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Token Ring Aglet</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Token Ring Aglet</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
<p>This challenging project was carried out by my roomate, Noah Anderson, and myself as our final project
for our advanced java class.  
It was a very involved project involving the use of IBM's Aglet's Java library to simulate a virtual 
token ring network.</p>

<p>Creation of this program could be considered an exercise in Extreme Programming, 
as we were quite literally sitting side by side while working on this project--for hours at a time.  
Our hard work paid off as we received a high-flying A+ as a final grade.</p>  

<p>It is difficulty to show a simple overview of this program because it was very massive and complex.  
However, the basic features of the program allowed for the token ring network (which consisted of chain 
of Tahiti servers) to send messages to each Tahiti server and detect when a Tahiti server failed 
(and consequently repair the network chain automatically).  Check out the documentation link for a full 
overview of this program's design.</p>  

<p>One other nuance of this program that was challenging was testing: 
our GUI had a number of fields which had to be filled out (in multiple windows) every time the program 
was loaded, so we automated the program set up through the use of metadata.  When the program loaded up, 
it populated the text fields with data from the metadata file and was, with much brevity, ready to be tested.  
I feel this automated testing was paramount to the project's success.</p>
</div>

<h2>My Role in the Team</h2>
<div class="gentry">
Since our team consisted of my roommate and myself, we quite literally shared the workload half and half.
I created the framework that drives the user interface, and Noah worked heavily on the networking aspect of this project.
However, there was not really a clear-cut division of work, as that would greatly limit our over all
understanding of the project--a clear violation of one the underpinning principles of extreme programming.
</div>

<h2><a href="./Zink_&amp;_Anderson-assignment5.doc">Program Documentation</a></h2>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in Java 1.4.2 Standard Edition and IBM's Aglets library</li>
<li>Implements Java RMI</li>
<li>Simulates token ring network</li>
<li>Highly involved group project</li>
<li><a href="./docs/">Javadocs for Token Ring Aglet</a></li>
</ul>
</div>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="tokenring.jpg" alt="tokenring"/>
<p class="subtext">A simple diagram that depicts the inner workings of this program.</p>
<img class="picbox" src="tokengui.jpg" alt="tokengui"/>
<p class="subtext">The user GUI.</p>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
