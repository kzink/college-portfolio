// File: ChatServer.java
// Author: Kevin Zink
// Programming problem #2, part 2
// Created on Feb 20, 2004
//
// This is a chat client to be used with TalkClient.java
// This program implements Datagram packets as a method of transporting data.
// This server works by listening for packets on a specified port.  When it receives a packet from an unknown host, it adds
// that host its subsribers list and then forwards the packet to all subscribers.  This program also has a rudimentary method of
// removing subscribers from its list.

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketAddress;
import java.util.Vector;

/** ChatServer
 * @author Kevin Zink
 *
 */
class ChatServer {

	static DatagramSocket socket = null;    //socket
	static byte[] buf = new byte[256];      //buffer used to store packet information
	static DatagramPacket packet = new DatagramPacket(buf, buf.length); //Datagram packet
	static Vector addresses = new Vector(); //Vector that stores all clients' IP addresses and ports
    
	public static void main(String[] args) {
		try {
			socket = new DatagramSocket();
			System.out.println("TalkServer listening on rendezvous port: " 
								+ socket.getLocalPort());
		} catch (IOException e) {
			System.err.println("Error creating socket");
			return;
		}

        // This loop listens for packets until Control-C is initiated.
		while (true) {
			try {
				socket.receive(packet);
			} catch (IOException e1) {
				e1.printStackTrace();
			}
            
            // If a packet is received from an unknown address, the address and port is added to the subscriber list
			if (!addresses.contains(packet.getSocketAddress())) {
				addresses.add(packet.getSocketAddress());
			}

            // If a packet is received that contains the string "has left the chat room." That client is removed from the subscriber list
            String data = new String(packet.getData(), 0, packet.getLength());
            if (data.endsWith("has left the chat room.")) {
                addresses.remove(packet.getSocketAddress());
            }
                
            // This loop sends the packet to all known clients.
			for (int i = 0; i < addresses.size(); ++i) {
				packet.setSocketAddress((SocketAddress) addresses.elementAt(i));
				try {
					socket.send(packet);
				} catch (IOException e2) {
					e2.printStackTrace();
				}
			}
		}
	}
}
