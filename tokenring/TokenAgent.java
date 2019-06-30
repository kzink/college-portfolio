import java.util.Enumeration;
import com.ibm.agletx.util.*;
import com.ibm.aglet.*;
import java.net.URL;
import java.net.MalformedURLException;

/** This class is created within a NodeAgentFrame to send a
 * token amongst the hosts to detect faults.
 *
 * @author Noah Anderson and Kevin Zink
 */
public class TokenAgent extends Aglet {

    String prevSite = "";
    String currSite = "";

    /**
     * This method is invoked at creation
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

        try {
            dispatch(new URL(currSite));
        }
        catch (Exception e) {
            // if the token cannot be sent, we know there is a broken link
            System.out.println("The Token Could not be sent to site: " + currSite);
        }
    }

    /**
     * This method is invoked when it arrives on
     * a new Tahiti server
     */
    public void run() {
        try {
            Thread.sleep(1500); // sleep for 1.5 seconds
        }
        catch (Exception ex) {
            ex.printStackTrace();
        }

        for (Enumeration e = getAgletContext().getAgletProxies();
                e.hasMoreElements();) {
            // looking for the NodeAgent so we can contact it
            AgletProxy prox = (AgletProxy)e.nextElement();
            try {
                if (prox.getAgletClassName().equals("NodeAgent")) {
                    Object reply = prox.sendMessage(new Message("token", prevSite));

                    // wait for reply in form of next hop
                    prevSite = currSite;
                    currSite = (String) reply;

                    try {
                        dispatch(new URL(currSite));
                    }
                    catch (Exception ex) {
                        // if the token cannot be sent, we know there is a broken link
                        System.out.println("The Token Could not be sent to site: " + currSite);
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
            System.out.println("TokenAgent was told to dispose itself - disposing!");
            dispose();
        }

        return returnVal;
    }
}
