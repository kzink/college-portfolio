// File: TicTacToePanel.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 5, Programming Problem 1
// This class contains the buttons and their operations for the calculator

import java.awt.*;
import javax.swing.*;
import java.awt.event.ActionListener;
import java.awt.event.ActionEvent;
import java.util.Random;

/** This class contains a JPanel for the game.
 */
class TicTacToePanel extends JPanel {

    private String grid[];
    private ImageIcon playerIcon;
    private ImageIcon cpuIcon;
    private JButton buttonArray[];
    private ImageIcon iconX;
    private ImageIcon iconO;
    private JRadioButton xButton; 
    private JRadioButton oButton; 
    private JTextField message;
    private String gameWinner;
    private int counter;
    private boolean gameOver;

    /** Default constructor which contains the "game engine" for tictactoe.  Icons are used as pictures
        for the JButtons.
     */
    public TicTacToePanel() {
        buttonArray = new JButton[9];
        grid = new String[9]; //this holds
        for ( int i = 0; i < grid.length; i++ ) {
            grid[i] = "-1";
        }
        iconX = new ImageIcon( "images/x.jpg", "X" );
        iconO = new ImageIcon( "images/circle.jpg", "O" );
        playerIcon = iconX; 
        cpuIcon = iconO; 
        counter = 0;

        ActionListener listener = new ButtonHandler(); //listener

        // Text window
        setLayout(new BorderLayout( 10, 2 ) );
        message = new JTextField( "" );
        message.setFont( new Font( "Monospaced", 1, 15 ) );
        message.setEditable( false ); //makes the text window non-editable

        // This is a sub Panel within the main JPanel which holds the text window, restart and radio buttons.
        JPanel StatusPanel = new JPanel();
        StatusPanel.setLayout(new BorderLayout( 10, 2 ));

        // Restart button.
        JButton restartButton = new JButton( "Restart" );
        JPanel restartFrame = new JPanel();
        restartFrame.add( restartButton );
        restartButton.addActionListener( listener );

        // Radiobuttons. 
        xButton = new JRadioButton( "X", true );
        xButton.addActionListener( listener );
        oButton = new JRadioButton( "O" );
        oButton.addActionListener( listener );
        JPanel rButtons = new JPanel();
        rButtons.add( xButton );
        rButtons.add( oButton );

        // Radio button so the buttons are exclusive.
        ButtonGroup RadioGroup = new ButtonGroup();
        RadioGroup.add( xButton );
        RadioGroup.add( oButton );
        // Adding the components to the status panel.
        StatusPanel.add( message, BorderLayout.CENTER );
        StatusPanel.add( rButtons, BorderLayout.EAST );
        StatusPanel.add( restartFrame, BorderLayout.WEST );

        add( StatusPanel, BorderLayout.SOUTH ); //sets the status panel as the SOUTH border of the main JPanel.

        JPanel buttons = new JPanel(); // creates a panel of JButtons
        buttons.setLayout( new GridLayout( 3, 3 ) ); //JPanel of JButtons uses a grid layout

        // This creates the buttons and adds them to the listener
        for ( int i = 0; i < buttonArray.length; i++ ) {
            buttonArray[i] = new JButton();
            buttons.add( buttonArray[i] );
            buttonArray[i].setActionCommand( "clicked " + i ); //used for action handling
            buttonArray[i].addActionListener( listener );
        }
        add(buttons, BorderLayout.CENTER); //adds the button panel as the center field in the main JPanel.
    }

    /** ButtonHandler - Handles all JButton and RadioButton actions.
     */
    class ButtonHandler implements ActionListener {
        public void actionPerformed( ActionEvent e ) {
            String action = e.getActionCommand();

            if ( action.startsWith( "clicked " ) ) {
                message.setText( "Game in progress..." );
                xButton.setEnabled( false ); //disables radiobuttons.
                oButton.setEnabled( false );
                int suffix = Integer.parseInt( action.substring( action.length() - 1 ) );  //button #

                if ( grid[suffix].equals( "-1" ) ) {
                    buttonArray[suffix].setIcon( playerIcon ); //sets the icon of the clicked button.
                    grid[suffix] = playerIcon.getDescription(); //records who owns this square in the grid.
                    buttonArray[suffix].setEnabled( false ); //disables the button.
                    counter++;
                }

                winnerTest(); //tests for a winning condition.
                
                // Here, the computer gets to play via random number generator
                if ( !gameOver ) {
                    Random random = new Random();
                    int i = random.nextInt( 9 );

                    while ( !grid[i].equals( "-1" ) ) {
                        i = random.nextInt( 9 );
                    }

                    counter++;
                    buttonArray[i].setIcon( cpuIcon );
                    grid[i] = cpuIcon.getDescription();
                    buttonArray[i].setEnabled( false );
                    winnerTest();
                } 
                
                // This alters the text box according to the winning conditions.
                if ( gameOver ) {
                    if ( gameWinner.equals( "player" ) ) {
                        message.setText( "You Win!  Restart to play again." );
                    } else if ( gameWinner.equals( "computer" ) ) {
                        message.setText( "Computer Wins!  Restart to play again." );
                    } else {
                        message.setText( "Tie Game!  Restart to play again." );
                    }
                }
                // This handles the restart button.
            } else if ( action.equals( "Restart" ) ) {
                message.setText( "Game reset!" );
                xButton.setEnabled( true );
                oButton.setEnabled( true );
                counter = 0;
                // Sets all buttons to have no icon and re-enables them.
                for ( int i = 0; i < buttonArray.length; i++ ) {
                    buttonArray[i].setIcon( null );
                    buttonArray[i].setEnabled( true );
                    grid[i] = "-1";
                }
                // Toggles for the RadioButtons.
            } else if ( action.equals( "X" ) ) {
                playerIcon = iconX;
                cpuIcon = iconO;
            } else if ( action.equals( "O" ) ) {
                playerIcon = iconO;
                cpuIcon = iconX;
            }
        }
    }

    /** This function checks to see if there is a winner.
     */
    public void winnerTest() {
        gameOver = true;
        String winner = "";

        // These conditions check for the 8 possible winning conditions
        if (grid[0].equals(grid[1]) && grid[1].equals(grid[2]) && !grid[0].equals("-1")) {
            winner = grid[0];
        } else if (grid[3].equals(grid[4]) && grid[4].equals(grid[5]) && !grid[3].equals("-1")) {
            winner = grid[3];
        } else if (grid[6].equals(grid[7]) && grid[7].equals(grid[8]) && !grid[6].equals("-1")) {
            winner = grid[6];
        } else if (grid[0].equals(grid[3]) && grid[3].equals(grid[6]) && !grid[0].equals("-1")) {
            winner = grid[0];
        } else if (grid[1].equals(grid[4]) && grid[4].equals(grid[7]) && !grid[1].equals("-1")) {
            winner = grid[1];
        } else if (grid[2].equals(grid[5]) && grid[5].equals(grid[8]) && !grid[2].equals("-1")) {
            winner = grid[2];
        } else if (grid[0].equals(grid[4]) && grid[4].equals(grid[8]) && !grid[0].equals("-1")) {
            winner = grid[0];
        } else if (grid[2].equals(grid[4]) && grid[4].equals(grid[6]) && !grid[2].equals("-1")) {
            winner = grid[2];
        } else if (counter == 9) {
            // in the case of a tie
        } else {
            gameOver = false;
        }

        // If the game is over, gameWinner is set to indicate which player won and all JButtons are disabled.
        if (gameOver) {
            for ( int i = 0; i < buttonArray.length; i++ ) {
                buttonArray[i].setEnabled( false );
            }
            if (winner.equals(playerIcon.getDescription())) {
                gameWinner = "player";
            } else if (winner.equals(cpuIcon.getDescription())) {
                gameWinner = "computer";
            } else {
                gameWinner = "tie";
            }
        }
    }
}
