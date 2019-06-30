//Kevin Zink
//mg004
//tree class compilation file

#include <iostream.h>
#include <iomanip.h>
#include"tree.h"

// Tree::Tree(void)
// Constructor--sets root to NULL
//   Arguments: none 
//   Returns: none
Tree::Tree(void)
{
    root = NULL;
}

// void Tree::insert(Word foo)a
// Function inserts an inputted data element into the tree
//   Arguments:  data to insert
//   Returns: none
void Tree::insert(Word foo)
{
    TNode* par = NULL;
    TNode* curr = trav(foo,par);
    
    if (!curr) //in the case that the node doesn't exist--avoids duplicates
    {
        curr = new TNode(foo);
        
        if (!par)   //if no par exists, the new node is the root
            root = curr;
        else if (foo > par->data)   //reassigns the par node's pointers
            par -> right = curr;
        else
            par -> left = curr;
    }
}

// TNode* Tree::trav(Word foo, TNode *& par)a
// Function traverses through the Tree to the specified data element
//   Arguments:  data element and address of par's pointer
//   Returns: 
TNode* Tree::trav(Word foo, TNode *& par)
{
    TNode* curr = root;
    par = NULL;
    
    while (curr && (curr -> data != foo)) //searches through the tree until
    {                                     //it finds where the data element
        par = curr;                       //should be inserted
        
        if (foo > curr -> data)
            curr = curr -> right;
        else
            curr = curr -> left;
    }
    return curr;
}

// bool Tree::find (Word foo)
// Function determines if a queried element is in the Tree
//   Arguments:  dat element
//   Returns: bool
bool Tree::find (Word foo)
{
    TNode* par = NULL;
    return (trav(foo,par)); //if the node for foo exists the bool will be true
}

// void Tree::chainsaw (TNode* t)
// Function removes a node and all of its children
//   Arguments:  pointer to node to destroy
//   Returns: none 
void Tree::chainsaw (TNode* t)
{
    if (t)
    {
        chainsaw(t->left);  //recursively destroys all children
        chainsaw(t->right);
        delete t;   //deletes original node
    }
}

// Tree::~Tree(void)
// Destructor that "chainsaws" from the root of the Tree
//   Arguments:  none
//   Returns: none
Tree::~Tree(void)
{
    chainsaw(root);
}

// TNode* Tree::FindNode(Word soundex) const
// Function determines whether a node containing a specified soundex code is in 
// the Tree and then returns a pointer to it
//   Arguments:  soundex code
//   Returns: pointer to node containing code
TNode* Tree::FindNode(Word soundex) const
{
    TNode* rval = NULL;
    TNode* curr = root;

    while (curr && curr -> data != soundex) //goes until it finds the node
    {
        if (soundex > curr -> data) //navigates left or right through tree
            curr = curr -> right;
        else
            curr = curr -> left;
    }
    
    if (curr && curr -> data == soundex)    //check to make sure the node exists
        rval = curr;                        //and its data matches inputted data
    
    return rval;    //rval is NULL if the node is not found
}

// void Tree::PrintWords( Word soundex) const
// Function prints all of a node's list
//   Output: All words from the dictionary with the same soundex code
//   Arguments: soundex code
//   Returns: none
void Tree::PrintWords( Word soundex) const
{
    TNode* curr = FindNode(soundex);    //finds the node to print from
    
    if (curr)   //checks to make sure the returned node exists
        curr -> matches.printnum();     //prints the node's list
}   

// Word Tree::GetWord( Word soundex, int n) const
// Function finds the node that matches the inputted code and outputs element
// n from its list
//   Arguments:  soundex code to look for and the nth element from its list
//               to return
//   Returns:  the Nth element from the node's list
Word Tree::GetWord( Word soundex, int n) const
{
    Word rval;
    TNode* curr = FindNode(soundex);    //finds node

    if (curr)
        rval = curr->matches.getitem(n);    //gets Nth element
    
    return rval;
}

// void Tree::InsertWord( Word word)
// Inserts a Word's soundex code into the tree and adds the word to its list
//   Arguments:  Word to insert
//   Returns: none
void Tree::InsertWord( Word word)
{
    Word code = word.Soundex();
    TNode* curr = FindNode(code);   //finds node if it exists
    
    
    if(!curr)   //if the code doesn't have an entry in the tree, one is created
    {
        insert(code);   //inserts the node
        curr = FindNode(code);  //reassigns the curr pointer to new node
    }
    
    if(curr)    //unnecessary check for redudancy
    {
        curr -> matches.insert(word); //adds the inputted Word to the correct 
    }                                 //code's linked list
}

// bool Tree::CheckWord( Word word) const
// Function determines if a Word is under its soundex code's list
//   Arguments:  Word to search for
//   Returns: bool
bool Tree::CheckWord( Word word) const
{
    bool rval = false;
    Word code = word.Soundex();
    TNode* curr = FindNode(code);   //finds node
    
    if (curr)   //if the node exists, search through its list for the Word
        rval = curr -> matches.find(word);
    
    return rval;
}

// void Tree::print(TNode* t, int lev)
// Function to test print tree
//   Output:  Outputs all elements after inputted node
//   Arguments:  pointer to virtual root, nodes level in tree
//   Returns: none
void Tree::print(TNode* t, int lev)
{
    if (t)
    {
        print(t->right, lev+1);
        cout << setw(12*lev) << t->data << endl;
        print (t->left, lev+1);
    }
}

// void Tree::print(void)
// Test function that outputs entire list
//   Arguments:  none
//   Returns: none
void Tree::print(void)
{
    print(root, 0);
}

