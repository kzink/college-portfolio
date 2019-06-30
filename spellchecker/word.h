//Kevin Zink - Word.h for John Clark
//mg004

class Word
{
    friend ostream &operator<<( ostream&, const Word &); 
                                    //output stream for "word" class objects.
                                    //it is a "friendly" function so that it may
                                    //access the private elements of "words"
    public:
        Word(void);                //default constructor
        ~Word(void);
        
        void Set(const char *);    //function called by constructors that 
                                   //actually changes the object's array to an
                                   //"imported" char array
        
        Word(const Word &);        //copy constructors for word objects, char
        Word(const char *);        //arrays and ints
        Word(const int);
        Word Soundex(void);        //returns soundex code for word
        
        void FixCap(const Word&);  //functions modify first char in array
        char FirstChar(void) const;//to match a different Word's array
        
        const Word & operator = ( const Word & );  //overloaded assignment op's
        const Word & operator = ( const char * );
        const Word & operator = ( const int );     
        
        bool operator == ( const Word & ) const;   //overloaded comparison op's
        bool operator > ( const Word & ) const;
        bool operator < ( const Word & ) const;
        bool operator <= ( const Word & ) const;
        bool operator >= ( const Word & ) const;
        bool operator != ( const Word & ) const;
        
        
    private:
        char array[40];                 // class' array
};
