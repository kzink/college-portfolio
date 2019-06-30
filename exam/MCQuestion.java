// File: MCQuestion.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 4, Programming Problem 1
// This is a subclass of Question specific to multiple choice questions.

/** A subclass to represent multiple choice questions
* @author Kevin Zink
*/
public class MCQuestion extends Question {
    
    private String questionList[];
    private int answer;
    
    public MCQuestion(String s, String ql[], int a, int w) {
        super(s);
        if (ql.length < 2) {
            System.exit(1);
        }
        questionList = ql;
        answer = a;
        setWeight(w);
    }
    
    public boolean ask() {
        int selection = -1;
        boolean validAnswer = false;
        char counter = 'a';
        System.out.println(getText() + "? ");
        for (int i = 0; i < questionList.length; i++) {
            System.out.println(counter + ") " + questionList[i]);
            counter++;
        }
        String input = Console.readLine("Answer:");
        
        while (!validAnswer) {
            switch (Character.toLowerCase(input.charAt(0))) {
            case 'a':
                selection = 0;
                break;
            case 'b':
                selection = 1;
                break;
            case 'c':
                selection = 2;
                break;
            case 'd':
                selection = 3;
                break;
            case 'e':
                selection = 4;
                break;
            case 'f':
                selection = 5;
                break;
            case 'g':
                selection = 6;
                break;
            default:
                selection = -1;
                break;
            }
                    
            if (selection >= 0 && selection < questionList.length)
            {
                validAnswer = true;
            } else {
                input = Console.readLine("Invalid answer; try again: ");
            }
        }

        System.out.print ("The answer you chose, " + questionList[selection] + ", ");
        if (selection == answer) {
            System.out.println ("is the correct answer.");
        } else {
            System.out.println ("is not the correct answer.");
            System.out.println ("The correct answer is: " + questionList[answer] + ".");
        }
        return (selection == answer);
    }
}
