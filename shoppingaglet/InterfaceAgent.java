// Name: Kevin Zink
// CSCI: 3830
// Filename: InterfaceAgent.java

// This Aglet represents a seller sites.  It inventories a list of items
// from a file and communicates with the Shopping Aglet.  Some of this code was taken from the 
// example Aglets program we were provided.

import com.ibm.aglet.*;
import java.util.Vector;
import java.io.IOException;
import java.util.Hashtable;
import javax.swing.JFileChooser;
import javax.swing.JFrame;
import java.io.*;

/** This class creates a child aglet called ShoppingAgent
 * and sends it out to multiple remote sites. 
 */
public class InterfaceAgent extends Aglet{

    private Hashtable table = new Hashtable();
    private BufferedReader in;

    /* The run method
     */
    public void run() {
    }

    /** The Oncreate method
     *  This method launches a JFileChooser to select an inventory file
     */
    public void onCreation(java.lang.Object init) {

        JFrame frame = new JFrame();
        JFileChooser jFC = new JFileChooser();

        jFC.setFileSelectionMode(JFileChooser.FILES_ONLY);
        int result = jFC.showOpenDialog(frame);

        // If the cancel selection is chosen from the JFileChooser, the Aglet will be simply dispose itself
        if (result == jFC.CANCEL_OPTION) {
            InterfaceAgent.this.dispose();
        }

        File filename = jFC.getSelectedFile();

        try {
            in = new BufferedReader(new FileReader(filename));
            String line;

            // Reads in from the file one line at a time and parses it using tab delimited format.
            while ((line = in.readLine()) != null) {
                String patternStr = "\t";
                String[] fields = line.split(patternStr);

                table.put(fields[0], Integer.valueOf(fields[1])); //enters pair into the hash table
            }

            in.close(); //close filestream

        } catch (IOException x) {
            x.printStackTrace();
        }
    }

    /** handle messages sent from ShoppingAgent.
     */
    public boolean handleMessage(Message msg){
        String msgKind = msg.getKind();
        Object value = table.get(msgKind);

        // Send back the corresponding value to the query (the table key) if it exists
        if (value != null) {
            msg.sendReply( ((Integer) value).intValue() ); 
        
        // send back -1 if the key was not found
        } else {
            msg.sendReply( (int) -1 ); 
        }

        return true;
    }
}
