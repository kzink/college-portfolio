// Name: Kevin Zink
// CSCI: 3830
// Filename: ShoppingAgent.java

// This file represents a Shopping aglet which is passed between 
// "Seller" Tahiti servers.  It is somewhat based on the example Aglet's program
// provided to us online.

import com.ibm.aglet.*;
import java.util.Vector;
import java.util.Enumeration;
import com.ibm.agletx.util.*;
import java.net.URL;
import java.net.MalformedURLException;

/** This class is created by the Stationary aglet.
 * It is then sent on an itinerary of remote sites
 * using the SeqPlanItinerary class.
 */
public class ShoppingAgent extends Aglet{

    // the list of remote sites
    Vector remoteSiteList;
    // the SeqPlanItinerary class
    SeqPlanItinerary itin;
    // the AgletProxy of the parent aglet
    AgletProxy parentProxy;
    // Query
    String query;
    Message msg;
    int bestOfferYet = -1;
    String bestOfferYetURL;

    // extract the init parameter and instantiate
    // and initialize the SeqPlanItinerary object
    public void onCreation(Object init){
        parentProxy = (AgletProxy)init;

    
            remoteSiteList = ((ControllerAgent)parentProxy.getAglet()).getRemoteSiteList(); 
            query = ((ControllerAgent)parentProxy.getAglet()).getQuery();
        } 
        catch(InvalidAgletException iae){
            iae.printStackTrace();
        }

        // initialize the SeqPlanItinerary object
        createItin(remoteSiteList);
    }

    /** the run method which is invoked upon arrival at any tahiti server
     */
    public void run() {

        Enumeration proxies = getAgletContext().getAgletProxies();

        // query interface agent by getting aglet proxy and searching for interface agent
        // once interface agent is found, send a message to query the server.
        while (proxies.hasMoreElements()) {

            AgletProxy currProxy = (AgletProxy) proxies.nextElement();

            try {
                //test to see if this proxy is the InterfaceAgent
                if (currProxy.getAgletClassName().equals("InterfaceAgent")) {

                    // create message and send message
                    try {
                        int value = ((Integer) currProxy.sendMessage(new Message(query))).intValue();

                        // if value is < 0, nothing was found
                        if (value < 0 ) {
                        // otherwise, we set the best offer yet
                        } else {
                            // if no best offer has been found yet, we set it
                            if (bestOfferYet == -1) {
                                bestOfferYet = value;
                                bestOfferYetURL = this.getAgletContext().getHostingURL().toString();
                            // otherwise we compare the found value to the best offer yet and alter it if necessary
                            } else if (value < bestOfferYet) {
                                bestOfferYet = value;
                                bestOfferYetURL = this.getAgletContext().getHostingURL().toString();
                            }
                        }
                    } 
                    catch (Exception e) {
                        System.out.println("Can't send message");
                        e.printStackTrace();
                    }
                }
            }
            catch (InvalidAgletException iae) {
                System.out.println("Error invoking getAgletClassName()");
            }
        }
    }

    private void createItin(Vector parmRemoteSiteList){
        // The constructor of SeqPlanItinerary takes
        // its associated aglet. When the aglet arrives
        // on a remote site, the SeqPlanItinerary object
        // sends a message to the aglet. The functionality
        // of the aglet on each remote site therefore has 
        // to go inside handleMessage for that site's message
        // and not inside the aglet's run method

        itin = new SeqPlanItinerary(this);
        for (int i = 0; i < parmRemoteSiteList.size(); i++){
            String remoteSite=(String)parmRemoteSiteList.elementAt(i);
            itin.addPlan(remoteSite, "RemoteSite" + i);
        }

        String homeURLString = this.getAgletContext().getHostingURL().toString();

        // We want to send the aglet back to its home at the end
        // of its itinerary. So we add the homeURLString as the
        // last site on the itineraryb
        itin.addPlan(homeURLString, "home");

        // send the aglet out on its itinerary
        itin.startTrip();
    }

    /** handle messages sent to the aglet. The SeqPlanItinerary object
     * called itin sends messages to the aglet when the aglet arrives
     * on a remote site
     */
    public boolean handleMessage(Message msg){

        boolean returnVal = true;

        String msgKind = msg.getKind();
        
        if (msgKind.substring(0,3).equals("Rem")){
            // aglet is on some remote site
            // extrace the remote site number
            char siteNum = msgKind.charAt(10);
            //print("I am at Remote Site #"+siteNum);
            try{
                parentProxy.sendMessage(new Message("Hello", 
                            "MobileAglet is on RemoteSite #"+siteNum));
            }
            catch(Exception e){
                // for now I am just catching all exception types here
                e.printStackTrace();
            }
        }
        else if (msgKind.equals("home")) {
            System.out.println("\n\nSearch Completed.");
            if (bestOfferYet == -1) {
                System.out.println("No Item could be found using the search string: " + query + ".");
            } else {
                System.out.println("Lowest Value Found for " + query + " was $" + (float)bestOfferYet / 100 + " from site " + bestOfferYetURL);
            }

            dispose();

        } else returnVal = false;

        return returnVal;
    }
}
