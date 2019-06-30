// File: Scheduler.java

// Kevin R. Zink, ID 3541
// CSCI 3830, Spring 2003
// Programming Problem 1
// This class contains the class a calendar-based Scheduler program.

/** This class contains a fully functional Scheduling program.
* @author Kevin Zink
*/
public class Scheduler {
    /** Default constructor.
     * 
     */
    public Scheduler() {
    }

    /** Main function.
     * This creates the frame for the Scheduler.
     */
    public static void main(String args[]) {
        SchedulerFrame tframe= new SchedulerFrame();
        tframe.show();
    }
}
