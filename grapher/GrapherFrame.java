// File: GrapherFrame.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 8/9, Programming Problem 1
// This class contains a JFrame and JPanel for the graphing program.

import java.awt.*;
import javax.swing.*;
import java.io.*;
import java.awt.event.*;
import javax.swing.JFileChooser;
import java.util.Vector;
import java.awt.geom.GeneralPath;
import java.awt.geom.Rectangle2D;
import java.awt.geom.Ellipse2D;

/** This class contains a JFrame.
 */
class GrapherFrame extends JFrame {
    /** This function creates a JFrame based on the user's screen resolution and adds a menu system.
     */
    
    private File filename = null;
    private BufferedReader in;
    private PrintStream output;
    private floatVector xLoc = new floatVector();
    private floatVector yLoc = new floatVector();
    boolean lines = true;
    boolean symbols = true;
    private String symbol = "Open Circle";
    private GraphPanel graph;

    public GrapherFrame() {

        setDefaultCloseOperation(3);
        
        ActionListener listener = new ButtonHandler(); //action listener

        // this gets the user's screensize
        Toolkit tkit = Toolkit.getDefaultToolkit();
        Dimension dimension = tkit.getScreenSize();
        int height = dimension.height;
        int width = dimension.width;

        // setting up the frames name, location, and size.
        setTitle("Grapher");
        setLocation(width / 4, height / 4);
        setSize(width / 2, height / 2);

        //JMenu objects
        JMenuBar menuBar;
        JMenu menu, submenu;
        JMenuItem menuItem;
        JCheckBoxMenuItem cbMenuItem;
        JRadioButtonMenuItem rbMenuItem;
            
        //Create the menu bar.
        menuBar = new JMenuBar();
        setJMenuBar(menuBar);

        // -----------------------------File Menu------------------------------
        menu = new JMenu("File");
        menu.setMnemonic(KeyEvent.VK_F);
        menuBar.add(menu);

        menuItem = new JMenuItem("Open", KeyEvent.VK_O);
        menuItem.addActionListener( listener );
        menu.add(menuItem);
        
        menuItem = new JMenuItem("Save", KeyEvent.VK_S);
        menuItem.addActionListener( listener );
        menu.add(menuItem);

        menuItem = new JMenuItem("Save As", KeyEvent.VK_A);
        menuItem.addActionListener( listener );
        menu.add(menuItem);

        menuItem = new JMenuItem("Exit", KeyEvent.VK_E);
        menuItem.addActionListener( listener );
        menu.add(menuItem);
        
        // -----------------------------Manipulate Menu------------------------------
        menu = new JMenu("Manipulate");
        menu.setMnemonic(KeyEvent.VK_M);
        menuBar.add(menu);

        menuItem = new JMenuItem("Add", KeyEvent.VK_A);
        menuItem.addActionListener( listener );
        menu.add(menuItem);

        menuItem = new JMenuItem("Subtract", KeyEvent.VK_S);
        menuItem.addActionListener( listener );
        menu.add(menuItem);

        menuItem = new JMenuItem("Multiply", KeyEvent.VK_M);
        menuItem.addActionListener( listener );
        menu.add(menuItem);
        
        menuItem = new JMenuItem("Divide", KeyEvent.VK_D);
        menuItem.addActionListener( listener );
        menu.add(menuItem);
        
        menuItem = new JMenuItem("Power", KeyEvent.VK_P);
        menuItem.addActionListener( listener );
        menu.add(menuItem);
        
        menuItem = new JMenuItem("Normalize", KeyEvent.VK_N);
        menuItem.addActionListener( listener );
        menu.add(menuItem);
        
        // -----------------------------Graph Menu------------------------------
        menu = new JMenu("Graph");
        menu.setMnemonic(KeyEvent.VK_G);
        menuBar.add(menu);

        // Type checkbox submenu
        submenu = new JMenu("Type");
        submenu.setMnemonic(KeyEvent.VK_T);

        cbMenuItem = new JCheckBoxMenuItem("Lines", true);
        cbMenuItem.setMnemonic(KeyEvent.VK_L);
        cbMenuItem.addActionListener( listener );
        submenu.add(cbMenuItem);

        cbMenuItem = new JCheckBoxMenuItem("Symbols", true);
        cbMenuItem.setMnemonic(KeyEvent.VK_S);
        cbMenuItem.addActionListener( listener );
        submenu.add(cbMenuItem);

        menu.add(submenu);

        // Symbols radiobutton submenu
        submenu = new JMenu("Symbols");
        submenu.setMnemonic(KeyEvent.VK_S);

        ButtonGroup group = new ButtonGroup();
        rbMenuItem = new JRadioButtonMenuItem("Open Circle");
        rbMenuItem.setSelected(true);
        rbMenuItem.setMnemonic(KeyEvent.VK_O);
        rbMenuItem.addActionListener( listener );
        group.add(rbMenuItem);
        submenu.add(rbMenuItem);

        rbMenuItem = new JRadioButtonMenuItem("Closed Circle");
        rbMenuItem.setMnemonic(KeyEvent.VK_C);
        rbMenuItem.addActionListener( listener );
        group.add(rbMenuItem);
        submenu.add(rbMenuItem);
        
        rbMenuItem = new JRadioButtonMenuItem("Open Square");
        rbMenuItem.setMnemonic(KeyEvent.VK_S);
        rbMenuItem.addActionListener( listener );
        group.add(rbMenuItem);
        submenu.add(rbMenuItem);
        
        rbMenuItem = new JRadioButtonMenuItem("Closed Square");
        rbMenuItem.setMnemonic(KeyEvent.VK_D);
        rbMenuItem.addActionListener( listener );
        group.add(rbMenuItem);
        submenu.add(rbMenuItem);

        menu.add(submenu);
        
        menuBar.add(Box.createHorizontalGlue());

        // -----------------------------About Menu item------------------------------
        
        menu = new JMenu("About");
        menu.setMnemonic(KeyEvent.VK_A);
        menuItem = new JMenuItem("About", KeyEvent.VK_A);
        menuItem.addActionListener( listener );
        menu.add(menuItem);
        menuBar.add(menu);


        // ----------------------------Graph JPanel-----------------------------------
        graph = new GraphPanel();
        Container container = getContentPane();
        container.add(graph);
    }

    /** This class contain the JPanel for the actual graph.
     */
    private class GraphPanel extends JPanel {
        private double shape_width;
        private double shape_height;
        private int x_loc;
        private int y_loc;

        public GraphPanel() {
        }

        public void paintComponent (Graphics g) {
            super.paintComponent(g);
            Graphics2D g2 = (Graphics2D)g;
            GeneralPath gpath;
            
            // X Axis
            gpath = new GeneralPath();
            gpath.moveTo( 5, getHeight() / 2 );
            gpath.lineTo( getWidth(), getHeight() / 2);
            g2.draw(gpath);
            g2.drawString("X Data", 13 * getWidth() / 15, getHeight() / 2);
            
            // Y Axis
            gpath.moveTo( 5, 0 );
            gpath.lineTo( 5, getHeight());
            g2.draw(gpath);
            g2.drawString("Y Data", 10, getHeight() / 10);
            g2.drawString("0", 7, getHeight() / 2 + 13);
            
            // If no data has been read, just make the tick marks and bail out.
            if (xLoc.size() == 0) {
                // X tick
                gpath.moveTo(getWidth() - 5, getHeight() / 2 + 5);
                gpath.lineTo(getWidth() - 5, getHeight() / 2 - 5);
                g2.draw(gpath);
                g2.drawString("1", getWidth() - 10, getHeight() / 2 + 20);

                // Y ticks
                gpath.moveTo(0, 5);
                gpath.lineTo(10, 5);
                g2.draw(gpath);
                g2.drawString("1", 15, 10);
            
                gpath.moveTo(0, getHeight() - 5);
                gpath.lineTo(10, getHeight() - 5);
                g2.draw(gpath);
                g2.drawString("-1", 15, getHeight() - 1);
                
                return;
            }
            
            // Temporary vectors to hold sorted elements for graphing.
            floatVector sortedX = new floatVector();
            floatVector sortedY = new floatVector();
            
            for( int i = 0; i < xLoc.size(); ++i) {
                sortedX.add( xLoc.get(i) );
                sortedY.add( yLoc.get(i) );
            }
            
            // Sorts the data via X values.
            for( int i = 0; i < sortedX.size(); ++i) {
                for( int j = 0; j < sortedX.size() - 1; ++j) {
                    if (sortedX.get(j) > sortedX.get(j + 1)) {
                        float temp = sortedX.get(j);
                        sortedX.set(j, sortedX.get(j+1));
                        sortedX.set(j+1, temp);
                        
                        temp = sortedY.get(j);
                        sortedY.set(j, sortedY.get(j+1));
                        sortedY.set(j+1, temp);
                    }
                }
            }

            // Graphs the data and modified tick marks.
            // Max X and Y values.
            float xmax = sortedX.get(sortedX.size() - 1);
            float ymax = Math.abs( sortedY.get(0) );
            
            // Determines maximum Y value.
            for (int i = 1; i < sortedY.size(); ++i) {
                if ( Math.abs( sortedY.get(i) ) > ymax)
                    ymax = Math.abs( sortedY.get(i) );
            }

            // Graphs each line if toggled.
            if (lines) {
                gpath.moveTo(sortedX.get(0) * getWidth() / xmax + 5, getHeight() * (1 - sortedY.get(0) / ymax) / 2 );
                for (int i = 1; i < sortedX.size(); ++i) {
                    gpath.lineTo( sortedX.get(i) * getWidth() / xmax + 5, getHeight() * (1 - sortedY.get(i) / ymax) / 2 );
                }
            }

            // Graphs each symbol if toggled.
            if (symbols) {
                for (int i = 0; i < sortedX.size(); ++i) {
                    Rectangle2D square = new java.awt.geom.Rectangle2D.Float( sortedX.get(i) * getWidth() / xmax + 2, 
                            getHeight() * (1 - sortedY.get(i) / ymax) / 2 - 3,
                            7, 7 );

                    // Chooses whether to draw filled or empty circles or squares
                    if (symbol.equals("Open Circle")) {
                        Ellipse2D circle = new java.awt.geom.Ellipse2D.Float();
                        circle.setFrame(square);
                        g2.draw(circle);
                    } else if (symbol.equals("Closed Circle")) {
                        Ellipse2D circle = new java.awt.geom.Ellipse2D.Float();
                        circle.setFrame(square);
                        g2.fill(circle);
                    } else if (symbol.equals("Open Square")) {
                        g2.draw(square);
                    } else if (symbol.equals("Closed Square")) {
                        g2.fill(square);
                    }
                }
            }

            // Tick marks
            // X tick
            gpath.moveTo(getWidth() - 5, getHeight() / 2 + 5);
            gpath.lineTo(getWidth() - 5, getHeight() / 2 - 5);
            g2.draw(gpath);
            g2.drawString(new Float(xmax).toString(), 29 * getWidth() / 30, getHeight() / 2 + 20);

            // Y tick
            gpath.moveTo(0, 5);
            gpath.lineTo(10, 5);
            g2.draw(gpath);
            g2.drawString(new Float(ymax).toString(), 15, 10);

            gpath.moveTo(0, getHeight() - 5);
            gpath.lineTo(10, getHeight() - 5);
            g2.draw(gpath);
            g2.drawString(new Float(-ymax).toString(), 15, getHeight() - 1);
            
        }
    }

    /** This class is an ActionListener that handles all menu-driven events.
     */
    private class ButtonHandler implements ActionListener {
        public void actionPerformed( ActionEvent e ) {
            String action = e.getActionCommand();

            // Open
            if (action.equals( "Open" ) ) {
                
                // Implements the JFileChooser
                JFileChooser jFC = new JFileChooser();
                
                jFC.setFileSelectionMode(JFileChooser.FILES_ONLY);
                int result = jFC.showOpenDialog(GrapherFrame.this);
                if (result == jFC.CANCEL_OPTION) 
                    return;
                
                filename = jFC.getSelectedFile();
                
                try {
                    BufferedReader in = new BufferedReader(new FileReader(filename));
                    String line;

                    // Clears the vector after any subsequent file opens.
                    xLoc.clear();
                    yLoc.clear();

                    // Reads in from the file one line at a time and parses it.
                    while ((line = in.readLine()) != null) {
                        String patternStr = "[ \t]";
                        String[] fields = line.split(patternStr);
                        
                        xLoc.add( Float.valueOf( fields[0] ) );
                        yLoc.add( Float.valueOf( fields[1] ) );
                    }

                    in.close();
                } catch (IOException x) {
                    x.printStackTrace();
                }

                // Save
            } else if (action.equals( "Save" ) ) {
                if (filename == null) {
                    JOptionPane.showMessageDialog( GrapherFrame.this, "No file to save to",
                            "Error", JOptionPane.ERROR_MESSAGE );
                    return;
                }
                
                try {
                    output = new PrintStream(new FileOutputStream(filename));
                } catch (IOException iOE) {
                    JOptionPane.showMessageDialog( GrapherFrame.this, "Error Opening File",
                                                   "Error", JOptionPane.ERROR_MESSAGE );
                }
                
                // Writes to the file with tab-delimited format.
                for (int i = 0; i < xLoc.size(); ++i) {
                    output.println( xLoc.get(i) + "\t" + yLoc.get(i) );
                }
                
                // Closes Filehandler
                output.close();

                // Save As
            } else if (action.equals( "Save As" ) ) {
                // Gets the filename from JFileChooser
                JFileChooser jFC = new JFileChooser();
                
                jFC.setFileSelectionMode(JFileChooser.FILES_ONLY);
                int result = jFC.showSaveDialog(GrapherFrame.this);
                if (result == jFC.CANCEL_OPTION) return;
                
                filename = jFC.getSelectedFile();
                
                // Errors if the file exists.
                if ( filename.exists() ) 
                {
                    JOptionPane.showMessageDialog( GrapherFrame.this, "File Already Exists",
                            "Error", JOptionPane.ERROR_MESSAGE );
                    return;
                }
                
                // Writes to the file.
                try {
                    output = new PrintStream(new FileOutputStream(filename));
                } catch (IOException iOE) {
                    JOptionPane.showMessageDialog( GrapherFrame.this, "Error Opening File",
                                                   "Error", JOptionPane.ERROR_MESSAGE );
                }
                for (int i = 0; i < xLoc.size(); ++i) {
                    output.println( xLoc.get(i) + "\t" + yLoc.get(i) );
                }
                output.close();
                
                // Exit
            } else if (action.equals( "Exit" ) ) {
                System.exit(1);

                // Addition
            } else if (action.equals( "Add" ) ) {
                String s = (String)JOptionPane.showInputDialog(
                        GrapherFrame.this,
                        "Enter constant to add to Y data:");
                
                for (int i = 0; i < yLoc.size(); ++i) {
                   yLoc.set(i, yLoc.get(i) + Float.valueOf(s).floatValue() );
                }

                // Subtraction
            } else if (action.equals( "Subtract" ) ) {
                String s = (String)JOptionPane.showInputDialog(
                        GrapherFrame.this,
                        "Enter constant to subtract from Y data:");
                
                for (int i = 0; i < yLoc.size(); ++i) {
                   yLoc.set(i, yLoc.get(i) - Float.valueOf(s).floatValue() );
                }
                
                // Multiplication
            } else if (action.equals( "Multiply" ) ) {
                String s = (String)JOptionPane.showInputDialog(
                        GrapherFrame.this,
                        "Enter constant to multiply Y data by:");
                
                for (int i = 0; i < yLoc.size(); ++i) {
                   yLoc.set(i, yLoc.get(i) * Float.valueOf(s).floatValue() );
                }
                
                // Division
            } else if (action.equals( "Divide" ) ) {
                String s = (String)JOptionPane.showInputDialog(
                        GrapherFrame.this,
                        "Enter constant to divide Y data by:");
                
                if (s.equals("0")) {
                    JOptionPane.showMessageDialog( GrapherFrame.this, "Cannot Divide by 0", 
                            "Error", JOptionPane.ERROR_MESSAGE );
                    return;
                }

                for (int i = 0; i < yLoc.size(); ++i) {
                   yLoc.set(i, yLoc.get(i) / Float.valueOf(s).floatValue() );
                }
                
                // Power 
            } else if (action.equals( "Power" ) ) {
                String s = (String)JOptionPane.showInputDialog(
                        GrapherFrame.this,
                        "Enter constant to raise Y data to:");
                
                for (int i = 0; i < yLoc.size(); ++i) {
                   yLoc.set( i, (float)Math.pow(yLoc.get(i), Float.valueOf(s).floatValue()) );
                }
                // Normalization
            } else if (action.equals( "Normalize" ) ) {
                float max = 0;
                for (int i = 0; i < yLoc.size(); ++i) {
                    max = Math.max( max, Math.abs(yLoc.get(i)) );
                }

                for (int i = 0; i < yLoc.size(); ++i) {
                   yLoc.set(i, yLoc.get(i) / max );
                }

                // About
            } else if (action.equals( "About" ) ) {
                JOptionPane.showMessageDialog( GrapherFrame.this, 
                        "Grapher V1.0\nBy Kevin Zink\nSubmitted on Thursday, December 18th, 2003",
                        "About this program", 
                        JOptionPane.PLAIN_MESSAGE );

                // Checkbox toggles
            } else if (action.equals( "Lines" ) ) {
                if (lines) 
                    lines = false;
                else 
                    lines = true;
            } else if (action.equals( "Symbols" ) ) {
                if (symbols)
                    symbols = false;
                else 
                    symbols = true;

                // Radiobutton toggles
            } else if (action.equals( "Open Circle" ) ) {
                symbol = "Open Circle";
            } else if (action.equals( "Closed Circle" ) ) {
                symbol = "Closed Circle";
            } else if (action.equals( "Open Square" ) ) {
                symbol = "Open Square";
            } else if (action.equals( "Closed Square" ) ) {
                symbol = "Closed Square";
            }

            // repaints the graph after any changes.
            graph.repaint(0, 0, 0, graph.getWidth(), graph.getHeight());
        }
    }
}
