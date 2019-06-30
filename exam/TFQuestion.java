// File: TFQuestion.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 4, Programming Problem 1
// This is a subclass of Question specific to TF questions.

/** A subclass to represent TF questions
* @author Kevin Zink
*/
public class TFQuestion extends Question {
    
    private boolean answer;
    
    public TFQuestion(String s, boolean a, int w) {
        super(s);
        setWeight(w);
        answer = a;
    }
    
    public boolean ask() {
        boolean completedAnswer = false; 
        boolean input = false; 
        System.out.println(getText() + "? (t)rue or (f)alse");
        String temp = Console.readLine("Answer:");

        while (completedAnswer == false) {
            if (temp.equalsIgnoreCase("t")) {
                input = true;
                completedAnswer = true;
            } else if (temp.equalsIgnoreCase("f")) {
                input = false;
                completedAnswer = true;
            } else {
                temp = Console.readLine("Invalid Answer; try again: ");
            }
        } 

        System.out.print("The answer you chose, " + input + ", ");
        if (input == answer) {
            System.out.println("is the correct answer.");
            return true;
        } else {
            System.out.println("is not the correct answer.");
            System.out.println("The correct answer is " + answer + ".");
            return false;
        }
    }
}
