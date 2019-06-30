// File: ChatClient.java
// Author: Kevin Zink
// Programming problem #2, part 2
// Created on Feb 20, 2004
// This is a chat client to be used with TalkServer.java

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.InetAddress;
import java.net.SocketException;
import java.net.UnknownHostException;
import javax.swing.JFrame;
import java.awt.*;
import javax.swing.JOptionPane;
import javax.swing.text.*;

/**ChatClient
 * @author Kevin Zink
 */
public class ChatClient extends JFrame {

	private javax.swing.JPanel mainWindow = null;
	private javax.swing.JScrollPane jScrollPane = null;
	private javax.swing.JTextArea textWindow = null;
	private javax.swing.JTextField inputField = null;
	private static javax.swing.JButton sendButton = null;
	private javax.swing.JPanel actionWindow = null;
	private javax.swing.JPanel inputBar = null;
	private javax.swing.JPanel buttonBar = null;
	private javax.swing.JButton connectBox = null;
	private javax.swing.JButton closeButton = null;
	private javax.swing.JDialog connectDialog = null;
	private javax.swing.JPanel dialogBox = null;
	private javax.swing.JTextField portField = null;
	private javax.swing.JTextField addressField = null;
	private javax.swing.JTextField usernameField = null;
	private javax.swing.JButton connectButton = null;
	private javax.swing.JLabel portLabel = null;
	private javax.swing.JLabel addressLabel = null;
	private javax.swing.JLabel usernameLabel = null;
	private javax.swing.JPanel addressBar = null;
	private javax.swing.JPanel usernameBar = null;
	private static DatagramSocket socket = null;
	private String userName = null;
	private byte[] buf = new byte[256];
	private DatagramPacket inPacket = new DatagramPacket(buf, buf.length); //packet used to handle received packets
	private DatagramPacket outPacket = new DatagramPacket(new byte[1], 1); //packet used to construct outgoing packets
	private Thread readerThread = null;
	private javax.swing.JPanel connectBar = null;

    //used for window size and position
    private Toolkit tkit = Toolkit.getDefaultToolkit();
    private Dimension dimension = tkit.getScreenSize();
    private int height = dimension.height;
    private int width = dimension.width;

    /**
     * This is the main function
     */
    public static void main(String[] args) {
		try {
			socket = new DatagramSocket(); //initiation of socket
		} catch (SocketException e) {
			e.printStackTrace();
		}
		new ChatClient().show();
	}

	/**
	 * This is the default constructor
	 */
	public ChatClient() {
		super();
		initialize();
	}
	/**
	 * This method initializes *this*
	 * 
	 * @return void
	 */
	private void initialize() {
        setDefaultCloseOperation(3);

        // setting up the frames name, location, and size.
        setTitle("Chat Client");
        setLocation(width / 4, height / 4);
        setSize(width / 2, height / 2);
        this.setContentPane(getMainWindow());
	}
	/**
	 * This method initializes mainWindow
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getMainWindow() {
		if (mainWindow == null) {
			mainWindow = new javax.swing.JPanel();
			mainWindow.setLayout(new java.awt.BorderLayout());
			mainWindow.add(getJScrollPane(), java.awt.BorderLayout.CENTER);
			mainWindow.add(getActionWindow(), java.awt.BorderLayout.SOUTH);
		}
		return mainWindow;
	}
	/**
	 * This method initializes jScrollPane to be used with textWindow
	 * 
	 * @return javax.swing.JScrollPane
	 */
	private javax.swing.JScrollPane getJScrollPane() {
		if(jScrollPane == null) {
			jScrollPane = new javax.swing.JScrollPane();
			jScrollPane.setViewportView(getTextWindow());
		}
		return jScrollPane;
	}
	/**
	 * This method initializes textWindow
	 * 
	 * @return javax.swing.JTextArea
	 */
	private javax.swing.JTextArea getTextWindow() {
		if(textWindow == null) {
			textWindow = new javax.swing.JTextArea();
			textWindow.setEditable(false);
            textWindow.setLineWrap(true);  //line wrapping
		}
		return textWindow;
	}
	/**
	 * This method initializes inputField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getInputField() {
		if(inputField == null) {
			inputField = new javax.swing.JTextField();
			inputField.setColumns(25);

            // This limits the textfield to 50 characters
            inputField.setDocument(new PlainDocument() {
                public void insertString(int offs, String str, AttributeSet a) throws BadLocationException {
                    if ((getLength() + str.length()) <= 50) {
                        super.insertString(offs, str, a);
                    } else {
                        throw new BadLocationException("Insertion exceeds 50 characters", offs);
                    }
                }
            });
        }
        return inputField;
	}

	/**
	 * This method initializes sendButton
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getSendButton() {
		if(sendButton == null) {
			sendButton = new javax.swing.JButton();
			sendButton.setName("");
			sendButton.setText("Send");
			sendButton.setEnabled(false); //send button is disabled until the connect dialog has been completed.
            
            // Attempts to send message when button is pressed
			sendButton.addActionListener(new java.awt.event.ActionListener() { 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
					if (!inputField.getText().equals("")) {
						send(userName + ": " + inputField.getText());
						inputField.setText("");
					}
				}
			});
		}
		return sendButton;
	}
	/**
	 * This method initializes actionWindow
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getActionWindow() {
		if(actionWindow == null) {
			actionWindow = new javax.swing.JPanel();
			java.awt.GridLayout layGridLayout2 = new java.awt.GridLayout();
			layGridLayout2.setRows(2);
			layGridLayout2.setColumns(2);
			actionWindow.setLayout(layGridLayout2);
			actionWindow.add(getInputBar(), null);
			actionWindow.add(getButtonBar(), null);
		}
		return actionWindow;
	}
	/**
	 * This method initializes inputBar
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getInputBar() {
		if(inputBar == null) {
			inputBar = new javax.swing.JPanel();
			inputBar.add(getInputField(), null);
			inputBar.add(getSendButton(), null);
		}
		return inputBar;
	}
	/**
	 * This method initializes buttonBar
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getButtonBar() {
		if(buttonBar == null) {
			buttonBar = new javax.swing.JPanel();
			buttonBar.setLayout(new java.awt.FlowLayout());
			buttonBar.add(getConnectBox(), null);
			buttonBar.add(getCloseButton(), null);
		}
		return buttonBar;
	}
	/**
	 * This method initializes connectBox
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getConnectBox() {
		if(connectBox == null) {
			connectBox = new javax.swing.JButton();
			connectBox.setText("Connect");
            
            // Creates dialog box for connection options
			connectBox.addActionListener(new java.awt.event.ActionListener() { 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
					getConnectDialog().show();
				}
			});
		}
		return connectBox;
	}
	/**
	 * This method initializes closeButton
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getCloseButton() {
		if(closeButton == null) {
			closeButton = new javax.swing.JButton();
			closeButton.setText("Close");
			closeButton.addActionListener(new java.awt.event.ActionListener() { 

                // Exits program and sends a "<user> has left the chat room." message 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
					if(sendButton.isEnabled()) {
						send(userName + " has left the chat room.");
					}
                    socket.close(); //closes socket
					System.exit(1);
				}
			});
		}
		return closeButton;
	}
	/**
	 * This method initializes dialogBox
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getDialogBox() {
		if(dialogBox == null) {
			dialogBox = new javax.swing.JPanel();
			java.awt.GridLayout layGridLayout4 = new java.awt.GridLayout();
			layGridLayout4.setRows(3);
			layGridLayout4.setColumns(1);
			dialogBox.setLayout(layGridLayout4);
			dialogBox.add(getAddressBar(), null);
			dialogBox.add(getUsernameBar(), null);
			dialogBox.add(getConnectBar(), null);
		}
		return dialogBox;
	}
	/**
	 * This method initializes connectDialog - this is the connection dialog box window
	 * 
	 * @return javax.swing.JDialog
	 */
	private javax.swing.JDialog getConnectDialog() {
		if(connectDialog == null) {
			connectDialog = new javax.swing.JDialog();
			connectDialog.setContentPane(getDialogBox());
			connectDialog.setSize(320, 175);
            connectDialog.setLocation(height / 3, width / 3);
		}
		return connectDialog;
	}
	/**
	 * This method initializes portField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getPortField() {
		if(portField == null) {
			portField = new javax.swing.JTextField();
			portField.setColumns(5);
		}
		return portField;
	}
	/**
	 * This method initializes addressField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getAddressField() {
		if(addressField == null) {
			addressField = new javax.swing.JTextField();
			addressField.setColumns(10);
		}
		return addressField;
	}
	/**
	 * This method initializes usernameField
	 * 
	 * @return javax.swing.JTextField
	 */
	private javax.swing.JTextField getUsernameField() {
		if(usernameField == null) {
			usernameField = new javax.swing.JTextField();
			usernameField.setColumns(10);
		}
		return usernameField;
	}
	/**
	 * This method initializes connectButton
	 * 
	 * @return javax.swing.JButton
	 */
	private javax.swing.JButton getConnectButton() {
		if(connectButton == null) {
			connectButton = new javax.swing.JButton();
			connectButton.setText("Done");
			connectButton.addActionListener(new java.awt.event.ActionListener() { 
				public void actionPerformed(java.awt.event.ActionEvent e) {    
					try {
                        // Creates an error box if the fields aren't filled out and bails out. 
                        if (addressField.getText().equals("") || usernameField.getText().equals("") || portField.getText().equals("")) {
                            JOptionPane.showMessageDialog( ChatClient.this, "Please fill out all fields", 
                            "Error", JOptionPane.ERROR_MESSAGE );
                            return;
                        } else {
                            outPacket.setAddress(InetAddress.getByName(addressField.getText())); //sets IP address of server
                        }
					} catch (UnknownHostException e1) {
						e1.printStackTrace();
					}
					outPacket.setPort(Integer.valueOf(portField.getText()).intValue());
					userName = usernameField.getText();
					send(userName + " has entered the chat room."); //sends a message to the server to indicate client has connected.
					getConnectDialog().hide();
					sendButton.setEnabled(true); //enables the send button
                    connectBox.setEnabled(false); //disables the connect button

                    // this thread listens for incoming packets and displays their contents in the program's main window.
                    readerThread = new Thread() {
                        public void run() {
                            while (true) {
                                try {
                                    socket.receive(inPacket);
                                } catch (IOException e1) {
                                    e1.printStackTrace();
                                }
                                String received = new String(inPacket.getData(), 0, inPacket.getLength());
                                textWindow.append(received + "\n");

                                // this keeps the main text window's scrollbar on the button of the window
                                textWindow.setCaretPosition(textWindow.getDocument().getLength());
                            }
                        }
                    };
                    readerThread.start(); //start thread !
				}
			});
		}
		return connectButton;
	}
	/**
	 * @param string
     * This function sends data through the socket
	 */
	protected void send(String string) {
		outPacket.setData(string.getBytes());
		try {
			socket.send(outPacket);
		} catch (IOException e) {
			e.printStackTrace();
		}
		
	}
	/**
	 * This method initializes portLabel
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getPortLabel() {
		if(portLabel == null) {
			portLabel = new javax.swing.JLabel();
			portLabel.setText("Port:");
		}
		return portLabel;
	}
	/**
	 * This method initializes addressLabel
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getAddressLabel() {
		if(addressLabel == null) {
			addressLabel = new javax.swing.JLabel();
			addressLabel.setText("Address:");
		}
		return addressLabel;
	}
	/**
	 * This method initializes usernameLabel
	 * 
	 * @return javax.swing.JLabel
	 */
	private javax.swing.JLabel getUsernameLabel() {
		if(usernameLabel == null) {
			usernameLabel = new javax.swing.JLabel();
			usernameLabel.setText("User Name:");
		}
		return usernameLabel;
	}
	/**
	 * This method initializes addressBar
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getAddressBar() {
		if(addressBar == null) {
			addressBar = new javax.swing.JPanel();
			addressBar.add(getAddressLabel(), null);
			addressBar.add(getAddressField(), null);
			addressBar.add(getPortLabel(), null);
			addressBar.add(getPortField(), null);
		}
		return addressBar;
	}
	/**
	 * This method initializes usernameBar
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getUsernameBar() {
		if(usernameBar == null) {
			usernameBar = new javax.swing.JPanel();
			usernameBar.add(getUsernameLabel(), null);
			usernameBar.add(getUsernameField(), null);
		}
		return usernameBar;
	}
	/**
	 * This method initializes connectBar
	 * 
	 * @return javax.swing.JPanel
	 */
	private javax.swing.JPanel getConnectBar() {
		if(connectBar == null) {
			connectBar = new javax.swing.JPanel();
			connectBar.add(getConnectButton(), null);
		}
		return connectBar;
	}
}
