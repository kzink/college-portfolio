// File: MyEntry.java
// Name: Kevin Zink
// Program #3

// This class represents an Entry object that goes into the JavaSpace

/*
 * Created on Mar 11, 2004
 *
 */

import net.jini.core.entry.Entry;

/**
 * @author Kevin Zink
 *
 */
public class MyEntry implements Entry {
    
	public String location; //stores location
	//public String[] personName = new String[3]; //stores names
    public String personName1;
    public String personName2;
    public String personName3;
	public Integer time; //stores time

    /**
     * Default Constructor
     * 
     */
	public MyEntry() {
		location = "";
		personName1 = ""; 
		personName2 = ""; 
		personName3 = "";
		time = new Integer(0);
	}

    /**
     * This constructor takes the arguments to build the Entry
     * 
     */
	public MyEntry(String loc, String p1, String p2, String p3, Integer t) {
        location = loc;
        personName1 = p1;
        personName2 = p2;
        personName3 = p3;
        time = t;
    }
}
