// File: Question.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 4, Programming Problem 1
// This class is an abstract class for the elements common to all kinds of questions.

/** A class to store general information about an Exam question
* @author Kevin Zink
*/
public abstract class Question {
    
    private int weight;
    private String text; 
    
    /** Class Constructor.  Sets the String text to the String s.
     */
    public Question(String s) {
        text = s;
    }

    /** Sets the weight.
     */
    public void setWeight(int w) {
        if (w < 1) {
            System.exit(1);
        }
        weight = w;   
    }

    /** Gets the weight.
     */
    public int getWeight() {
        return weight;
    }

    /** Gets the text from the question.
     */
    public String getText() {
        return text;
    }

    /** Abstract method which administers the question, takes in the answer, and reports whether it was correct. 
     */
    public abstract boolean ask();
}
