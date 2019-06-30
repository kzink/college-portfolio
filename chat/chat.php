<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

<html>

<head>
<title>Chat Client/Server</title>
<?php
include("../header.php");
?>
</head>

<body>
<h1>Chat Client/Server</h1>

<?php
include("../navbar.php");
?>

<h2>Program Description</h2>
<div class="gentry">
This package contains two individual programs--a server and a client.  The server, which
utilizes datagram packets, maintains a list of subscribers (machines running the chat client).
This list is updated every time a client connects to the server, and when a client sends a message
to the server, the message is then sent to every address in the subscriber list.  The GUI for the
client is very simple, containing a connection dialog and a simple onscreen message display.
</div>

<h2>Features</h2>
<div class="gentry"><ul>
<li>Written in Java build 1.4.2 Standard Edition</li>
<li>Implements standard Java datagram packets</li>
<li>Demonstrates a simple client/server application</li>
<li>Each server can host multiple clients</li>
<li><a href="./docs/">Javadocs for Chat</a></li>
</ul></div>

<h2>Sample Run</h2>
<div class="gentry">
<img class="picbox" src="chat.jpg" alt="chat.jpg"/>
</div>

<?php
include("../footer.php");
?>

</body>
</html>
