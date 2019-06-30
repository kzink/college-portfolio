// File: SchedulerContainer.java

// Kevin R. Zink, ID 3541
// CSCI 3830, Spring 2003
// Programming Problem 1
// This class contains a Container for the Scheduling program.

/** This is a container class built specifically for the Scheduler program.
 * @author Kevin Zink
 */
public class SchedulerContainer {

    private static int totalDays = 31 * 12 + 1; //31 days per month, 12 months, day 0 is ignored
    private static int totalCols = 24 * 4; //24 rows per column, 4 columns per table.
    private String days[][] = new String[totalDays][totalCols]; //This is an array of Strings 373 by 96

    /** Default constructor.
     * Sets all Strings in the Strings array to be empty.
     */
    public SchedulerContainer() {
        for(int i = 0; i < totalDays; ++i) {
            for (int j = 0; j < totalCols; ++j) {
                days[i][j] = "";
            }
        }
    }
    
    /** Sets a String of a particular month, day, row, and column.
     */
    public void setDay(String aString, int month, int day, int row, int col) {
        if (month > 11 || month < 0 || day < 1 || day > 372 || col < 0 || col > 3 || row < 0 || row > 23) {
            System.out.println("out of bounds in setDay() somewhere");
        } else {
            days[31 * month + day][col * 24 + row] = aString;
        }
    }

    /** Gets a String of a particular month, day, row, and column.
     */
    public String getDay(int month, int day, int row, int col) {
        if (month > 11 || month < 0 || day < 1 || day > 372 || col < 0 || col > 3 || row < 0 || row > 23) {
            System.out.println("out of bounds in getDay() somewhere");
            return "";
        }
        return days[31 * month + day][col * 24 + row];
    }

    /** Gets a String of a particular day, row, and column.  
     * Differs from getDay() by indexing into the String array with int day, which can range from 1 - 373.
     */
    public String getDayDirect(int day, int row, int col) {
        if (day < 1 || day > 372 || col < 0 || col > 3 || row < 0 || row > 23) {
            System.out.println("out of bounds in getDayDirect() somewhere");
            return "";
        }
        return days[day][col * 24 + row];
    }
    
    /** Sets a String of a particular day, row, and column.  
     * Differs from setDay() by indexing into the String array with int day, which can range from 1 - 373.
     */
    public void setDayDirect(String aString,  int day, int row, int col) {
        if (day < 1 || day > 372 || col < 0 || col > 3 || row < 0 || row > 23) {
            System.out.println("out of bounds in setDayDirect() somewhere");
        } else {
            days[day][col * 24 + row] = aString;
            //System.out.println(day + " " + col + " " + row + " " + aString);
        }
    }
}
