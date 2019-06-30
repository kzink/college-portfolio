// File: TicTacToeFrame.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 6, Programming Problem 1
// This class contains a JFrame for the game.

import javax.swing.JFrame;
import java.awt.*;

/** This class contains a JFrame to hold the game.
*/
class TicTacToeFrame extends JFrame {
    /** This function creates a JFrame based on the user's screen resolution.
     */
    public TicTacToeFrame() {

        setDefaultCloseOperation(3);

        // this gets the user's screensize
        Toolkit tkit = Toolkit.getDefaultToolkit();
        Dimension dimension = tkit.getScreenSize();
        int height = dimension.height;
        int width = dimension.width;
        
        // setting up the frames name, location, and size.
        setTitle("TicTacToe");
        setLocation(width / 4, height / 4);
        setSize(width / 2, height / 2);
        
        // Adding the panel to the Frame.
        TicTacToePanel Tpanel= new TicTacToePanel();
        Container container = getContentPane();
        container.add(Tpanel);
        //pack(); //packs the window as tightly as possible.
    }
}

