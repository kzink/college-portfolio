/** A main program to text the Exam class.  Provided as part of Assignment 4
  * for CSCI 2830, University of Nebraska at Omaha, Fall 2003
  *
  * @author Mark Pauley
  */
public class ExamDemo {
    /** The main program: creates a short exam and administers it.
      * @param An array of strings to accept command-line parameters.
      */
    public static void main(String args[]) {
    // Create an exam worth a total of ten points: eight one-point questions
    // and one two-point question
    Exam myExam = new Exam();
    myExam.addQuestion(new TFQuestion("The capital of the state of " +
                                      "Nebraska is Lincoln", true, 1));
    String PopChoices[] = {"Florida", "California", "Rhode Island", "Nevada",
                           "Michigan"};
    myExam.addQuestion(new MCQuestion("What U.S. state has the " +
                                      "largest population",
                                      PopChoices, 1, 1)); 
    myExam.addQuestion(new SAQuestion("What U.S. city has the " +
                                      "nickname \"The Windy City\"",
                                      "Chicago", 1));
    myExam.addQuestion(new TFQuestion("There are 51 states in the U.S.",
                                      false, 1));
    String BCchoices[] = {"Phoenix", "Flagstaff", "Parker"};
    myExam.addQuestion(new MCQuestion("What is the capital of " +
                                      "Arizona", BCchoices, 0, 1));
    myExam.addQuestion(new SAQuestion("Which U.S. state has the " +
                                      "smallest population",
                                      "Wyoming", 1));
    myExam.addQuestion(new SAQuestion("Which U.S. state is the " +
                                      "farthest west", "Alaska", 2));
    myExam.addQuestion(new TFQuestion("The Rocky Mountains are in the " +
                                      "eastern United States", false, 1));
    String PEIChoices[] = {"Cedar Key, FL", "New York City, NY",
                           "Charlotte, NC", "Eastport, ME"};
    myExam.addQuestion(new MCQuestion("What is the easternmost city " +
                                      "in the U.S.", PEIChoices, 3, 1));

    // give the exam and report the score
    int score = myExam.giveExam();
    System.out.println("You scored a " + score + "% on the exam!");
  } // end main()
} // end class ExamDemo

