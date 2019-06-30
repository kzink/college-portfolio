// File: Calcframe.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 6, Programming Problem 1
// This class contains the class for a simple calculator

import javax.swing.JFrame;
import java.awt.*;

/** This class contains a JFrame to hold the calculator.
* @author Kevin Zink
*/
class Calcframe extends JFrame {
    /** This function creates a JFrame based on the user's screen resolution.
     */
    public Calcframe() {

        setDefaultCloseOperation(3);

        // this gets the user's screensize
        Toolkit tkit = Toolkit.getDefaultToolkit();
        Dimension dimension = tkit.getScreenSize();
        int height = dimension.height;
        int width = dimension.width;
        
        // setting up the frames name and location.
        setTitle("Calculator");
        setLocation(width / 4, height / 4);
        
        // Adding the panel to the Frame.
        Calcpanel cpanel= new Calcpanel();
        Container container = getContentPane();
        container.add(cpanel);
        pack(); //packs the window as tightly as possible.
    }
}

