// File: Exam.java

// Kevin R. Zink, ID 3541
// CSCI 2830, Fall 2003
// Assignment 4, Programming Problem 1
// This class stores a collection of Questions as an exam.

/** This class stores a series of Questions
* @author Kevin Zink
*/
public class Exam {
    
    private Question questions[]; 
    private int count;
    
    /** Class Constructor.  Makes an empty questions array.
     */
    public Exam() {
        questions = new Question[100];
        count = 0;
    }

    /** Adds a question to the array.
     */
    public void addQuestion(Question q) {
        questions[count] = q;
        count++;
    }

    /** Gives the user the exam and returns their score.
     */
    public int giveExam() {
        int total = 0;
        int userScore = 0;
        for (int i = 0; i < count; i++) {
            total += questions[i].getWeight();
            if (questions[i].ask()) {
                userScore += questions[i].getWeight();
            }
            System.out.println();
        }
        return (userScore * 100 / total);
    }
}
