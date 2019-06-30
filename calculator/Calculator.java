// File: Calculator.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 6, Programming Problem 1
// This class contains the class for a simple calculator

/** This class contains a fully functional calculator.
* @author Kevin Zink
*/
public class Calculator {
    /** Default constructor.
     * 
     */
    public Calculator() {
    }

    /** Main function.
     * This creates the frame for the calculator.
     */
    public static void main(String args[]) {
        Calcframe cframe= new Calcframe();
        cframe.show();
    }
}
