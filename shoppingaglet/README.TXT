-------------------------------
Author: Kevin Zink
Program: Programming Problem #4
-------------------------------

----------------------
Shopping Aglet Package
----------------------

---------
FILENAMES
---------

ControllerAgent.java
InterfaceAgent.java
ShoppingAgent.java
inv1.txt
inv2.txt
inv3.txt

-----
SETUP
-----

Installation of this program is simple: put all of the files into the "public" folder of your
Aglet's installation directory.

To compile the program, do the standard "javac *.java" or run compile.bat

To start the program, start one Tahiti Server and on it create a ControllerAgent.  
This server will act as our HOST or starting point.  
Once created, this Aglet will pop up a GUI window.  For information on how to use this, look to the
"RUNNING THE SHOPPING AGENT" section of this readme.

Now, start another Tahiti server, with a specified port, and on it create an InterfaceAgent.
Upon creation of the InterfaceAgent Aglet, you will be specified to choose a file.  Select a .txt
file that contains a catalog of items to inventory.  

---------------------------------
	!!!!IMPORTANT!!!!
---------------------------------

The catalog file must contain 2 values per line, a name and a value, separated by a tab delimeter.
The name will indicate the name of the item and the value will indicate the price of that item
in American cents.

An example Entry:
Shoes	400  <---- This would represent an item named "Shoes" whose value is $4.00

There are 3 inventory files included with this package for sampling (inv1.txt inv2.txt inv3.txt). 

Repeat this process of making Tahiti servers hosting InterfaceAgents as many times as you want.
These servers will act as a remote seller sites.

--------------------------
RUNNING THE SHOPPING AGENT
--------------------------

The HOST tahiti server's GUI is very straightforward.  There is a query box, which you will use
to input your query string, and a remote sites list box, which allows you to add and remove
seller sites.

You will want to add all of the seller sites you have created into the remote sites list by 
using the add button.  Simply type in the site's address and hit "Add."  

Note: The seller sites are specified through the standard "atp://address:port" syntax.

Once your remote server list is populated, type a search string into the query box and hit 
the "Search" button.  The Aglet will then search through all sites in the remote list and
attempt to find the cheapest entry corresponding to the search query.  If the item was found on a remote site, its lowest price will appear in the console window of the HOST tahiti server, 
along with the site that was selling the item.

Note: you can search as many times as you would like without restarting any of the Tahiti servers.

-------------
DOCUMENTATION
-------------
Documentation can be found within the code, itself, as well as within the included javadocs
that are located in the "Docs" folder.