// File: SAQuestion.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 4, Programming Problem 1
// This is a subclass of Question specific to short answer questions.

/** A subclass to represent short answer questions
* @author Kevin Zink
*/
public class SAQuestion extends Question {
    
    private String answer;
    
    public SAQuestion(String s, String a, int w) {
        super(s);
        setWeight(w);
        answer = a;
    }
    
    public boolean ask() {
        boolean input; 
        System.out.println(getText() + "? ");
        String temp = Console.readLine("Answer:");

        System.out.print("The answer you chose, \"" + temp + ",\" ");
        if (temp.equalsIgnoreCase(answer)) {
            System.out.println("is the correct answer.");
            input = true;
        } else {
            System.out.println("is not the correct answer.");
            System.out.println("The correct answer is " + answer + ".");
            input = false;
        }
        return (input);
    }
}
