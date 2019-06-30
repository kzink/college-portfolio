//Kevin Zink - Word.cpp for John Clark
//mg004

#include <iostream.h>
#include"word.h"
#include <string.h>
#include <ctype.h>

// char Word::FirstChar (void) const
// Simple function to retrieve the first char in the current Word's array
//   Arguments: none 
//   Returns: the first char of the word's array
char Word::FirstChar (void) const
{
    return (array[0]);
}

// void Word::FixCap (const Word& right)
// This function matches the first char of the current Word with the
// imported Word's array's first char.
//   Arguments: address of imported word 
//   Returns: none
void Word::FixCap (const Word& right)
{
    if (isupper(right.FirstChar()))
    {
        array[0] = toupper(array[0]);//right.FirstChar();
    }
}

// Word Word::soundex (void) // function header
// Creates a soundex code out of the current Word's array
//   Arguments: none 
//   Returns: Word which contains soundex code
Word Word::Soundex (void) // function header
{
    char code[] = "Z0000";//array to hold soundex code
    int cc = 0;           //keeps track of what element to modify in code array
    char charlast = '9';  //keeps track of last letter to soundex conversion
    bool number = false, firstnum = false;
    
    for (int i = 0; (array[i] != '\0') &&  (cc != 5); i++) //loops stops when
    {                              //terminal character is reached or if
        char compare = '7';        //soundex code becomes longer than
                                   //5 characters, if the case ever occurs
        
        switch (tolower(array[i]))//this switch determines soundex code for each
                                  // letter
        {
        
            case 'b':
            case 'p':
            case 'f':
            case 'v':
                compare = '1'; //variable stores soundex code for later use
                break;
                
            case 'c':
            case 's':
            case 'k':
            case 'g':
            case 'j':
            case 'q':
            case 'x':
            case 'z':
                compare = '2';
                break;

            case 'd':
            case 't':
                compare = '3';
                break;

            case 'l':
                compare = '4';
                break;

            case 'm':
            case 'n':
                compare = '5';
                break;

            case 'r':
                compare = '6';
                break;

            case '0':              //in the case of a number
            case '1':  
            case '2': 
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
                number = true;
                compare = '7';
                break;

            default:
                compare = '7';
                break;
        }

        if (i == 0)              //used to set first number in array "code"
        {
            if (number)
            {
                firstnum = true; //this bool lets the program know if the first
            }                    //character is a number. If this is the case,
            code[0] = compare;   //code[] should start with the default 'Z'
            cc++;                
        }
            
        else
        {
            if (charlast == compare || compare == '7') //this avoids identical
            {                                          //consecutive letters  
            }                                          //and "gaps" in "code[]"
            else 
            {
                code[cc] = compare; //puts soundex code into array "code"
                cc++;
            }
        }        
        charlast = compare;   //sets charlast for future comparison
    }    
    if (firstnum)             //test weather or not to set the first char
    {                         //in the code array to 'Z'
        code[0] = 'Z';       
    }
    else 
    {
        code[0] = toupper(array[0]);//assigns the first element in "code[]" 
    }                               //to first letter in the "word"
    
    Word soundex = code;
    
    return soundex;              
}

//input: pointer to char array
//output: none
//return: none
void Word::Set( const char * in )
{
    if ( in == 0)            //in the case that nothing is input, the first  
        array[0] = '\0';     //element in the array becomes the terminal char
    strcpy (array, in);      //otherwise the input array is copied to the class'
}                            //array

//input: nothing
//output: nothing
//return: none
Word::Word ( void )
{
    Set("");                 //default constructor sends "empty" array to be
}                            //processed by the Set function

// Word::~Word(void)
// Destructor that deletes the word's array
//   Arguments:  none
//   Returns: none
Word::~Word(void)
{
    delete [] array;
}

//input: address of word class object
//output: nothing
//return: nothing
Word::Word (const Word &right)
{                            //simply copies the array of the input object to
    Set (right.array);       //the array of the current object
}

//input: pointer to char array
//output: nothing
//return: nothing
Word::Word (const char *right)
{
    Set ( right );           //copies char array into array of current object
}

//input: integer
//output: nothing
//return: nothing
Word::Word (const int x)
{
    array[0] = x;           //sets first element of object's array to int
    array[1] = '\0';        //and sets the next element to the terminal char
}

//input: address of a Word
//output: nothing
//return: pointer to modified Word
const Word & Word::operator = ( const Word & right )
{
    Set(right.array);       //copies input object's array into current object
    return *this;           //returns pointer to current object
}

//input: character array
//output: none
//return: pointer to modified word
const Word & Word::operator = ( const char * right)
{
    Set(right);             //sets inserted array into current object
    return *this;           //returns pointer to current object
}

//input: integer
//output: none
//return: pointer to modified word
const Word & Word::operator = ( const int x)
{
    array[0] = x;           //assigns first element in array to inserted int
    array[1] = '\0';        //sets next element to terminal char
    return *this;           //returns pointer to currect obj
}
 
//input: address to object & ostream
//output: inputted object's array
//return: output
ostream &operator << ( ostream &output, const Word &right )
{
    output << right.array;          //outputs array
    return output;                  //returns type output
}

//input: address of object word
//output: none
//return: bool
bool Word::operator == ( const Word &right) const
{
    return (strcasecmp (array, right.array) == 0);  //if strcasecmp returns
}                                                   //value == to 0, the case
                                                    //is true
//input: address of object word
//output: none
//return: bool
bool Word::operator < ( const Word &right) const
{
    return (strcasecmp (array, right.array) < 0);   //if strcasecmp returns
}                                                   //value < 0, the case
                                                    //is true
//input: address of object word
//output: none
//return: bool
bool Word::operator > ( const Word &right) const
{
    return (strcasecmp (array, right.array) > 0);   //if strcasecmp returns
}                                                   //value > 0, the case
                                                    //is true
//input: address of object word
//output: none
//return: bool
bool Word::operator <= ( const Word &right) const
{
    return !( *this > right.array );    //if currect obj's array is > inputted
}                                       //array then it can't be <= inputted 
                                        //array
//input: address of object word
//output: none
//return: bool
bool Word::operator >= ( const Word &right) const
{
    return !( *this < right.array );    //if currect obj's array is < inputted
}                                       //array then it can't be >= inputted
                                        //array
//input: address of object word
//output: none
//return: bool
bool Word::operator != ( const Word &right) const
{
    return !( *this == right.array );   //if currect obj's array is == inputted
}                                       //array then it can't be != inputted
                                        //array
