//Kevin Zink
//mg004
//class List header file

#include"word.h"

class Node              //class Node
{
    public:             
        Word data;      //contains the data for the Node
        Node *next;     //points to the next Node in the chain
        
        //This is the constructor for the node class. It simply, as a default,
        //makes the data of the Node 0 and makes the next Node in the chain NULL
        //intput: data to be put into Node and a pointer to the next Node.
        //output: none
        //return: none
        Node(Word d, Node*N=NULL)  
        {                            
            data = d;               
            next=N;
        }
};

class List          //definition for class List
{
    public:
        List(void);     //constructor & destructor
        ~List(void);
        
        void insert(Word);   //functions to insert & remove elements from List
        void remove(Word);
        Word remove(void);
        Word getitem(int);   //retrieves the nth element of the list
        bool empty(void) const; //self-explanatory bool
        
        bool find(Word) const;   //determines if an element is present in List
        
        void printnum(void) const; //outputs the entire list
        
        
    private:
        Node* head;     //Node that points to the "head" of the list
        
        //disabling of implicit copy/assignment constructors
        List(const List&);  
        const List& operator=(const List&);
};
