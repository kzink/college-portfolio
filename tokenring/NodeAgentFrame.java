import javax.swing.JPanel;
import javax.swing.JFrame;
import javax.swing.JOptionPane;
import javax.swing.text.*;
import java.awt.*;
import java.io.*;

/*
 * Created on Apr 24, 2004
 *
 */

/**
 * @author Noah Anderson and Kevin Zink
 *
 */
public class NodeAgentFrame extends JFrame {

	private javax.swing.JLabel jLabel = null;
	private javax.swing.JTextField MessageEntryField = null;
	private javax.swing.JTextField TargetEntryField = null;
	private javax.swing.JButton SendButton = null;
	private javax.swing.JPanel SouthPanel = null;
	private javax.swing.JScrollPane CenterPanel = null;
	private javax.swing.JButton ExitButton = null;
	private javax.swing.JPanel NorthPanel = null;
	private javax.swing.JTextArea MessageTextArea = null;
	private javax.swing.JLabel jLabel1 = null;
	private javax.swing.JLabel jLabel2 = null;
	private javax.swing.JLabel jLabel3 = null;
	private javax.swing.JLabel jLabel4 = null;
	private javax.swing.JPanel NeighborPanel1 = null;
	private javax.swing.JPanel NeighborPanel2 = null;
	private javax.swing.JTextField NeighborEntryField1 = null;
	private javax.swing.JTextField NeighborEntryField2 = null;
	private javax.swing.JTextField NameEntryField = null;
	private javax.swing.JButton SetNeighborsButton = null;
	private javax.swing.JButton StartButtonToken = null;
    private String chatHeader = "";

	//used for window size and position
	private Toolkit tkit = Toolkit.getDefaultToolkit();
	private Dimension dimension = tkit.getScreenSize();
	private int height = dimension.height;
	private int width = dimension.width;
    private NodeAgent parent;

	/**
	 * This is the default constructor
	 */
	public NodeAgentFrame(NodeAgent agent) {
		super();
        parent = agent;
		initialize();
	}
	/**
	 * This method initializes this
	 * 
	 * @return void
	 */
	private void initialize() {

		this.getContentPane().setLayout(new java.awt.BorderLayout());
		this.getContentPane().add(getCenterPanel());
		this.getContentPane().add(getNorthPanel(), java.awt.BorderLayout.NORTH);
		this.getContentPane().add(getSouthPanel(), java.awt.BorderLayout.SOUTH);

		setDefaultCloseOperation(3);

		// setting up the frames name, location, and size.
		setTitle("Token Aglet Client");
		setLocation(width / 4, height / 5);
	}

	/**
	 * This method initializes disables the Start Token Button
	 * 
	 */
    public void disableTokenButton() {
        StartButtonToken.setEnabled(false);
    }
    
	/**
	 * This method initializes appends text to the main MessageTextArea
	 * 
	 */
    public void append(String text) {
        MessageTextArea.append("\n" + text);
        MessageTextArea.setCaretPosition(MessageTextArea.getText().length());
    }
	
    /**
	 * This method changes the value of the neighbor 1 text
	 * 
	 */
    public void setNeighborOne(String text) {
	    NeighborEntryField1.setText(text);
    }
    
    /**
	 * This method changes the value of the neighbor 2 text
	 * 
	 */
    public void setNeighborTwo(String text) {
	    NeighborEntryField2.setText(text);
    }
    
	/**
	 * This method initializes simulates the setup button being pressed (to speed up testing)
	 * 
	 */
    public void fireSetupButton() {
        SetNeighborsButton.doClick();
    }
    
	/**
	 * This method initializes jLabel3
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getJLabel3() {
		if(jLabel3 == null) {
			jLabel3 = new javax.swing.JLabel();
			jLabel3.setText("Target:");
		}
		return jLabel3;
	}
	/**
	 * This method initializes jLabel
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getJLabel() {
		if(jLabel == null) {
			jLabel = new javax.swing.JLabel();
			jLabel.setText("Message:");
		}
		return jLabel;
	}
	/**
	 * This method initializes TargetEntryField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getTargetEntryField() {
		if(TargetEntryField == null) {
			TargetEntryField = new javax.swing.JTextField();
			TargetEntryField.setColumns(10);
		}
		return TargetEntryField;
	}
	/**
	 * This method initializes MessageEntryField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getMessageEntryField() {
		if(MessageEntryField == null) {
			MessageEntryField = new javax.swing.JTextField();
			MessageEntryField.setColumns(23);
		}
		return MessageEntryField;
	}
	/**
	 * This method initializes SendButton
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getSendButton() {
		if(SendButton == null) {
			SendButton = new javax.swing.JButton();
			SendButton.setText("Send");
			SendButton.setEnabled(false);
			SendButton.addActionListener(new java.awt.event.ActionListener() { 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
                    if ( !MessageEntryField.getText().equals("") && !TargetEntryField.getText().equals("")) {
                        append(chatHeader + MessageEntryField.getText());
                        parent.deployMessageAgent(chatHeader + MessageEntryField.getText(), TargetEntryField.getText());
                    }
                }
			});
		}
		return SendButton;
	}
	/**
	 * This method initializes CenterPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JScrollPane getCenterPanel() {
		if(CenterPanel == null) {
			CenterPanel = new javax.swing.JScrollPane();
		    CenterPanel.setViewportView(getMessageTextArea());
		}
		return CenterPanel;
	}
	/**
	 * This method initializes SouthPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getSouthPanel() {
		if(SouthPanel == null) {
			SouthPanel = new javax.swing.JPanel();
            SouthPanel.add(getJLabel3(), null);
			SouthPanel.add(getTargetEntryField(), null);
            SouthPanel.add(getJLabel(), null);
			SouthPanel.add(getMessageEntryField(), null);
			SouthPanel.add(getSendButton(), null);
			SouthPanel.add(getExitButton(), null);
			SouthPanel.setBorder(javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.RAISED));
		}
		return SouthPanel;
	}
	/**
	 * This method initializes ExitButton
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getExitButton() {
		if(ExitButton == null) {
			ExitButton = new javax.swing.JButton();
			ExitButton.setText("Exit");
			ExitButton.addActionListener(new java.awt.event.ActionListener() { 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
                    parent.dispose();
				}
			});
		}
		return ExitButton;
	}
	/**
	 * This method initializes NorthPanel
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getNorthPanel() {
		if(NorthPanel == null) {
			NorthPanel = new javax.swing.JPanel();
			java.awt.GridLayout layGridLayout2 = new java.awt.GridLayout();
			layGridLayout2.setRows(2);
			layGridLayout2.setColumns(2);
			NorthPanel.setLayout(layGridLayout2);
			NorthPanel.add(getNeighborPanel1(), null);
			NorthPanel.add(getNeighborPanel2(), null);
			NorthPanel.setBorder(javax.swing.BorderFactory.createEtchedBorder(javax.swing.border.EtchedBorder.RAISED));
		}
		return NorthPanel;
	}
	/**
	 * This method initializes MessageTextField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextArea getMessageTextArea() {
		if(MessageTextArea == null) {
			MessageTextArea = new javax.swing.JTextArea();
			MessageTextArea.setEditable(false);
            MessageTextArea.setLineWrap(true);
            MessageTextArea.setRows(10);
			MessageTextArea.setBackground(new java.awt.Color(255,255,252));
		}
		return MessageTextArea;
	}
	/**
	 * This method initializes jLabel1
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getJLabel1() {
		if(jLabel1 == null) {
			jLabel1 = new javax.swing.JLabel();
			jLabel1.setText("Neighbor 1:");
		}
		return jLabel1;
	}
	/**
	 * This method initializes jLabel2
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getJLabel2() {
		if(jLabel2 == null) {
			jLabel2 = new javax.swing.JLabel();
			jLabel2.setText("Neighbor 2:");
		}
		return jLabel2;
	}
	/**
	 * This method initializes jLabel4
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getJLabel4() {
		if(jLabel4 == null) {
			jLabel4 = new javax.swing.JLabel();
			jLabel4.setText("Name:");
		}
		return jLabel4;
	}
	/**
	 * This method initializes NeighborPanel1
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getNeighborPanel1() {
		if(NeighborPanel1 == null) {
			NeighborPanel1 = new javax.swing.JPanel();
			java.awt.FlowLayout layFlowLayout4 = new java.awt.FlowLayout();
			layFlowLayout4.setHgap(20);
			NeighborPanel1.setLayout(layFlowLayout4);
			NeighborPanel1.add(getJLabel1(), null);
			NeighborPanel1.add(getNeighborEntryField1(), null);
			
            NeighborPanel1.add(getJLabel4(), null);
			NeighborPanel1.add(getNameEntryField(), null);
		}
		return NeighborPanel1;
	}
	/**
	 * This method initializes NeighborPanel2
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getNeighborPanel2() {
		if(NeighborPanel2 == null) {
			NeighborPanel2 = new javax.swing.JPanel();
			java.awt.FlowLayout layFlowLayout3 = new java.awt.FlowLayout();
			layFlowLayout3.setHgap(20);
			NeighborPanel2.setLayout(layFlowLayout3);
			NeighborPanel2.add(getJLabel2(), null);
			NeighborPanel2.add(getNeighborEntryField2(), null);
			NeighborPanel2.add(getSetNeighborsButton(), null);
			NeighborPanel2.add(getStartButtonToken(), null);
		}
		return NeighborPanel2;
	}
	/**
	 * This method initializes getNameEntryField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getNameEntryField() {
		if(NameEntryField == null) {
			NameEntryField = new javax.swing.JTextField();
			NameEntryField.setColumns(10);
		}
		return NameEntryField;
	}
	/**
	 * This method initializes NeighborEntryField1
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getNeighborEntryField1() {
		if(NeighborEntryField1 == null) {
			NeighborEntryField1 = new javax.swing.JTextField();
			NeighborEntryField1.setColumns(10);
		}
		return NeighborEntryField1;
	}
	/**
	 * This method initializes NeighborEntryField2
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getNeighborEntryField2() {
		if(NeighborEntryField2 == null) {
			NeighborEntryField2 = new javax.swing.JTextField();
			NeighborEntryField2.setColumns(10);
		}
		return NeighborEntryField2;
	}
	/**
	 * This method initializes SetNeighborsButton
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getSetNeighborsButton() {
		if(SetNeighborsButton == null) {
			SetNeighborsButton = new javax.swing.JButton();
			SetNeighborsButton.setText("Setup Network");
			SetNeighborsButton.addActionListener(new java.awt.event.ActionListener() { 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
                    if ( !NeighborEntryField1.getText().equals("") && !NeighborEntryField2.getText().equals("") && !NameEntryField.getText().equals("")) {
                        parent.setNeighbors(NeighborEntryField1.getText(), NeighborEntryField2.getText());
                        parent.setName(NameEntryField.getText());
                        SetNeighborsButton.setEnabled(false);
                        SendButton.setEnabled(true);
                        StartButtonToken.setEnabled(true);
                        NeighborEntryField1.setEditable(false);
                        NeighborEntryField2.setEditable(false);
                        NameEntryField.setEditable(false);
                        chatHeader = "<" + NameEntryField.getText() + "> ";
                        MessageTextArea.setText("Chat Client Enabled");
                        setTitle("Token Aglet Client: " + NameEntryField.getText());
                    }
				}
			});
		}
		return SetNeighborsButton;
	}
	/**
	 * This method initializes StartButtonToken
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getStartButtonToken() {
		if(StartButtonToken == null) {
			StartButtonToken = new javax.swing.JButton();
			StartButtonToken.setText("Start Token");
			StartButtonToken.setPreferredSize(new java.awt.Dimension(114,26));
			StartButtonToken.setEnabled(false);
			StartButtonToken.addActionListener(new java.awt.event.ActionListener() { 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
					System.out.println("Initiate Token Button"); // TODO Auto-generated Event stub actionPerformed()
                    parent.deployToken();
				}
			});
		}
		return StartButtonToken;
	}

    /**
     * This method loads default values based on the default.txt file
     *
     */
    public void loadDefault()
    {
        try
        {
            String address = parent.getProxy().getAddress();
            String IP = parent.hostToIP(parent.parseHost(address));
            String port = parent.parsePort(address);

            BufferedReader in = new BufferedReader(new FileReader("C:\\java\\aglets-2.0.2\\public\\default.txt"));
            while (in.ready())
            {
                String line = in.readLine();

                // Ignore comments
                if (line.startsWith("#"))
                    continue;

                String parts[] = line.split("\\|");
                String currAddress = "atp://" + parts[0] + "/";

                // Ignore incomplete lines
                if (parts.length < 4)
                    continue;

                String currIP = parent.hostToIP(parent.parseHost(currAddress));
                String currPort = parent.parsePort(currAddress);
                System.out.println("IP: " + currIP + " and port: " + currPort);

                if (currIP.equals(IP) && currPort.equals(port))
                {
                    getNameEntryField().setText(parts[1]);
                    getNeighborEntryField1().setText(parts[2]);
                    getNeighborEntryField2().setText(parts[3]);
                }
            }
            in.close();
        }
        catch (Exception e)
        {
            e.printStackTrace();
        }
    }
}
