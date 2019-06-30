// File: JavaspaceClient.java
// Name: Kevin Zink
// Program #3 - JavaSpaces

//This file contains a client program to interface with the JavaSpace software package.  
//A few portions of this program are taken from the Dietel book's JavaSpace sample programs.

import javax.swing.JPanel;
import javax.swing.JFrame;
import net.jini.core.entry.UnusableEntryException;
import net.jini.core.transaction.TransactionException;
import net.jini.core.lease.Lease;
import net.jini.space.JavaSpace; 
import java.rmi.RemoteException;
import javax.swing.JOptionPane;

/*
 * Created on Mar 10, 2004
 *
 */

/**
 * @author Kevin Zink
 *
 */
public class JavaspaceClient extends JPanel {

    private javax.swing.JTextField locationText = null;
    private javax.swing.JPanel topPanel = null;
    private javax.swing.JTextField firstNameText = null;
    private javax.swing.JLabel middleNameLabel = null;
    private javax.swing.JTextField lastNameText = null;
    private javax.swing.JLabel lastNameLabel = null;
    private javax.swing.JTextField middleNameText = null;
    private javax.swing.JLabel firstNameLabel = null;
    private javax.swing.JPanel bottomPanel = null;
    private javax.swing.JButton writeButton = null;
    private javax.swing.JButton takeButton = null;
    private javax.swing.JTextField timeText = null;
    private javax.swing.JPanel lastNamePanel = null;
    private javax.swing.JPanel middlePanel = null;
    private javax.swing.JPanel timePanel = null;
    private javax.swing.JPanel middleNamePanel = null;
    private javax.swing.JPanel firstNamePanel = null;
    private MyEntry myEntry = new MyEntry();
    private JavaSpace space;
	private javax.swing.JPanel locationPanel = null;
	private javax.swing.JPanel buttonPanel = null;

    /**
     * This is the main function - it constructs the program's JFrame.
     */
    public static void main(String[] args) {
        JFrame frame = new JFrame();
        frame.getContentPane().add(new JavaspaceClient());
        frame.setDefaultCloseOperation(3);
        frame.setTitle("Javaspace Client");
        frame.pack();
        frame.show();
    }
    /**
     * This is the default constructor
     */
    public JavaspaceClient() {
        super();
        initialize();
    }
    /**
     * This method initializes this
     * 
     * @return void
     */
    private void initialize() {
        java.awt.GridLayout layGridLayout8 = new java.awt.GridLayout();
        layGridLayout8.setRows(3);
        layGridLayout8.setColumns(1);
        this.setLayout(layGridLayout8);
        this.add(getTopPanel(), null);
        this.add(getMiddlePanel(), null);
        this.add(getBottomPanel(), null);
        this.setSize(262, 311);
          
        // set JavaSpace address upon launch
        String jiniURL = "jini://localhost";
        JavaSpaceFinder findtool = new JavaSpaceFinder( jiniURL );
        space = findtool.getJavaSpace();
    }

    /**
     * This method deposits an Entry into the javaSpace
     * 
     * @return void
     */
    public void writeEntry() {         

        Integer timevalue;

        // if the time text field is empty, set the Entry field to 0
        if (timeText.getText().equals("")) {
            timevalue = new Integer(0);
        } else {
            timevalue = Integer.valueOf(timeText.getText());
        }

        // bounds checking on the time field - launches error box if in violation
        if ( Integer.valueOf(timeText.getText()).intValue() > 23 || Integer.valueOf(timeText.getText()).intValue() < 0 ) {
            JOptionPane.showMessageDialog( JavaspaceClient.this, "Time Field must contain and Integer between the values of 0 and 23",
                    "Error", JOptionPane.ERROR_MESSAGE );
            return;
        }

        // setting the fields in the Entry class
        myEntry = new MyEntry(
                locationText.getText(), 
                firstNameText.getText(), 
                middleNameText.getText(), 
                lastNameText.getText(), 
                timevalue);

        // Enter object into JavaSpace         
        try {   
            space.write( myEntry, null, Lease.FOREVER);
            
            //DEBUGGING
            /*
            System.out.println("Wrote entry");   
            System.out.println("location: " + myEntry.location);
            System.out.println("1st name: " + myEntry.personName1);
            System.out.println("2nd name: " + myEntry.personName2);
            System.out.println("3rd name: " + myEntry.personName3);
            System.out.println("time: " + myEntry.time);
            */
        }        

        // handle exception network failure        
        catch ( RemoteException exception ) {            
            exception.printStackTrace();        
        }         
        // handle exception invalid transaction
        catch ( TransactionException exception ) {            
            exception.printStackTrace();         
        }
    } 

    /**
     * This method takes an Entry from the javaSpace
     * 
     * @return void
     */
    public void takeEntry() { 
        
        // set the Integer time to null if it's an empty string
        Integer timevalue;
        if (timeText.getText().equals("")) {
            timevalue = null;
        } else {
            timevalue = Integer.valueOf(timeText.getText());
        }

        // sets all of the Entry object's fields
        myEntry = new MyEntry(
                locationText.getText(), 
                firstNameText.getText(), 
                middleNameText.getText(), 
                lastNameText.getText(), 
                timevalue);

        // set all fields to null if they're empty Strings
        if (myEntry.personName1.equals("")) {
            myEntry.personName1 = null;
        }
        if (myEntry.personName2.equals("")) {
            myEntry.personName2 = null;
        }
        if (myEntry.personName3.equals("")) {
            myEntry.personName3 = null;
        }
        if (myEntry.location.equals("")) {
            myEntry.location = null;
        }

        try {
            // read Entry from JavaSpace               
            myEntry = (MyEntry) space.take( myEntry, null, JavaSpace.NO_WAIT );  

            // populate text fields if returned Entry is not null
            if (myEntry != null) {
                //System.out.println("Found Entry");
                locationText.setText(myEntry.location); 
                firstNameText.setText(myEntry.personName1);
                middleNameText.setText(myEntry.personName2); 
                lastNameText.setText(myEntry.personName3);
                timeText.setText(myEntry.time.toString());
            } else {
                JOptionPane.showMessageDialog( JavaspaceClient.this, "No Entry Found for Current Query",
                        "Error", JOptionPane.ERROR_MESSAGE );
            }
        }
        // handle exception network failure	
        catch ( RemoteException exception ) {           
            exception.printStackTrace();       
        }
        // handle exception invalid transaction
        catch ( TransactionException exception ) {
            exception.printStackTrace();
        }        
        // handle exception unusable Entry     
        catch ( UnusableEntryException exception ) {     
            exception.printStackTrace();
        }        
        // handle exception interrupting        
        catch ( InterruptedException exception ) {       
            exception.printStackTrace();       
        }  
    } // end method takeEntry

    /**
     * This method initializes locationText
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JTextField getLocationText() {
        if(locationText == null) {
            locationText = new javax.swing.JTextField("");
            locationText.setColumns(5);
        }
        return locationText;
    }
    /**
     * This method initializes topPanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getTopPanel() {
        if(topPanel == null) {
            topPanel = new javax.swing.JPanel();
            java.awt.GridLayout layGridLayout1 = new java.awt.GridLayout();
            layGridLayout1.setRows(2);
            layGridLayout1.setColumns(1);
            topPanel.setLayout(layGridLayout1);
            topPanel.add(getLocationPanel(), null);
            topPanel.add(getTimePanel(), null);
        }
        return topPanel;
    }
    /**
     * This method initializes firstNameText
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JTextField getFirstNameText() {
        if(firstNameText == null) {
            firstNameText = new javax.swing.JTextField();
            firstNameText.setColumns(10);
        }
        return firstNameText;
    }
    /**
     * This method initializes middleNameLabel
     * 
     * @return javax.swing.JLabel
     */
    private javax.swing.JLabel getMiddleNameLabel() {
        if(middleNameLabel == null) {
            middleNameLabel = new javax.swing.JLabel();
            middleNameLabel.setText("2nd");
        }
        return middleNameLabel;
    }
    /**
     * This method initializes lastNameText
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JTextField getLastNameText() {
        if(lastNameText == null) {
            lastNameText = new javax.swing.JTextField();
            lastNameText.setColumns(10);
        }
        return lastNameText;
    }
    /**
     * This method initializes lastNameLabel
     * 
     * @return javax.swing.JLabel
     */
    private javax.swing.JLabel getLastNameLabel() {
        if(lastNameLabel == null) {
            lastNameLabel = new javax.swing.JLabel();
            lastNameLabel.setText("3rd");
        }
        return lastNameLabel;
    }
    /**
     * This method initializes middleNameText
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JTextField getMiddleNameText() {
        if(middleNameText == null) {
            middleNameText = new javax.swing.JTextField();
            middleNameText.setColumns(10);
        }
        return middleNameText;
    }
    /**
     * This method initializes firstNameLabel
     * 
     * @return javax.swing.JLabel
     */
    private javax.swing.JLabel getFirstNameLabel() {
        if(firstNameLabel == null) {
            firstNameLabel = new javax.swing.JLabel();
            firstNameLabel.setText("1st");
        }
        return firstNameLabel;
    }
    /**
     * This method initializes bottomPanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getBottomPanel() {
        if(bottomPanel == null) {
            bottomPanel = new javax.swing.JPanel();
            bottomPanel.setLayout(new java.awt.BorderLayout());
            bottomPanel.add(getButtonPanel(), java.awt.BorderLayout.SOUTH);
        }
        return bottomPanel;
    }
    /**
     * This method initializes writeButton
     * 
     * @return javax.swing.JButton
     */
    private javax.swing.JButton getWriteButton() {
        if(writeButton == null) {
            writeButton = new javax.swing.JButton();
            writeButton.setText("Write");
            writeButton.addActionListener(new java.awt.event.ActionListener() { 
                public void actionPerformed(java.awt.event.ActionEvent e) {    
                //invoke writeEntry()
                    writeEntry();
                }
            });
        }
        return writeButton;
    }
    /**
     * This method initializes takeButton
     * 
     * @return javax.swing.JButton
     */
    private javax.swing.JButton getTakeButton() {
        if(takeButton == null) {
            takeButton = new javax.swing.JButton();
            takeButton.setText("Take");
            takeButton.addActionListener(new java.awt.event.ActionListener() { 
                public void actionPerformed(java.awt.event.ActionEvent e) {    
                //invoke takeEntry()
                    takeEntry();
                }
            });
        }
        return takeButton;
    }
    /**
     * This method initializes timeText
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JTextField getTimeText() {
        if(timeText == null) {
            timeText = new javax.swing.JTextField();
            timeText.setColumns(5);
            timeText.setHorizontalAlignment(javax.swing.JTextField.LEADING);
        }
        return timeText;
    }
    /**
     * This method initializes lastNamePanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getLastNamePanel() {
        if(lastNamePanel == null) {
            lastNamePanel = new javax.swing.JPanel();
            lastNamePanel.setLayout(new java.awt.FlowLayout());
            lastNamePanel.add(getLastNameLabel(), null);
            lastNamePanel.add(getLastNameText(), null);
        }
        return lastNamePanel;
    }
    /**
     * This method initializes middlePanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getMiddlePanel() {
        if(middlePanel == null) {
            middlePanel = new javax.swing.JPanel();
            java.awt.GridLayout layGridLayout5 = new java.awt.GridLayout();
            layGridLayout5.setRows(3);
            layGridLayout5.setColumns(1);
            middlePanel.setLayout(layGridLayout5);
            middlePanel.add(getFirstNamePanel(), null);
            middlePanel.add(getMiddleNamePanel(), null);
            middlePanel.add(getLastNamePanel(), null);
            middlePanel.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.RAISED), "Names", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Dialog", java.awt.Font.BOLD, 12), java.awt.Color.black));
        }
        return middlePanel;
    }
    /**
     * This method initializes timePanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getTimePanel() {
        if(timePanel == null) {
            timePanel = new javax.swing.JPanel();
            timePanel.setLayout(new java.awt.FlowLayout());
            timePanel.add(getTimeText(), null);
            timePanel.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.RAISED), "Time", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, null, null));
        }
        return timePanel;
    }
    /**
     * This method initializes middleNamePanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getMiddleNamePanel() {
        if(middleNamePanel == null) {
            middleNamePanel = new javax.swing.JPanel();
            middleNamePanel.add(getMiddleNameLabel(), null);
            middleNamePanel.add(getMiddleNameText(), null);
        }
        return middleNamePanel;
    }
    /**
     * This method initializes firstNamePanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getFirstNamePanel() {
        if(firstNamePanel == null) {
            firstNamePanel = new javax.swing.JPanel();
            firstNamePanel.add(getFirstNameLabel(), null);
            firstNamePanel.add(getFirstNameText(), null);
        }
        return firstNamePanel;
    }
	/**
	 * This method initializes locationPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getLocationPanel() {
		if(locationPanel == null) {
			locationPanel = new javax.swing.JPanel();
			locationPanel.add(getLocationText(), null);
			locationPanel.setBorder(javax.swing.BorderFactory.createTitledBorder(javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.RAISED), "Location", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, null, null));
		}
		return locationPanel;
	}
	/**
	 * This method initializes buttonPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getButtonPanel() {
		if(buttonPanel == null) {
			buttonPanel = new javax.swing.JPanel();
			buttonPanel.add(getWriteButton(), null);
			buttonPanel.add(getTakeButton(), null);
		}
		return buttonPanel;
	}
}
