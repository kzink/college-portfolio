// File: SchedulerFrame.java

// Kevin R. Zink, ID 3541
// CSCI 3830, Spring 2003
// Programming Problem 1
// This class contains the class a Scheduling program with file input/output.

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.tree.*;
import javax.swing.table.*;
import javax.swing.JTable.*;
import java.util.Vector;
import java.io.*;
import java.util.zip.*;

/** This class creates a JFrame and JPanels for the Scheduler. 
 */
public class SchedulerFrame extends JFrame {

    private String[] classList = new String[300];
    private JButton buttonArray[] = new JButton[31];
    private String selMonth;
    private JTextField StatusBar;
    private JTree peopleTree;
    private DefaultMutableTreeNode root;
    private JTable timeTable;
    private JList roomNumbers;
    private TimeTableDataModel tableModel = new TimeTableDataModel();
    private SchedulerContainer schedule = new SchedulerContainer();
    private int month = 0;
    private int day = 1;
    private Choice ChoiceBox;
    private File file = new File("Data.txt.gz"); //file used to store the time table
    private BufferedReader in;
    private PrintStream output;

    /** Constructor Class -- Modifies the JFrame so that it is the appropriate size and initiates construction of the JPanels. 
     */
    public SchedulerFrame() {
        try {
            setDefaultCloseOperation(3);

            // this gets the user's screensize
            Toolkit tkit = Toolkit.getDefaultToolkit();
            Dimension dimension = tkit.getScreenSize();
            int height = dimension.height;
            int width = dimension.width;

            // setting up the frames name, location, and size.
            setTitle("Scheduler");
            setLocation(width / 4, height / 4);
            setSize(width / 2, height * 3 / 5);

            // Make panels!
            MakePanels();
        } catch(Exception e) {
            e.printStackTrace();
        }
    }

    /** This function handles construction of all JPanels and objects.
     */
    private void MakePanels() throws Exception {

        ActionListener alistener = new ButtonHandler(); //action listener for JButtons
        ItemListener ilistener = new ChoiceHandler(); //item listener for choice box

        // Panels
        JPanel NorthPanel = new JPanel();
        JPanel CenterPanel = new JPanel();
        JPanel SouthPanel = new JPanel();
        JPanel TreePanel = new JPanel();
        JPanel MenuButtonPanel = new JPanel();
        JPanel StatusPanel = new JPanel();

        // Layout Settings for each panel
        NorthPanel.setLayout(new BorderLayout());
        CenterPanel.setLayout(new GridLayout(5,7));
        SouthPanel.setLayout(new BorderLayout());
        TreePanel.setLayout(new BorderLayout());
        MenuButtonPanel.setLayout(new GridLayout());
        StatusPanel.setLayout(new BorderLayout());
        this.getContentPane().setLayout(new BorderLayout());


        // --------------------------- Top Panel----------------------------
        // Contains 2 JLabels and a Choice box.
        JLabel year = new JLabel("Year: 2004");
        JLabel month = new JLabel("Month:");
        ChoiceBox = new Choice();
        ChoiceBox.addItemListener( ilistener );
        ChoiceBox.add("January");
        ChoiceBox.add("February"); 
        ChoiceBox.add("March"); 
        ChoiceBox.add("April");
        ChoiceBox.add("May");
        ChoiceBox.add("June");
        ChoiceBox.add("July");
        ChoiceBox.add("August");
        ChoiceBox.add("September"); 
        ChoiceBox.add("October"); 
        ChoiceBox.add("November"); 
        ChoiceBox.add("December");
        NorthPanel.add(year, BorderLayout.WEST);

        // This panel is used to hold the JLabel and Choice box to deal with spacing issues.
        JPanel TopPanel = new JPanel();
        TopPanel.setLayout(new FlowLayout());

        TopPanel.add(month);
        TopPanel.add(ChoiceBox);
        NorthPanel.add(TopPanel, BorderLayout.CENTER);
        this.getContentPane().add(NorthPanel, BorderLayout.NORTH);


        // ------------------------- Center Panel--------------------------
        // Contains 31 JButton for every day of the month (30th and 31st JButtons are disabled for the appropriate months).
        this.getContentPane().add(new JScrollPane(CenterPanel), BorderLayout.CENTER);
        for (int i = 0; i < 31; ++i) {
            buttonArray[i] = new JButton(Integer.toString(i + 1));
            CenterPanel.add( buttonArray[i] );
            buttonArray[i].setActionCommand( "clicked " + (i + 1) );
            buttonArray[i].addActionListener( alistener );
        }

        // ------------------------- Bottom Panel--------------------------
        // The bottom panel consists of 3 sub-panels.
        this.getContentPane().add(SouthPanel, BorderLayout.SOUTH);

        // Room Number JList 
        SouthPanel.add(TreePanel, BorderLayout.NORTH);
        for (int i = 0; i < 300; ++i) {
            classList[i] = "PKI " + (i + 100);
        }
        roomNumbers = new JList(classList);
        roomNumbers.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        roomNumbers.setVisibleRowCount(11);
        TreePanel.add(new JScrollPane(roomNumbers), BorderLayout.WEST);

        // Tree of PKI people
        createNodes();
        DefaultTreeModel peopleTreeModel = new DefaultTreeModel(root);
        peopleTree = new JTree(peopleTreeModel);
        peopleTree.getSelectionModel().setSelectionMode(TreeSelectionModel.DISCONTIGUOUS_TREE_SELECTION);
        peopleTree.setVisibleRowCount(11);
        TreePanel.add(new JScrollPane(peopleTree), BorderLayout.CENTER);

        // JTable for Meeting Times
        timeTable = new JTable(tableModel) {
            public Dimension getPreferredScrollableViewportSize() {
                // This is used to restrict the number of rows that are visible to 11.
                return new Dimension(getPreferredSize().width, getRowHeight() * 11);
            }
        };

        // Loads file if it exists
        if (file.exists())
            LoadFile(); 

        timeTable.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
        TreePanel.add(new JScrollPane(timeTable), BorderLayout.EAST); 

        // Menu JButtons Panel
        JButton SaveInTableButton = new JButton("Save in Table");
        JButton ClearButton = new JButton("Clear List/Tree");
        JButton SaveInFileButton = new JButton("Save on File");
        JButton ExitButton = new JButton("Exit");
        JButton ClearRowButton = new JButton("Clear sel. Row");

        // Listener Registration for JButtons
        SaveInTableButton.addActionListener( alistener );
        ClearButton.addActionListener( alistener );
        SaveInFileButton.addActionListener( alistener );
        ExitButton.addActionListener( alistener );
        ClearRowButton.addActionListener( alistener );

        SouthPanel.add(MenuButtonPanel,  BorderLayout.CENTER);
        MenuButtonPanel.add(SaveInTableButton);
        MenuButtonPanel.add(SaveInFileButton);
        MenuButtonPanel.add(ClearRowButton);
        MenuButtonPanel.add(ClearButton);
        MenuButtonPanel.add(ExitButton);

        // Status Bar Panel
        SouthPanel.add(StatusPanel,  BorderLayout.SOUTH);
        StatusBar = new JTextField();
        StatusBar.setFont( new Font( "Monospaced", 1, 13 ) );
        StatusBar.setEditable( false ); //makes the text window non-editable
        StatusBar.setText("Status Bar");
        StatusPanel.add(StatusBar, BorderLayout.CENTER);

        buttonArray[0].doClick(); //Simulates clicking on JButton for Day 1 to start the program off.
    }

    /** This function is called when the program is started.  
      It reads in data from the file "Data.txt.gz" and inserts it into the table.
     */
    private void LoadFile() {
        try {
            // This program implements java's built-in gzip compression for the filestream to save some space.
            BufferedReader in = new BufferedReader(new InputStreamReader(new GZIPInputStream(new FileInputStream(file))));
            String line;
            int colCount = 0;
            int rowCount = 0;
            int dayCount = 1;

            // Reads in from the file one line at a time and injects it into the table.
            while ((line = in.readLine()) != null && dayCount < 373) {
                schedule.setDayDirect(line, dayCount, rowCount, colCount); //insertion into the Container.
                tableModel.setValueAt(line, rowCount, colCount + 1); //insertion into the JTable.
                if (colCount == 3) {
                    rowCount++; 
                    colCount = 0;
                }
                else
                    colCount++;

                if (rowCount == 24) {
                    rowCount = 0;
                    dayCount++;
                }
            }
            in.close(); //close Filestream
        } catch (IOException x) {
            x.printStackTrace();
        }
    }

    /** Action listener class which implements all actions done by this program's JButtons.
     */
    class ButtonHandler implements ActionListener {
        public void actionPerformed( ActionEvent e ) {
            String action = e.getActionCommand();

            // All "Day" JButton Actions 
            if (action.startsWith( "clicked ")) {
                day = Integer.parseInt(action.substring(8));  //parses out button #

                // updates the status bar
                StatusBar.setText("Editing: " + ChoiceBox.getSelectedItem() + " " + day + ", 2004");

                // Inject data from the selected day from SchedulerContainer into the table.
                for (int i = 0; i < 24; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        String dataEntry = schedule.getDay(month, day, i, j); //grabs String from container
                        tableModel.setValueAt(dataEntry, i, j + 1); //inserts String into table
                    }
                }

                // Exit Action
            } else if (action.equals("Exit")) {
                System.exit(1);

                // Save in table Action
            } else if (action.equals("Save in Table")) {

                // Error conditions -- prompt dialog boxes
                if (timeTable.getSelectedRow() == -1) {
                    JOptionPane.showMessageDialog( SchedulerFrame.this, "Please select a row from the table.", 
                            "Error", JOptionPane.ERROR_MESSAGE );
                    return;
                } else if (roomNumbers.isSelectionEmpty()) {
                    JOptionPane.showMessageDialog( SchedulerFrame.this, "Please select a room from the rooms list.", 
                            "Error", JOptionPane.ERROR_MESSAGE );
                    return;
                } else if (peopleTree.isSelectionEmpty()) {
                    JOptionPane.showMessageDialog( SchedulerFrame.this, "Please select instructor(s) from the people tree.", 
                            "Error", JOptionPane.ERROR_MESSAGE );
                    return;

                    // If there are no errors, save data from the JList and JTree into the selected row in the table.
                } else {
                    //updates table with room # (from JList)
                    tableModel.setValueAt(roomNumbers.getSelectedValue(), timeTable.getSelectedRow(), 1);                     
                    //updates container with room #
                    schedule.setDay((String)roomNumbers.getSelectedValue(), month, day, timeTable.getSelectedRow(), 0); 

                    String instructors[] = getSelectedValues(); //gets selected items from the JTree

                    // Inserts JTree items into the table and container
                    for (int i = 0; i < 3; ++i) {
                        schedule.setDay(instructors[i], month, day, timeTable.getSelectedRow(), i + 1);
                        tableModel.setValueAt(instructors[i], timeTable.getSelectedRow(), i + 2);
                    }
                }

                // Save to File Action
            } else if (action.equals("Save on File")) {

                // creating output stream
                try {
                    output = new PrintStream(new GZIPOutputStream(new FileOutputStream(file)));
                } catch (IOException iOE) {
                    JOptionPane.showMessageDialog( SchedulerFrame.this, "Error Opening File",
                            "Error", JOptionPane.ERROR_MESSAGE );
                }

                // Writes to the file one table entry per line
                for (int i = 1; i < 373; ++i) {
                    for (int j = 0; j < 24; ++j) {
                        for (int k = 0; k < 4; ++k) {
                            output.println(schedule.getDayDirect(i, j, k));
                        }
                    }
                }

                // Close Filehandler
                output.close();

                // Clear list/tree Action 
            } else if (action.equals("Clear List/Tree")) {
                roomNumbers.clearSelection();
                peopleTree.clearSelection();

                // Clear selected Row Action
            } else if (action.equals("Clear sel. Row")) {
                if (timeTable.getSelectedRow() == -1) {
                    JOptionPane.showMessageDialog( SchedulerFrame.this, "Please select a row from the table.", 
                            "Error", JOptionPane.ERROR_MESSAGE );
                    return;
                }

                // To clear the row, empty Strings are inserted into each column of the selected row,
                // updating the table and container.
                for (int i = 0; i < 4; ++i) {
                    schedule.setDay("", month, day, timeTable.getSelectedRow(), i);
                    tableModel.setValueAt("", timeTable.getSelectedRow(), i + 1);
                }
            }
        }
    }

    /** Item listener class which listens for changes in the Choice drop-down menu.
     */
    class ChoiceHandler implements ItemListener {
        public void itemStateChanged( ItemEvent e ) {
            String itemName = e.getItem().toString();

            // This determines whether the 30th and 31st days of each month will be enabled.
            if (itemName.equals("January") || itemName.equals("March") || itemName.equals("May") || itemName.equals("July") || 
                    itemName.equals("August") || itemName.equals("October") || itemName.equals("December")) {
                buttonArray[30].setVisible(true);
                buttonArray[29].setVisible(true);
            } else if (itemName.equals("February")) {
                buttonArray[30].setVisible(false);
                buttonArray[29].setVisible(false);
            } else if (itemName.equals("April") || itemName.equals("June") || 
                    itemName.equals("September") || itemName.equals("November")) {
                buttonArray[30].setVisible(false);
                buttonArray[29].setVisible(true);
            }

            month = ChoiceBox.getSelectedIndex();
            day = 1;
            buttonArray[0].doClick(); // Simulates clicking on "day 1" button to avoid getting to the 30th or 31st day on certain months.
        }
    }

    /** This function parses out the selected entries from the JTree with a maximum of 3 entries.  
      This is taken directly from the Table sample program from the course website.
     */
    private String[] getSelectedValues(){
        int index = 0;
        // create the string array in which the strings are returned
        // maximum of 3 persons can meet for a meeting
        String[] returnVal= new String[3];
        for (int i=0; i<returnVal.length; i++) 
            returnVal[i] = new String();

        TreePath[] peopleTreePath = peopleTree.getSelectionPaths();
        for (int i=0; i< peopleTreePath.length; i++){
            DefaultMutableTreeNode selectedNode = 
                (DefaultMutableTreeNode)
                (peopleTreePath[i].getLastPathComponent());
            if (selectedNode.isLeaf()){
                returnVal[index++] = selectedNode.toString();
            }
        }
        return returnVal;
    }

    /** Class contains a Table Model for the JTable.  
      The Table has 5 columns and 24 rows. 
      This model is based off of the table included in the sample code provided on the course website.  
     */
    class TimeTableDataModel extends AbstractTableModel {

        private String[] columnName = {"Time", "Loc", "Pers 1", "Pers 2", "Pers 3"};
        private String[] locColumn = new String[24];
        private String[] pers1Column = new String[24];
        private String[] pers2Column = new String[24];
        private String[] pers3Column = new String[24];

        public TimeTableDataModel(){
            for(int i = 0; i < 24; ++i) {
                locColumn[i] = "";
                pers1Column[i] = "";
                pers2Column[i] = "";
                pers3Column[i] = "";
            }
        }

        /** methods used by the table model interface
         */
        public int getRowCount(){return 24;}
        public int getColumnCount(){return 5;}

        /** Returns the object at the specified row and column.
         */
        public Object getValueAt(int row, int col){
            String rval = "";

            // This inserts each hour of the day, with appropriate a.m. or p.m. tag, into the first column of the table.  
            // Note: These values never change.
            if (col == 0) {
                if (row == 0 || row == 12) {
                    rval = "12:00";
                } else {
                    rval = Integer.toString(row % 12) + ":00";
                }
                if (row < 12) {
                    rval += " a.m.";
                } else {
                    rval += " p.m.";
                }
                // These conditions deal with the location and people columns.
            } else if (col == 1) {
                rval = locColumn[row];
            } else if (col == 2) {
                rval = pers1Column[row];
            } else if (col == 3) {
                rval = pers2Column[row];
            } else if (col == 4) {
                rval = pers3Column[row];
            }
            return rval;
        }
        public String getColumnName(int index){
            return columnName[index];
        }

        /** Sets the value at the specified row and column with the a String object.
         */
        public void setValueAt(Object aValue, int row, int col) {
            if (!(aValue instanceof String)) return;
            String aString = (String)aValue;

            if (col == 1) {
                locColumn[row] = aString;
            } else if (col == 2) {
                pers1Column[row] = aString;
            } else if (col == 3) {
                pers2Column[row] = aString;
            } else if (col == 4) {
                pers3Column[row] = aString;
            }
            fireTableCellUpdated(row, col);
        }
    }

    /** Function creates the nodes of the JTree in this program.  
      This is taken directly from the sample code provided to us via the class website.
     */
    private void createNodes() {
        root = new DefaultMutableTreeNode("PKI People");
        DefaultMutableTreeNode departments = 
            new DefaultMutableTreeNode("Department");
        DefaultMutableTreeNode offices = 
            new DefaultMutableTreeNode("Offices");
        DefaultMutableTreeNode  cs = 
            new DefaultMutableTreeNode("Comp. Sci.");
        DefaultMutableTreeNode  isqa = 
            new DefaultMutableTreeNode("ISQA");
        DefaultMutableTreeNode  csFaculty = 
            new DefaultMutableTreeNode("CS Faculty");
        DefaultMutableTreeNode  csStaff = 
            new DefaultMutableTreeNode("CS Staff");
        DefaultMutableTreeNode  isqaFaculty = 
            new DefaultMutableTreeNode("ISQA Faculty");
        DefaultMutableTreeNode  isqaStaff = 
            new DefaultMutableTreeNode("ISQA Staff");
        DefaultMutableTreeNode  csFaculty1 = 
            new DefaultMutableTreeNode("CS Faculty1");
        DefaultMutableTreeNode  isqaFaculty1 = 
            new DefaultMutableTreeNode("ISQA Faculty1");
        DefaultMutableTreeNode  csFaculty2 = 
            new DefaultMutableTreeNode("CS Faculty2");
        DefaultMutableTreeNode  isqaFaculty2 = 
            new DefaultMutableTreeNode("ISQA Faculty2");
        DefaultMutableTreeNode  csStaff1 = 
            new DefaultMutableTreeNode("CS Staff1");
        DefaultMutableTreeNode  isqaStaff1 = 
            new DefaultMutableTreeNode("ISQA Staff1");
        DefaultMutableTreeNode  csStaff2 = 
            new DefaultMutableTreeNode("CS Staff2");
        DefaultMutableTreeNode  isqaStaff2 = 
            new DefaultMutableTreeNode("ISQA Staff2");	
        DefaultMutableTreeNode budgetOffice = 
            new DefaultMutableTreeNode("Budget Office");
        DefaultMutableTreeNode studentOffice = 
            new DefaultMutableTreeNode("Student Office");
        DefaultMutableTreeNode budgetOfficer1 = 
            new DefaultMutableTreeNode("Budget Officer1");
        DefaultMutableTreeNode budgetOfficer2 = 
            new DefaultMutableTreeNode("Budget Officer2");
        DefaultMutableTreeNode studentOfficer1 = 
            new DefaultMutableTreeNode("Student Officer1");
        DefaultMutableTreeNode studentOfficer2 = 
            new DefaultMutableTreeNode("Student Officer2");
        DefaultMutableTreeNode studentOfficer3 = 
            new DefaultMutableTreeNode("Student Officer3");
        DefaultMutableTreeNode classrooms = 
            new DefaultMutableTreeNode("Classrooms");

        // add the nodes

        root.add(departments);
        root.add(offices);
        departments.add(cs);
        departments.add(isqa);
        cs.add(csFaculty);
        cs.add(csStaff);
        isqa.add(isqaFaculty);
        isqa.add(isqaStaff);
        csFaculty.add(csFaculty1);
        csFaculty.add(csFaculty2);
        csFaculty1.setAllowsChildren(false);	
        csFaculty2.setAllowsChildren(false);
        csStaff.add(csStaff1);
        csStaff.add(csStaff2);
        csStaff1.setAllowsChildren(false);	
        csStaff2.setAllowsChildren(false);

        isqaFaculty.add(isqaFaculty1);
        isqaFaculty.add(isqaFaculty2);
        isqaFaculty1.setAllowsChildren(false);	
        isqaFaculty2.setAllowsChildren(false);
        isqaStaff.add(isqaStaff1);
        isqaStaff.add(isqaStaff2);
        isqaStaff1.setAllowsChildren(false);	
        isqaStaff2.setAllowsChildren(false);

        offices.add(budgetOffice);
        offices.add(studentOffice);
        budgetOffice.add(budgetOfficer1);
        budgetOffice.add(budgetOfficer2);
        budgetOfficer1.setAllowsChildren(false);	
        budgetOfficer2.setAllowsChildren(false);
        studentOffice.add(studentOfficer1);
        studentOffice.add(studentOfficer2);
        studentOffice.add(studentOfficer3);
        studentOfficer1.setAllowsChildren(false);	
        studentOfficer2.setAllowsChildren(false);
        studentOfficer3.setAllowsChildren(false);
    }
}
