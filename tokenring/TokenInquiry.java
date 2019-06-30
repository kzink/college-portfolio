import java.util.Enumeration;
import com.ibm.agletx.util.*;
import java.net.URL;
import java.net.MalformedURLException;
import java.util.Vector;
import com.ibm.aglet.*;

/** This class is created within a NodeAgentFrame to send a
 * message to another frame.
 *
 * @author Noah Anderson and Kevin Zink
 */

public class TokenInquiry extends Aglet {

    String prevSite = "";
    String currSite = "";
    private AgletProxy updateAgent = null;

    /**
     * Thid method is invoked at creation
     *
     * @param Object init
     */    
    public void onCreation(Object init) {
        prevSite = (String) init;
        try {
            currSite = getProxy().getAddress();
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }
    }

    /**
     * This method is invoked when it arrives at
     * a new Tahiti server
     */
    public void run() {
        try {
            Thread.sleep(500); // sleep for 1/2 second
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }

        for (Enumeration e = getAgletContext().getAgletProxies(); e.hasMoreElements();) {
            // looking for a NodeAgent to try to send a message to
	    // Look for next hop

            AgletProxy prox = (AgletProxy)e.nextElement();

            try {
                if (prox.getAgletClassName().equals("NodeAgent")) {
                    String returnMessage = (String) prox.sendMessage(new Message("tokeninquiry", prevSite));

                    if (returnMessage.equals("end")) {
                        // destination NodeAgent has been found - can dispose of the message
                        System.out.println("TokenInquiry has been told to dispose itself!");
                        dispose();
                        
                    } else {
                        //Send the inquiry to the next site
                        String lastSiteVisited = prevSite; //the site this Aglet came from
                        prevSite = currSite; //the current site this Aglet is at
                        currSite = (String) returnMessage; //the next Site to visit
                        
                        //System.out.println("Dispatching TokenInquiry to site: " + currSite);

                        try {
                            dispatch(new URL(currSite));
                        }
                        catch (Exception ex) {
                            // if the token cannot be sent, we know there is a broken link
                            System.out.println("Token inquiry could not be sent to: " + currSite + " so I will spawn an update agent");

                            // this creates an UpdateAgent that is sent to the previous site from which the inquiry was from
                            updateAgent = getAgletContext().createAglet
                                (getCodeBase(), "UpdateAgent", lastSiteVisited);
                        }
                    }
                }
            }
            catch (Exception ex) {
                ex.printStackTrace();
            }
        }
    }

    /**
     * This method handles incoming messages from
     * other Aglets.
     *
     * @param Message msg
     * @return boolean
     */
    public boolean handleMessage(Message msg) {

        boolean returnVal = true;

        String msgKind = msg.getKind();
        if (msgKind.equals("end")) {
            System.out.println("TokenInquiry has been told to dispose itself!");
            dispose();
        }

        return returnVal;
    }
}
