// Kevin Zink
// Kzink
// War card program, including driver program from John Clark.

#include<stdlib.h>
#include<time.h>
#include"queue.h"
#include<string.h>

#define NUM_CARDS 52
/* ===================================================================

   This array is just the card names.  The actual cards are just integers
   from 0 - 51, which are set in the setDeck call, shuffleCards then
   mixes up the numbers.

   In order to get the card name from the number
   cout << card[ number % 13 ] << " of " << suit[ number / 13 ];

   =================================================================== */

static const char *card[] =       //holds names of cards
{
    "Two", "Three", "Four", "Five", "Six", "Seven", "Eight",
    "Nine", "Ten", "Jack", "Queen", "King", "Ace"
};
static const char *suit[] =       //holds suits of cards
{
    "Diamonds", "Clubs", "Hearts", "Spades"
};

// macros to get the card and suit
#define CARD(x) (card[ (x) % 13 ])
#define SUIT(x) (suit[ (x) / 13 ])
//In order to get the card name from the number using the macros
//cout << CARD( number ) << " of " << SUIT( number );

// prototypes
void setDeck( int * );  //initializes index array 0-51
void shuffleCards( int * ); //shuffles values of index array
void dealCards(QUEUE *, QUEUE *, int *);

int main ()
{
    QUEUE player1, player2;     //initialization of decks for both players
    init( &player1 );
    init( &player2 );

    int deck[NUM_CARDS];    //the array for the deck to be dealt
    int rounds = 0;     //keeps track of how many rounds played
    int ties = 0;   //counter for the number of ties

    char str1[25] = ""; //strings used to processing output
    char str2[25] = "";

    //pregame set-up commands    

    setDeck(deck);  //sets the deck
    shuffleCards(deck);  //shuffles the cards
    dealCards( &player1, &player2, deck);
    
    //actual game starts here and output for every hand is administered   

    printf ("Time for WAR!\n\n");
    printf ("%18s - %-21s %21s - %s", "Player 1's card", "Player 2's card",
            "Player 1's count", "Player2's count\n");

    //loop which goes until one of the players' decks is empty
    while (length( &player1 ) != 0 && length( &player2 ) != 0)
    {
        char symbol;                    //holds the symbol, >, <, =

        // grabs top cards from each player's deck
        int player1card = dequeue( &player1 );
        int player2card = dequeue( &player2 );

        //Simple if/else system that determines the comparison symbol between 
        //the two cards and then enqueues the winner's cards into his/her deck.
        if (player1card % 13 > player2card % 13)      
        {
            symbol = '>';
            enqueue( &player1, player1card); 
            enqueue( &player1, player2card);
        }
        // if player 2's wins the hand
        else if(player1card % 13 < player2card % 13)  
        {
            symbol = '<';
            enqueue( &player2, player2card); 
            enqueue( &player2, player1card);
        }
        // if a tie occurs
        else
        {
            symbol = '=';                      
            ties++;  //increments the # of tie hands
        }

        strcpy (str1, CARD( player1card));  //copies the card name into array.
        strcat (str1, " of ");  //concatenates "of" to the array.
        strcat (str1, SUIT( player1card));  //concatenates the suit to array.

        strcpy (str2, CARD( player2card));  //same process here except for
        strcat (str2, " of ");              //player 2
        strcat (str2, SUIT( player2card));

        printf ("%18s %c %-41s", str1, symbol, str2);
        printf ("%2d - %d\n", length( &player1 ), length( &player2 ));  

        rounds++;   //increments the # of rounds played
    }

    // finishing off the program, this section just outputs overall results from the game
    if (length ( &player1 ))
        printf("\nPlayer 1 wins!\n");
    else if (length ( &player2 ))
        printf("\nPlayer 2 wins!\n");
    else
        printf("\nThe game was a tie!\n");

    printf ("It took %d hands, and there were %d ties.\n\n", rounds, ties);
    return 0;
}

// void setDeck( int * indexArray )
// initializes indexArray to 0-51
// input:   an array of NUM_CARDS integers
// output:  the array filled in with the index as the value
// returns: nothing
//
void setDeck( int * indexArray )
{
    int i;          // loop counter
    for (i = 0; i < NUM_CARDS; i++)
    {
        indexArray[i] = i;
    }
}

// void shuffleCards( int * indexArray )
// uses rand() to switch elements for shuffling
// input:   an array of NUM_CARDS integers, that has been initialized
// output:  a shuffled array of NUM_CARDS integers
// returns: nothing
//
void shuffleCards( int * indexArray )
{
    int temp;       //holds value to be switched
    int randElem;   //holds index to random element
    int i;          // loop counter

    srand( time(NULL) );    // seed the random number generator
    for (i = 0; i < NUM_CARDS; i++)
    {
        randElem = rand() % NUM_CARDS;    //picks random num for index
        temp = indexArray[i];   //holds value of curr position for switch
        indexArray[i] = indexArray[randElem]; //switches rand elem to curr
        indexArray[randElem] = temp;          //switch curr elem to rand
    }
}

//void dealCards(Queue &player1, Queue &player2, int deck[])
//distributes all even elements from the deck to player 1 and all 
//odd to player 2. This simulates a sort of "real" dealing system.
//input: the address of the players' queues and the deck's array
//output: none
//return: none
void dealCards( QUEUE * player1, QUEUE * player2, int * deck)
{
    int i; //loop counter
    for (i = 0; i < NUM_CARDS; i++)
    {
        if (i % 2)    //if the number is odd this statement will be "1" or true
            enqueue( player2, deck[i]);   //sends card from deck to be enqueued
        else
            enqueue( player1, deck[i]);
    }
}
