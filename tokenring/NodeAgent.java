import com.ibm.aglet.*;
import java.util.Vector;
import java.io.IOException;
import java.util.Hashtable;
import java.net.InetAddress;
import java.util.Random;
import java.lang.Thread;
import java.util.Enumeration;


/** This class is spawned for each node in the Distributed Consensus Problem.
 *
 * @author Noah Anderson and Kevin Zink
 */

public class NodeAgent extends Aglet {

    private NodeMessage mess = new NodeMessage();
    private String neighbor1;
    private String neighbor2;
    public String selfAddress = null;;
    private NodeAgentFrame frame = new NodeAgentFrame(this);
    private Hashtable messagesReceived = new Hashtable();
    private Hashtable messagesSent = new Hashtable();
    private AgletProxy childProxy1 = null;
    private AgletProxy childProxy2 = null;
    private AgletProxy tokenProxy = null;
    private String name;
    private Random rnd = new Random();
    private boolean seenToken = true;
    private boolean threadStarted = false;
    private boolean receivedInquiry = false;
    private String tokenInquiryOrigin;
    private String tokenInquiryFirstHop;
    private String keepNeighbor;

    /**
     * This method is executed after an instance of NodeAgent has been
     * created and is now residing on a Tahiti server.
     */
    public void run() {
        selfAddress = this.getAgletContext().getHostingURL().toString();

        // launch the GUI aka NodeAgentFrame
        frame.pack();
        frame.loadDefault();
        frame.fireSetupButton();
        frame.show();
    }

    /**
     * This method will start the CheckForTokenThread, but only if
     * it hasn't already been started.
     */
    public void startTokenThread() {
        // launch token-checking thread
        if (threadStarted == false) {
            CheckForTokenThread tokenThread = new CheckForTokenThread();
            tokenThread.start();
            threadStarted = true;
        }
        frame.disableTokenButton(); //disable the Start Token Button in GUI
    }

    /**
     * This method will launch a new token onto the distributed network.
     * It will also try to start the startTokenThread() so that the 
     * thread can begin if it hasn't already
     */
    public void deployToken() {
        try {
            tokenProxy = getAgletContext().createAglet
                (getCodeBase(), "TokenAgent", neighbor1);
        }
        catch(Exception e){
            e.printStackTrace();
        }
        startTokenThread();  //fire up the token thread
    }
    /**
     * This method checks the local Tahiti server for a TokenInquiry
     * aglet and returns a boolean value appropriately
     *
     * @return boolean
     */
    public boolean isTokenInquiryHere() {
        boolean rval = false;
        for (Enumeration e = getAgletContext().getAgletProxies(); e.hasMoreElements();) {

            AgletProxy prox = (AgletProxy)e.nextElement();

            try {
                // if the token is present, tell it to dispose itself
                if (prox.getAgletClassName().equals("TokenInquiry")) {
                    rval = true;
                    prox.sendMessage(new Message("end", null));
                }
            }
            catch (Exception ex) {
                ex.printStackTrace();
            }
        }

        return rval;
    }
    
    /**
     * This thread will search for a TokenAgent on
     * this Tahiti server; if one is found, it will 
     * be ordered to dispose of itself.
     */
    public void disposeTokenIfPresent() {

	for (Enumeration e = getAgletContext().getAgletProxies(); e.hasMoreElements();) {

	    AgletProxy prox = (AgletProxy)e.nextElement();

	    try {
		// if the token is present, tell it to dispose itself
		if (prox.getAgletClassName().equals("TokenAgent")) {
		    prox.sendMessage(new Message("end", null));
		}
	    }
	    catch (Exception ex) {
		ex.printStackTrace();
	    }
	}
    }
    
    /**
     * This allows an external object to see the
     * name of this NodeAgent (ie NodeAgentFrame).
     *
     * @return String
     */
    public String getName() {
        return name;
    }

    /**
     * This method allows an external object to change
     * the name of the NodeAgent (ie NodeAgentFrame).
     *
     * @param String newName 
     */
    public void setName(String n) {
        name = n;
    }

    /**
     * This method parses the host out of a String 
     * representation of an atp:// style URL.
     *
     * @param String host
     * @return String
     */
    public String parseHost(String host) {
        return host.substring(6, host.indexOf(":", 4));
    }

    /** 
     * This method parses the port out of a String
     * reprsentation of an atp:// style URL.
     *
     * @param String host
     * @return String
     */
    public String parsePort(String host) {
        return host.substring(host.indexOf(":", 4) + 1, host.length() - 1);
    }

    /**
     * This method takes a hostname and returns a 
     * String representation of its IP address.
     *
     * @param String host
     * @return String
     */
    public String hostToIP(String host) {
        String rval = ""; 

        try {
            InetAddress net = InetAddress.getByName(host);

            rval = net.getHostAddress();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return rval;
    }

    /**
     * This method sets the values of the neighbor 
     * NodeAgents.
     *
     * @param String neighbor1
     * @param String neighbor2
     */
    public void setNeighbors(String n1, String n2) {
        neighbor1 = "atp://" + n1 + "/";
        neighbor2 = "atp://" + n2 + "/";
    }

    /**
     * This method retrieves the value of neighbor1.
     *
     * @return String
     */
    public String getNeighbor1() {
        return neighbor1;
    }

    /**
     * This method retrieves the value of neighbor2
     *
     * @return String
     */
    public String getNeighbor2() {
        return neighbor2;
    }
    
    /**
     * This method will compare two string representations
     * of URLs to see if they have the same actual IP and port
     *
     * @param String host1
     * @param String host2
     * @return boolean
     */
    public boolean compareAddresses(String host1, String host2) {

        boolean rval = false;

        String h1 = hostToIP(parseHost(host1)) + ":" + parsePort(host1);
        String h2 = hostToIP(parseHost(host2)) + ":" + parsePort(host2);

        if (h2.equals(h1)) {
            rval = true;
        }
        return rval;
    }

    /**
     * This method handles a wide variety of messages
     * that can be received from other Aglets that visit
     * the Tahiti server that runs this NodeAgent.
     *
     * @param Message msg
     * @return boolean
     */
    public boolean handleMessage(Message msg) {

        String msgKind = msg.getKind(); //get message kind

        if (msgKind.equals("message")) {
            Vector tmpVec = (Vector) msg.getArg();
            NodeMessage tmpMsg = new NodeMessage(tmpVec);

            // If this server is the intended destination for the message, we update the GUI and dispose of the message
            if (tmpMsg.getDestination().equals(name)) {

                msg.sendReply(new String("end")); //tell message to dispose itself

                // update GUI with newly received message if it is not a duplicate
                if (messagesReceived.containsKey(tmpMsg.getMessageId())) {
                    // disregard the duplicate
                    System.out.println("Duplicate detected - doing nothing");
                } else {
                    frame.append(tmpMsg.getText());
                    System.out.println("Haven't seen this message yet: updating GUI chat");

                    // put message ID into hastable
                    messagesReceived.put(tmpMsg.getMessageId(), tmpMsg.getSource());
                }

            } else {

                // If the server receives a message that it sent, we will assume the destination of the address did not
                // exist, so we'll dispose of this message
                if (messagesSent.containsKey(tmpMsg.getMessageId())) {
                    msg.sendReply(new String("end"));

                } else {
                    // send reply with the next neighbor address

                    Vector stampVec = new Vector();
                    stampVec = tmpMsg.getAddressStamp();

                    // compare last element site in message stamp to neighbor sites and send to respective neighbor
                    if (compareAddresses( (String)stampVec.lastElement(), neighbor1)) {
                        // send to neighbor2
                        System.out.println("sending this message to neighbor 2");
                        msg.sendReply(new String(neighbor2));
                    } else {
                        //send to neighbor1
                        System.out.println("sending this message to neighbor 1");
                        msg.sendReply(new String(neighbor1));
                    }
                }
            }
            System.out.println("-----------------Message process completed-----------------");

        } else if (msgKind.equals("queryaddress")) {
            msg.sendReply(new String(selfAddress));

        } else if (msgKind.equals("token")) {
            // This passes on the token to the next neighbor

            // Compare last site listed in tokenagent to neighboring sites send send to respective neighbor
            if (compareAddresses( (String)msg.getArg(), neighbor1)) {
                // send to neighbor2
                System.out.println("found TokenAgent and I'm sending it to neighbor 2");
                msg.sendReply(neighbor2);
            } else {
                //send to neighbor1
                System.out.println("found TokenAgent and I'm sending it to neighbor 1");
                msg.sendReply(neighbor1);
            }

            seenToken = true; //toggle boolean
            receivedInquiry = false; //reset inquiry flag

            startTokenThread(); //start the token thread

        } else if (msgKind.equals("tokeninquiry")) {

            disposeTokenIfPresent();

            // compare addresses and send to respective neighbor
            if (compareAddresses( (String)msg.getArg(), neighbor1)) {
                // send to neighbor2
                System.out.println("found TokenInquiry and I'm sending it to neighbor 2");
                msg.sendReply(neighbor2);
            } else {
                //send to neighbor1
                System.out.println("found TokenInquiry and I'm sending it to neighbor 1");
                msg.sendReply(neighbor1);
            }

            receivedInquiry = true; //toggle flag

        } else if (msgKind.equals("updateagent")) {

            keepNeighbor = (String)msg.getArg();
            if ( isTokenInquiryHere() ) {
                msg.sendReply("neighborinquiry");
                
            } else if (compareAddresses( keepNeighbor, neighbor1)) {
                // send to neighbor2
                System.out.println("found UpdateAgent and I'm sending it to my neighbor2: " + neighbor2);
                msg.sendReply(neighbor2);
            } else {
                // send to neighbor1
                System.out.println("found UpdateAgent and I'm sending it to my neighbor1: " + neighbor1);
                msg.sendReply(neighbor1);
            }
            disposeTokenIfPresent();

        } else if (msgKind.equals("newneighbor")) {

            String newNeighbor = (String)msg.getArg();

            System.out.println("About to compare keepNeighbor: " + keepNeighbor + " to neightbor1: " + neighbor1);
            if (compareAddresses(keepNeighbor, neighbor1)) {
                // send to neighbor2
                System.out.println("I have received repair info and I'm making my neighbor2 " + newNeighbor);
                neighbor2 = newNeighbor;
                frame.setNeighborTwo(parseHost(neighbor2) + ":" + parsePort(neighbor2)); //update GUI with new neighbor
                msg.sendReply("end");
            } else {
                //send to neighbor1
                System.out.println("I have received repair info and I'm making my neighbor1 " + newNeighbor);
                neighbor1 = newNeighbor;
                frame.setNeighborOne(parseHost(neighbor1) + ":" + parsePort(neighbor1)); //update GUI with new neighbor
                msg.sendReply("end");
            }

	    // ensure that only one of the two UpdateAgents triggers a new Token
            if (selfAddress.compareTo(newNeighbor) > 0) {
                try {
                    Thread.sleep(500); //sleep for 1/2 a second
                }
                catch (Exception e) {
                    e.printStackTrace();
                }

                deployToken();
            }
        }
        return true;
    }

    /**
     * This method will launch a MessageAgent and set
     * it up with all the information it needs.
     *
     * @param String text
     * @param String dest
     */
    public void deployMessageAgent(String text, String dest) {

        // Build NodeMessage
        mess.setText(text);
        mess.setDestination(dest);
        mess.addStamp(selfAddress);

	// pseudorandom number for uniqueness
        String ID = Long.toString(rnd.nextLong());

        // This generates a message ID via the random number generator
        mess.setMessageId(ID);

        // Add message ID to hashtable
        messagesSent.put(ID, "");

	// we'll need to create 2 MessageAgents, one for each direction in the ring
	// true to the spirit of "flooding"
        try{
            // Build MessageAgent 1
            childProxy1 = getAgletContext().createAglet
                (getCodeBase(), "MessageAgent", new MessageInit(mess, neighbor1));

            // Build MessageAgent 2
            childProxy2 = getAgletContext().createAglet
                (getCodeBase(), "MessageAgent", new MessageInit(mess, neighbor2));
        }
        catch(Exception e){
            e.printStackTrace();
        }

        // This launches a thread to wait for acknowledgements
        WaitForAckThread t = new WaitForAckThread(ID);
        t.start();
    }

    /**
     * This class is a thread that waits to see if an ACK has been
     * received for a message recently sent
     */
    class WaitForAckThread extends Thread {

        String ID = "";

        WaitForAckThread(String ID) {
            super();
        }

        public void run() {
            try {
                this.sleep(500); // sleep for .5 seconds
            } 
            catch (Exception e) {
                e.printStackTrace();
            }
            // check to see if an ACK has been received for the specified ID

            // if it has been received do nothing

            // else something about tokens
        }

    }

    /**
     * This thread is launched to periodically check 
     * how long it has been since the token was last there.
     * If two consequtive checks occur without a token visiting,
     * TokenInquiry Aglets will be sent out to discover the 
     * network fault and then repair the situation
     */
    class CheckForTokenThread extends Thread {

	CheckForTokenThread() {
	    super();
	}

	public void run() {
	    System.out.println("CheckForTokenThread.run() loop has begun");
	    while (true) {
		try {
		    this.sleep(13000); // sleep for 13 seconds

		    //Checking for the token flag

		    if (seenToken == false) {
			//I haven't seen the token in a blue moon :~(

			if (receivedInquiry == false) {
			    // Send out TokenInquiries via Flooding
			    System.out.println("Making TokinInquiryAgents and sending them to sites: " + neighbor1 + " and " + neighbor2);
			    childProxy1 = getAgletContext().createAglet
				(getCodeBase(), "TokenInquiry", neighbor1);

			    childProxy2 = getAgletContext().createAglet
				(getCodeBase(), "TokenInquiry", neighbor2);

			    System.out.println("Making TokinInquiryAgents and sending them to sites: " + neighbor1 + " and " + neighbor2);

			    receivedInquiry = true;
			}

		    } else {
			// everything is ok but we toggle the flag off again
			seenToken = false;
		    }
		}
		catch (Exception e) {
		    e.printStackTrace();
		}
	    }
	}
    }
}
