/** 
 * This class is used to transfer multiple
 * values to a new MessageInit at creation
 *
 * @author Noah Anderson and Kevin Zink
 */
public class MessageInit
{
    public NodeMessage mess;
    public String address;

    /**
     * Constructor
     *
     * @param NodeMessage message
     * @param String address
     */
    MessageInit(NodeMessage mess, String address)
    {
        this.mess = mess;
        this.address = address;
    }
}
