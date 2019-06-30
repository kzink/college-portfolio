// File: Calcpanel.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 5, Programming Problem 1
// This class contains the buttons and their operations for the calculator

import java.awt.*;
import javax.swing.JPanel;
import javax.swing.JButton;
import javax.swing.JTextField;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;

/** This class contains a JPanel for the Calculator
* @author Kevin Zink
*/
class Calcpanel extends JPanel {

    public double lhs;
    public double memory;
    public boolean erase;
    public String operation;
    private JTextField result;
    
    /** Default constructor
      * Sets up the buttons and text field in a border Layout
     */
    public Calcpanel() {
        operation = "";
        erase = true;
        memory = 0;

        setLayout(new BorderLayout(10, 2));
        result = new JTextField("0.0", 30);
        result.setFont(new Font("Monospaced", 1, 15));
        result.setHorizontalAlignment(JTextField.RIGHT);
        result.setEditable(false); //makes the text window non-editable
        add(result, BorderLayout.NORTH); //sets the text field as the NORTH border

        ActionListener listener = new ButtonHandler(); //listener
        
        JPanel buttons = new JPanel(); // creates a panel of JButtons
        
        buttons.setLayout(new GridLayout(5, 5)); //JPanel of JButtons uses a grid layout
        makeButton("MRC", buttons, listener);
        makeButton("M+", buttons, listener);
        makeButton("M-", buttons, listener);
        makeButton("sqrt", buttons, listener);
        makeButton("x^y", buttons, listener);
        
        makeButton("7", buttons, listener);
        makeButton("8", buttons, listener);
        makeButton("9", buttons, listener);
        makeButton("+", buttons, listener);
        makeButton("/", buttons, listener);
        
        makeButton("4", buttons, listener);
        makeButton("5", buttons, listener);
        makeButton("6", buttons, listener);
        makeButton("-", buttons, listener);
        makeButton(".", buttons, listener);
    
        makeButton("1", buttons, listener);
        makeButton("2", buttons, listener);
        makeButton("3", buttons, listener);
        makeButton("*", buttons, listener);
        makeButton("+/-", buttons, listener);
        
        makeButton("0", buttons, listener);
        makeButton("CE", buttons, listener);
        makeButton("CA", buttons, listener);
        makeButton("=", buttons, listener);
        add(buttons, BorderLayout.CENTER); //adds the JPanel as the center field in the main JPanel's layout
    }

    /** Makebutton - Makes buttons.
      * This function makes a JButton and adds it to the Action Listener
     */
    void makeButton(String name, JPanel panel, ActionListener listener) {
        JButton button = new JButton( name );
        panel.add( button );
        button.addActionListener( listener );
    }

    /** ButtonHandler - Handles all button actions.
      * This function is the heart of the calculator.  It detects which button has been pressed and does the
      * appropriate action.
     */
    class ButtonHandler implements ActionListener {
        public void actionPerformed(ActionEvent e) {
            String action = e.getActionCommand();
            if (action.equals("1") || action.equals("2") || action.equals("3") || action.equals("4") ||
                    action.equals("5") || action.equals("6") || action.equals("7") || action.equals("8") || 
                    action.equals("9") || action.equals("0") ) {
                if (erase) {
                    result.setText("");
                    erase = false;
                }
                result.setText(result.getText() + action);
            } else if (action.equals(".")) {
                if (erase) {
                    result.setText("");
                    erase = false;
                }
                if (result.getText().indexOf('.') == -1) {
                    result.setText(result.getText() + ".");
                }
            } else if (action.equals("CE")) {
                result.setText("0.0");
                erase = true;
            } else if (action.equals("+") || action.equals("-") || action.equals("*") || action.equals("/") || 
                    action.equals("x^y")) {
                lhs = Double.valueOf(result.getText()).doubleValue();
                operation = action;
                erase = true;
            } else if (action.equals("=")) {
                double rhs = Double.valueOf(result.getText()).doubleValue();
                erase = true;
                if (operation.equals("+")) {
                    result.setText(String.valueOf(lhs + rhs));
                } else if (operation.equals("-")) {
                    result.setText(String.valueOf(lhs - rhs));
                } else if (operation.equals("*")) {
                    result.setText(String.valueOf(lhs * rhs));
                } else if (operation.equals("/")) {
                    if (rhs == 0) {
                        result.setText("Error Div by 0");
                    } else {
                        result.setText(String.valueOf(lhs / rhs));
                    }
                } else if (operation.equals("x^y")) {
                    result.setText(String.valueOf(Math.pow(lhs, rhs)));
                }
            } else if (action.equals("CA")) {
                result.setText("0.0");
                lhs = 0;
                operation = "";
                erase = true;
            } else if (action.equals("sqrt")) {
                double rhs = Double.valueOf(result.getText()).doubleValue();
                result.setText(String.valueOf(Math.sqrt(rhs)));
                erase = true;
            } else if (action.equals("M+")) {
                double rhs = Double.valueOf(result.getText()).doubleValue();
                memory += rhs;
                erase = true;
            } else if (action.equals("M-")) {
                double rhs = Double.valueOf(result.getText()).doubleValue();
                memory -= rhs;
                erase = true;
            } else if (action.equals("MRC")) {
                result.setText(String.valueOf(memory));
                memory = 0;
                erase = true;
            } else if (action.equals("+/-")) {
                double rhs = Double.valueOf(result.getText()).doubleValue();
                result.setText(String.valueOf(-rhs));
                erase = true;
            }
        }
    }
}
