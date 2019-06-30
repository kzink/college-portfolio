//Kevin Zink
//mg004
//tree class header file

#include"list.h"

class TNode
{
    public:
        TNode* left;
        TNode* right;
        Word data;       //soundex code
        List matches;   //list of all words that have the same soundex code
        
        TNode(Word d)
        {
            data = d;
            left = right = NULL;
        }
};

class Tree
{
    public:
        Tree(void);
        ~Tree(void);
        
        bool empty(void) const; //standard tree tools
        bool find(Word);
        void insert(Word);
        void remove(Word);
        void print(void);
        void print(TNode*, int);
        
        void PrintWords( Word ) const;  //specialized tree tools for spellcheck
        TNode* FindNode( Word) const;
        Word GetWord( Word soundex, int n) const;
        void InsertWord( Word word);
        bool CheckWord( Word word) const;
            
    private:
            TNode* root;
            TNode* trav(Word, TNode*&);
            void chainsaw(TNode*);  //removes node
            //disabling of copy/assignment constructors
            Tree(const Tree&);
            const Tree& operator= (const Tree&);
};
