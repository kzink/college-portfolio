// Name: Kevin Zink
// CSCI: 3830
// Filename: ControllerAgent.java

// This Aglet spawns a GUI interface to allow the user to input a query String and remote seller sites
// This class also spawns the shopping Aglet.  Some of the code in this class is taken from the sample
// Aglets code we were provided.

import com.ibm.aglet.*;
import java.util.Vector;
import java.io.IOException;
import java.awt.*;
import javax.swing.*;
import javax.swing.border.Border;

/** This class creates a child aglet called ShoppingAgent
 * and sends it out to multiple remote sites.  It also spawns a GUI
 * interface used to add remote sites and specify a query string.
 */
public class ControllerAgent extends Aglet {

    private javax.swing.JPanel jContentPane = null;
    private javax.swing.JButton AddSiteButton = null;
    private javax.swing.JPanel ButtonPanel = null;
    private javax.swing.JButton SearchButton = null;
    private javax.swing.JButton CloseButton = null;
    private javax.swing.JButton RemoveSiteButton = null;
    private javax.swing.JPanel SitePanel = null;
    private javax.swing.JPanel QueryPanel = null;
    private javax.swing.JTextField QueryField = null;
    private javax.swing.JPanel AddPanel = null;
    private javax.swing.JTextField SiteField = null;
    private javax.swing.JList SiteList = null;

    public Vector remoteSiteList = new Vector(); //holds the remote sites

    /* The run method
     */
    public void run() {
        createFrame();  // Create a pop-up GUI to handle the query string and remote sites
    }

    private void createFrame() {
        JFrame frame = new JFrame();

        // this gets the user's screensize
        Toolkit tkit = Toolkit.getDefaultToolkit();
        Dimension dimension = tkit.getScreenSize();
        int height = dimension.height;
        int width = dimension.width;

        // this sets up the frames name, location, and size.
        frame.setTitle("Inventory List");
        frame.setLocation(width / 4, height / 4);
        frame.setSize(width / 3, height / 3);

        frame.setContentPane(getJContentPane());

        frame.show();
    }

    /** This method returns query string for the shopping agent's use
      */
    public String getQuery() {
        return QueryField.getText();
    }

    /** This method is called by the ShoppingAgent to get the
     * list of remote sites it should visit. The method
     * initializes a Vector object called remoteSiteList. 
     */
    public Vector getRemoteSiteList(){
        return remoteSiteList;
    }		

    /** handle messages sent from ShoppingAgent.
     */
    public boolean handleMessage(Message msg){
        String msgKind = msg.getKind();
        //print("Message sent from Mobile Aglet = "+(String)msg.getArg());
        return true;
    }

    //------------------------THESE FUNCTIONS BUILD THE GUI------------------------
    /**
     * This method initializes jContentPane
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getJContentPane() {
        if (jContentPane == null) {
            jContentPane = new javax.swing.JPanel();
            jContentPane.setLayout(new java.awt.BorderLayout());
            jContentPane.add(getButtonPanel(), java.awt.BorderLayout.SOUTH);
            jContentPane.add(getSitePanel(), java.awt.BorderLayout.CENTER);
            jContentPane.add(getQueryPanel(), java.awt.BorderLayout.NORTH);
        }
        return jContentPane;
    }
    /**
     * This method initializes RemoveSiteButton
     * 
     * @return javax.swing.JButton
     */
    private javax.swing.JButton getRemoveSiteButton() {
        if(RemoveSiteButton == null) {
            RemoveSiteButton = new javax.swing.JButton();
            RemoveSiteButton.setText("Remove");
            RemoveSiteButton.addActionListener(new java.awt.event.ActionListener() { 
                public void actionPerformed(java.awt.event.ActionEvent e) {    
                    
                    // This removes an entry to the remote site list
                    if (SiteList.getSelectedIndex() != -1) {
                        remoteSiteList.remove(SiteList.getSelectedIndex());
                        SiteList.setListData(remoteSiteList);
                    }
                    
                    // In the case that the list is empty
                    if (remoteSiteList.isEmpty()) {
                        SiteList.setListData(remoteSiteList);
                        SearchButton.setEnabled(false);
                        RemoveSiteButton.setEnabled(false);
                    }
                }
            });

        }
        return RemoveSiteButton;
    }
    /**
     * This method initializes AddSiteButton
     * 
     * @return javax.swing.JButton
     */
    private javax.swing.JButton getAddSiteButton() {
        if(AddSiteButton == null) {
            AddSiteButton = new javax.swing.JButton();
            AddSiteButton.setText("Add");

            // This adds an entry to the remote site list
            AddSiteButton.addActionListener(new java.awt.event.ActionListener() { 
                public void actionPerformed(java.awt.event.ActionEvent e) {    
                    remoteSiteList.add(SiteField.getText());  //this takes the text from the remote site textfield and puts them into the vector
                    SiteList.setListData(remoteSiteList);
                    SiteField.setText("atp://");
                    SearchButton.setEnabled(true);  //enables respective buttons
                    RemoveSiteButton.setEnabled(true);
                }
            });

        }
        return AddSiteButton;
    }
    /**
     * This method initializes ButtonPanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getButtonPanel() {
        if(ButtonPanel == null) {
            ButtonPanel = new javax.swing.JPanel();
            ButtonPanel.add(getSearchButton(), null);
            ButtonPanel.add(getCloseButton(), null);
        }
        return ButtonPanel;
    }
    /**
     * This method initializes CloseButton
     * 
     * @return javax.swing.JButton
     */
    private javax.swing.JButton getCloseButton() {
        if(CloseButton == null) {
            CloseButton = new javax.swing.JButton();
            CloseButton.setText("Close");
            CloseButton.addActionListener(new java.awt.event.ActionListener() { 
                public void actionPerformed(java.awt.event.ActionEvent e) {    
                    ControllerAgent.this.dispose(); //dispose of Aglet if "close" is pressed
                }
            });

        }
        return CloseButton;
    }
    /**
     * This method initializes SearchButton
     * 
     * @return javax.swing.JButton
     */
    private javax.swing.JButton getSearchButton() {
        if(SearchButton == null) {
            SearchButton = new javax.swing.JButton();
            SearchButton.setText("Search");
            SearchButton.setEnabled(false);
            
            // This button creates the shopping agent, which searches all sites in the remote site list
            SearchButton.addActionListener(new java.awt.event.ActionListener() { 
                public void actionPerformed(java.awt.event.ActionEvent e) {    
                    try{
                        AgletProxy childProxy = getAgletContext().createAglet
                        (getCodeBase(), "ShoppingAgent", ControllerAgent.this.getProxy());
                    }
                    catch(IOException ioe){
                        System.out.println("Could not create ShoppingAgent: IO error");
                        ioe.printStackTrace();
                    }
                    catch(AgletException ae){
                        System.out.println("Could not create ShoppingAgent: Aglet creation error");
                        ae.printStackTrace();	    
                    }
                    catch(ClassNotFoundException cnfe){
                        System.out.println("Could not create ShoppingAgent: Class not found");
                        cnfe.printStackTrace();	    
                    }
                    catch(InstantiationException ie){
                        System.out.println("Could not create ShoppingAgent: Instantiation error");
                        ie.printStackTrace();	    
                    }
                }
            });

        }
        return SearchButton;
    }
    /**
     * This method initializes SitePanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getSitePanel() {
        if(SitePanel == null) {
            SitePanel = new javax.swing.JPanel(new BorderLayout());
            SitePanel.add(getAddPanel(), BorderLayout.NORTH);
            SitePanel.add(new JScrollPane(getSiteList()));
            Border outer = javax.swing.BorderFactory.createTitledBorder(null, "Remote Site List",
                    javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION,
                    javax.swing.border.TitledBorder.DEFAULT_POSITION, null, null);
            Border inner = javax.swing.BorderFactory.createEmptyBorder(
                    0 /* top */, 10 /* left */, 10 /* bottom */, 10 /* right */);
            SitePanel.setBorder(javax.swing.BorderFactory.createCompoundBorder(outer, inner));
        }
        return SitePanel;
    }
    /**
     * This method initializes QueryPanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getQueryPanel() {
        if(QueryPanel == null) {
            QueryPanel = new javax.swing.JPanel();
            QueryPanel.add(getQueryField(), null);
            QueryPanel.setBorder(javax.swing.BorderFactory.createTitledBorder(null, "Query", javax.swing.border.TitledBorder.DEFAULT_JUSTIFICATION, javax.swing.border.TitledBorder.DEFAULT_POSITION, new java.awt.Font("Dialog", java.awt.Font.BOLD, 12), java.awt.Color.black));
        }
        return QueryPanel;
    }
    /**
     * This method initializes QueryField
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JTextField getQueryField() {
        if(QueryField == null) {
            QueryField = new javax.swing.JTextField();
            QueryField.setColumns(15);
        }
        return QueryField;
    }
    /**
     * This method initializes AddPanel
     * 
     * @return javax.swing.JPanel
     */
    private javax.swing.JPanel getAddPanel() {
        if(AddPanel == null) {
            AddPanel = new javax.swing.JPanel();
            AddPanel.add(getSiteField());
            AddPanel.add(getAddSiteButton());
            AddPanel.add(getRemoveSiteButton());
        }
        return AddPanel;
    }
    /**
     * This method initializes SiteList
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JList getSiteList() {
        if(SiteList == null) {
            SiteList = new javax.swing.JList(remoteSiteList);
        }
        return SiteList;
    }
    /**
     * This method initializes SiteField
     * 
     * @return javax.swing.JTextField
     */
    private javax.swing.JTextField getSiteField() {
        if(SiteField == null) {
            SiteField = new javax.swing.JTextField();
            SiteField.setColumns(15);
            SiteField.setText("atp://");
        }
        return SiteField;
    }
}
