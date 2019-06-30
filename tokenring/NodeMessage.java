import java.util.Vector;
import java.io.Serializable;

/**
 * This class stores the information needed for
 * the MessageAgent.
 *
 * @author Noah Anderson and Kevin Zink
 */
public class NodeMessage implements Serializable {
    
    public Vector everything = new Vector();
    /*****************
     *  Element Key  *
     *****************
     * 0 - text
     * 1 - messageId
     * 2 - destination
     * 3..n - addressStamp
     */
    /**
     * Default constructor
     */
    public NodeMessage() {
        for (int i = 0; i < 3; i++) {
            everything.add("");
        }
    }

    /**
     * Constructor
     *
     * @param Vector v
     */
    public NodeMessage(Vector v) {
        for (int i = 0; i < v.size(); i++) {
            everything.add(v.elementAt(i));
        }
    }

    /**
     * This method returns a vector that contains
     * all useful information.
     *
     * @return Vector
     */
    public Vector getVector() {
        return everything;
    }

    /**
     * This method finds the source of the datatype.
     *
     * @return String
     */
    public String getSource() {
	return (String)everything.elementAt(3);
    }

    /**
     * This method adds a stamp to the class
     *
     * @param String stamp
     */
    public void addStamp(String stamp) {
	everything.add(stamp);
    }

    /**
     * This method returns a vector of stamps
     *
     * @return vector
     */
    public Vector getAddressStamp() {
	Vector tmpvec = new Vector();
	for (int i = 3; i < everything.size(); i++) {
	    tmpvec.add(everything.elementAt(i));
	}
	return tmpvec;
    }

    /**
     * Setter of text
     *
     * @param String t
     */
    public void setText(String t) {
	everything.setElementAt(t, 0);
    }

    /**
     * Getter of text
     *
     * @return String
     */
    public String getText() {
	return (String)everything.elementAt(0);
    }

    /**
     * Setter of MessageId
     *
     * @param String m
     */
    public void setMessageId(String m) {
	everything.setElementAt(m, 1);
    }

    /**
     * Getter of the MessageId
     *
     * @return String
     */
    public String getMessageId() {
	return (String)everything.elementAt(1);
    }

    /**
     * Setter of the destination
     *
     * @param String d
     */
    public void setDestination(String d) {
	everything.setElementAt(d, 2);
    }

    /**
     * Getter of the destination
     *
     * @return String
     */
    public String getDestination() {
	return (String)everything.elementAt(2);
    }
}
