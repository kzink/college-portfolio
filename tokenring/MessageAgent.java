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

public class MessageAgent extends Aglet {

    // NodeMessage
    private NodeMessage mess;

    /**
     * This method is invoked when the MessageAgent 
     * is created.
     *
     * @param Object init
     */
    public void onCreation(Object init) {
        MessageInit messageInit = (MessageInit)init;
        mess = messageInit.mess;

        try {
            dispatch(new URL(messageInit.address));
        }
        catch (Exception e) {
            // if the token cannot be sent, we know there is a broken link
            System.out.println("The message could not be sent to site: " + messageInit.address + " and will be disposed");
            dispose();
        }
    }

    /**
     * This method is invoked everytime a MessageAgent arrives
     * at a new Tahiti server.
     */
    public void run()
    {
        for (Enumeration e = getAgletContext().getAgletProxies(); e.hasMoreElements();) {
            // looking for a NodeAgent to try to send a message to
            // if it is not the one we are looking for, the NodeAgent
            // will tell us its other neighbor so that we may dispatch 
            // to our next correct location
            
            AgletProxy prox = (AgletProxy)e.nextElement();
            
            try {
                if (prox.getAgletClassName().equals("NodeAgent")) {
                    String returnMessage = (String) prox.sendMessage(new Message("message", mess.getVector()));

                    // wait for a reply
                    
                    if (returnMessage.equals("end")) {
                        // destination NodeAgent has been found - can dispose of the message
                        System.out.println("Message disposed!");
                        dispose();
                    } else {
                        // send message to the next site given to it in the returnMessage
                        System.out.println("The message will be dispatched to " + returnMessage);

                        String secondMessage = (String) prox.sendMessage(new Message("queryaddress", null));
                        // waiting for second reply
                        
                        mess.addStamp(secondMessage);

                        try {
                            dispatch(new URL(returnMessage));
                        }
                        catch (Exception ex) {
                            // if the token cannot be sent, we know there is a broken link
                            System.out.println("The message could not be sent to site: " + returnMessage + " and will be disposed");
                            dispose();
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
     * deprecated - no messages need to be handled by
     * the MessageAgent at this time.
     *
     * @param Message msg
     * @return boolean
     */
    public boolean handleMessage(Message msg) {
        return true;
    }
}
