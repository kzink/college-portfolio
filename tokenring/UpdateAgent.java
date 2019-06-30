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
public class UpdateAgent extends Aglet {

    String newNeighbor = "";
    String nextSite = "";
    String prevSite = "";

    /** 
     * This method is invoked at creation
     *
     * @param Object init
     */
    public void onCreation(Object init) {
        nextSite = (String) init;
        try {
            newNeighbor = getProxy().getAddress();
            prevSite = newNeighbor;
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }

        try {
            dispatch(new URL(nextSite));
        }
        catch (Exception ex) {
            System.out.println("UpdateAgent could not dispatch to site: " + nextSite);
        }
    }

    /**
     * This method is invoked when it arrives
     * at a new Tahiti server.
     */
    public void run()
    {
        try {
            Thread.sleep(1000); // sleep for 1 second
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }

        for (Enumeration e = getAgletContext().getAgletProxies(); e.hasMoreElements();) {
            // looking for a NodeAgent to try to send a message to
            // it, trying to find a tahiti server with a stuck
	    // TokenInquiry to indicate where neighbor updates
	    // need to occur.
	     
            AgletProxy prox = (AgletProxy)e.nextElement();
            
            try {
                if (prox.getAgletClassName().equals("NodeAgent")) {
                    
                    nextSite = (String) prox.sendMessage(new Message("updateagent", prevSite));

                    if (nextSite.equals("neighborinquiry")) {
                        nextSite = (String) prox.sendMessage(new Message("newneighbor", newNeighbor));
                        System.out.println("UpdateAgent has sent newNeighbor info to the NodeAgent and will be disposed");
                        dispose();
                    } else {
                        try {
                            prevSite = getProxy().getAddress();
                        }
                        catch (Exception ex) {
                            ex.printStackTrace();
                        }

                        System.out.println("UpdateAgent came from site: " + prevSite);
                        System.out.println("Attempting to dispatch UpdateAgent to site: " + nextSite);

                        try {

                            dispatch(new URL(nextSite));
                        }
                        catch (Exception ex) {
                            System.out.println("UpdateAgent could not dispatch to site: " + nextSite);
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
     * other Aglets
     *
     * @param Message msg
     * @return boolean
     */
    public boolean handleMessage(Message msg) {

        boolean returnVal = true;

        String msgKind = msg.getKind();
        if (msgKind.equals("end")) {
            System.out.println("nobody likes me, i'll go eat worms");
            dispose();
        }

        return returnVal;
    }
}
