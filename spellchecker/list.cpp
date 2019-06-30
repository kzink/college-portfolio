//Kevin Zink
//mg004
//class List compilation file

#include <iostream.h>
#include"list.h"

//List class constructor.  Sets head to point to NULL
//List::List(void)
//input: none
//output: none
//return: none
List::List(void)
{
    head = NULL;
}

//function determines whether the link list is empty (if head points to nothing)
//bool List::empty(void) const
//input: none
//output: none
//return: bool
bool List::empty(void) const
{
    return(head==NULL);
}

//function will determine if an inputted element is present in the list
//bool List::find(int foo) const
//input: integer
//output: none
//return: bool
bool List::find(Word foo) const
{
    Node* curr = head;
    while(curr && curr -> data < foo)   //loop that runs while curr isn't NULL
    {                                   //and the data in that Node is less 
                                        //than queried number
        curr = curr -> next;    //sets curr to point to the next Node in List 
    }
    
    return (curr && curr -> data == foo);   //returns true if curr isn't NULL
}                                           //and curr's data equals queried #

//This function outputs the entire list in sequential order
//void List::printnum(void) const
//input: none
//output: the list
//return: none
void List::printnum(void) const
{
    Node* curr = head;
    int counter = 1;
    
    while (curr)    //loop that runs until it reaches the end of the List
    {
        if (counter % 20 == 0)  //tests to see if it is the 20th line
        {
            char temp[10];      //sets up a dummy buffer to delay outputting
            cerr << "Press return to continue";
            cin.getline(temp, 10);
        }
        
        cout << counter << " : " << curr -> data << endl;   //actual list output
        curr = curr -> next;
        counter++;
    }
    cerr << "Insert selection: ";
}

//this function effectively inserts data into the list by making a new Node
//and adjusting the pointers of the Nodes that should lie next to this new Node
//accordingly
//void List::insert(Word foo)
//input: data element to insert
//output: none
//return: none
void List::insert(Word foo)
{
    Node* curr = head;
    Node* prev = NULL;
    while(curr && curr -> data < foo)   //finds Nodes that should be ahead
    {                                   //and behind the new Node
        prev = curr;
        curr = curr -> next;
    }
    
    Node* temp = new Node(foo,curr);    //creates the new node that 
                                        //points to the correct next Node
    if (prev)   //if the previous pointer is not NULL, then it points to temp 
        prev -> next = temp;
    else        //otherwise the new node is the new head for the list
        head = temp;
}

//function effectively removes an inputted element's Node from the list and 
//adjusts the pointers of the adjoining Nodes.
//void List::remove(Word foo)
//input: element to remove
//output: none
//return: none
void List::remove(Word foo)
{
    Node* curr = head;
    Node* prev = NULL;
    while (curr && curr -> data < foo)  //loop finds the element from the List
    {                                   
        prev = curr;
        curr = curr -> next;
    }
    
    if(curr && curr-> data == foo)      
    {
        if (prev)
            prev -> next = curr-> next; //cuts the curr Node out of the chain
        else 
            head = curr -> next;    //if there is no previous Node, the next
                                    //node becomes the head of the list
        delete curr; //deletes removed Node
    }
}

// Word List::getitem(int foo)
// This function retrieves the soundex code from a desired node in the list
//   Arguments: number of element to retrieve from the list 
//   Returns: the Word at the desired node
Word List::getitem(int foo)
{
    Word rval;
    Node* curr = head;
    int counter = 1;
    
    while (curr && foo != counter)  //cycles through the list to find the right
    {                               //node
        curr = curr -> next;
        counter++;
    }
    if (curr && foo == counter)     //checks to make sure the node exists
        rval = curr -> data;        //and the node's number is correct
    
    return rval;
}

//function that removes elements from the list in sequential order
//int List::remove(void)
//intput: none
//output: none
//return: the removed element
Word List::remove(void)
{
    if (empty())    //bails out if there is nothing in the list
        exit(1);
    Word rval = head -> data;    
    remove (rval);  //calls the remove function to remove the "head" Node
    return rval;
}

//Destructor that simply calls the remove function until the list is empty
//List::~List(void)
//input: none
//output: none
//return none
List::~List(void)
{
    while (!empty())
        remove();
}
