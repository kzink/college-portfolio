// Kevin Zink
// Kzink
// Blackjack card program.

#include<stdlib.h>
#include<time.h>
#include"queue.h"
#include<string.h>
#include<unistd.h>
#include<ctype.h>

static const char *card[] =       //holds names of cards
{
    "A", "2", "3", "4", "5", "6", "7", "8",
    "9", "T", "J", "Q", "K" 
};

static const int cardvalue[] =
{
    1, 2, 3, 4, 5, 6, 7, 8 ,9 ,10, 10, 10, 10
};

// macros to get the card and the value
#define CARD(x) (card[ (x) % 13 ])
#define CARDVALUE(x) (cardvalue[ (x) % 13])

// prototypes
void setShoe( QUEUE *, int );
void shuffleShoe( QUEUE *, int );
void writer ( char * );
void reader ( char * );
int placeBet ( int );
int grabCard ( QUEUE *, int );
int addCards( int[] , int, int * );
void printPlayerCards(int[], int, int, int);
void printDealerCards(int[], int, int, int);
void bjRound( QUEUE *, int[], int[], int, int, int, int, int, int, int, int *, int *, int *, int *);
void matchOutput(int, int, int, int, int, int *);

int main ()
{
    // loads of flags
    int insurance = 0;
    int game_done = 0;
    int player_done = 0;
    int blackjack = 0;
    int round_over = 0;
    int dealer_done = 0;
    int output_results = 0;
    int bet;
    int split = 0;
    int double_down = 0;
    int pair = 0;
    int soft = 0;
    int num_dealer_cards = 0;
    int num_player_cards = 0;
    int total_dealer_cards = 0;
    int total_player_cards = 0;
    int player[20];
    int dealer[20];
    char buff[80];

    writer ("Enter starting amount of cash: ");
    reader (buff);
    int cash = atoi(buff);
    writer ("Enter number of decks: ");
    reader (buff);
    int num_cards = atoi(buff)*52;
    int orig_cash = cash;
    
    //int num_cards = 60 * 52;
    //int cash = 100;

    sprintf(buff, "Starting money - %d\nNumber of decks - %d\n", cash, num_cards/52);
    writer (buff);
    sprintf(buff, "Number of total cards - %d\n", num_cards);
    writer (buff);

    QUEUE shoe;     // the shoe
    init( &shoe );

    //pregame set-up commands    
    setShoe(&shoe, num_cards);  //sets the deck
    shuffleShoe(&shoe, num_cards);  //shuffles the cards

    writer ("Starting game...\n");

    // This loop goes till 0 is inputted when asked for a bet
    while (!game_done)
    {
        // resets flags for each round
        num_player_cards = num_dealer_cards = 0;
        blackjack = insurance = split = pair = double_down = player_done = dealer_done = round_over = 0;

        // ---------------------------Places Bets---------------------------
        bet = placeBet(cash);
        if (bet == 0)
        {
            game_done = 1;
        }
        else
        {
            // ------------------------game stats for round 1---------------------------
            dealer[0] = grabCard(&shoe, num_cards);
            dealer[1] = grabCard(&shoe, num_cards);
            player[0] = grabCard(&shoe, num_cards);
            player[1] = grabCard(&shoe, num_cards);
            num_player_cards = num_dealer_cards = 2;

            //player[0] = 6;
            //player[1] = 6;
            
            total_dealer_cards = addCards( dealer, num_dealer_cards, &soft);
            total_player_cards = addCards( player, num_player_cards, &soft);

            // this tests for possible split
            if (CARD(player[0]) == CARD(player[1]))
                pair = 1;

            // ------------------------ Checks for various flags in the beginning of the game---------------------------

            // for insurance
            if (CARD(dealer[0]) == "A")
            {
                sprintf(buff, "Dealer shows : %s\n", CARD(dealer[0]));
                writer(buff);
                sprintf(buff, "Do you want to purchase insurance for $%d? (y/n): ", bet / 2);
                writer(buff);
                reader (buff);
                if (buff[0] == 'y')
                {

                    if (total_dealer_cards == 21)
                    {
                        bet = (bet / 2) ;
                        printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft);
                        writer("\n");
                        round_over = 1;
                        insurance = 1;
                    }
                    else
                    {
                        cash -= (bet / 2);
                        writer("Dealer does not have 21, insurance lost\n");
                    }
                }
            }
            // checks for dealer blackjack
            else if (total_dealer_cards == 21 && total_player_cards != 21)
            {
                round_over = 1;
                printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft);
                writer("\n");
            }
            // checks for player blackjack
            else if (total_player_cards == 21 && total_dealer_cards != 21)
            {
                blackjack = 1;
                round_over = 1;
                printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft);
                writer("\n");
            }
            // checks for push
            else if (total_player_cards == 21 && total_dealer_cards == 21)
            {
                round_over = 1;
                printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft);
                writer("\n");
            }
            // default output if no special cases
            else 
            {
                // screen output for DEALER ROUND ONE
                sprintf(buff, "Dealer shows : %s\n", CARD(dealer[0]));
                writer(buff);
            }

            // screen output for PLAYER ROUND ONE 
            printPlayerCards(player, num_player_cards, total_player_cards, soft);

            if (!round_over)
            {
                // Screen output / input post round 2
                strcpy (buff, "");
                strcat(buff, "(H)it (S)tand ");
                if ( (bet * 2) <= cash )
                    strcat(buff, "(D)ouble ");
                if (pair && (bet * 2) <= cash )
                    strcat(buff, "s(P)lit" );
                strcat(buff, ": ");
                writer(buff);

                // This loop parses the input and does the correct subroutine

                int valid_input = 1;
                while (valid_input)
                {
                    valid_input = 0;
                    reader(buff);
                    char temp = tolower(buff[0]);

                    if (temp == 'h')
                    {
                        player[num_player_cards] = grabCard(&shoe, num_cards);
                        num_player_cards++;
                        output_results = 1;
                    }
                    else if (temp == 's' && total_player_cards <= 21)
                        player_done = 1;
                    else if (temp == 'd' && (bet * 2) <= cash)
                    {
                        bet *= 2;
                        sprintf(buff, "    Doubled bet to $%d\n", bet);
                        writer(buff);
                        player[num_player_cards] = grabCard(&shoe, num_cards);
                        num_player_cards++;
                        output_results = 1;
                        double_down = 1;
                    }
                    else if (temp == 'p' && pair && (bet * 2) <= cash)
                    {
                        split = 1;
                    }
                    else
                    {
                        valid_input = 1;
                        writer("Invalid input; try again: ");
                    }
                }

                total_player_cards = addCards( player, num_player_cards, &soft);

                // outputs PLAYER'S hand after selections
                if (output_results)
                    printPlayerCards(player, num_player_cards, total_player_cards, soft);
            }

            // output for non-split rounds
            if (!split)
            {
                bjRound( &shoe, player, dealer, split, double_down, blackjack, insurance, 
                        num_cards, round_over, player_done, &bet, &cash, &num_dealer_cards, &num_player_cards);
            }
            // special outputting stuff for split-rounds
            else
            {
                int xtrahand[20]; 
                int num_xtrahand_cards = 2;
                int bet1 = bet;
                
                if (CARD(player[0]) == "A" && CARD(player[1]) == "A")
                    player_done = 1;

                xtrahand[0] = player[0];
                xtrahand[1] = grabCard(&shoe, num_cards);
                int total_xtrahand_cards = addCards( xtrahand, num_xtrahand_cards, &soft);
                writer("first hand\n");
                printPlayerCards(xtrahand, num_xtrahand_cards, total_xtrahand_cards, soft);
                bjRound( &shoe, xtrahand, dealer, split, double_down, blackjack, insurance, 
                        num_cards, round_over, player_done, &bet, &cash, &num_dealer_cards, &num_xtrahand_cards);

                split++;
                player[1] = grabCard(&shoe, num_cards);
                total_player_cards = addCards( player, num_player_cards, &soft);
                writer("second hand\n");
                printPlayerCards(player, num_player_cards, total_player_cards, soft);
                bjRound( &shoe, player, dealer, split, double_down, blackjack, insurance,
                        num_cards, round_over, player_done, &bet1, &cash, &num_dealer_cards, &num_player_cards);

                total_xtrahand_cards = addCards( xtrahand, num_xtrahand_cards, &soft);
                total_player_cards = addCards( player, num_player_cards, &soft);
                total_dealer_cards = addCards( dealer, num_dealer_cards, &soft);
                writer("first hand - ");
                matchOutput(total_dealer_cards, total_xtrahand_cards, blackjack, insurance, bet, &cash);
                writer("second hand - ");
                matchOutput(total_dealer_cards, total_player_cards, blackjack, insurance, bet1, &cash);
            }
        }
    }

    sprintf(buff, "Your net cash change was: $%d\n", cash - orig_cash);
    writer (buff);
    return 0;
}

// This function is terribe ugly because it needs a bunch of flags to operate.
// It basically deals with everything that happens after the first 2 cards are dealt.
// It also figures out who won and how much money is awarded.
void bjRound( QUEUE * shoe, int player[], int dealer[], int split, int double_down, int blackjack, int insurance, 
        int num_cards, int round_over, int player_done, int *bet, int *cash, int *num_dealer_cards, int *num_player_cards) 
{
    char buff[80];
    int soft = 0;
    int total_player_cards = addCards( player, *num_player_cards, &soft);
    int total_dealer_cards;
    *num_dealer_cards = 2;

    // Loop prompts for more input
    while (!round_over && !player_done)
    {
        int output_results = 0;
        if (total_player_cards == 21 || total_player_cards > 21)
        {
            if (split == 2)
            {
                round_over = 0;
                player_done = 1;
            }
            else
                round_over = 1;
        }
        else if (!double_down) 
        {
            // Screen output / input post round 2
            strcpy (buff, "");
            strcat(buff, "(H)it (S)tand");
            if ( *num_player_cards == 2 && (*bet * 2) <= *cash )
                strcat(buff, " (D)ouble ");
            strcat(buff, ": ");
            writer(buff);

            // This loop parses the input and does the correct subroutine

            int valid_input = 1;
            while (valid_input)
            {
                valid_input = 0;
                reader(buff);
                char temp = tolower(buff[0]);

                if (temp == 'h')
                {
                    player[*num_player_cards] = grabCard(shoe, num_cards);
                    (*num_player_cards)++;
                    output_results = 1;
                }
                else if (*num_player_cards == 2 && (temp == 'd' && (*bet * 2) <= *cash))
                {
                    (*bet) *= 2;
                    sprintf(buff, "    Doubled bet to $%d\n", *bet);
                    writer(buff);
                    player[*num_player_cards] = grabCard(shoe, num_cards);
                    (*num_player_cards)++;
                    output_results = 1;
                    double_down = 1;
                }
                else if (temp == 's' && total_player_cards <= 21)
                    player_done = 1;
                else
                {
                    valid_input = 1;
                    writer("Invalid input; try again: ");
                }
            }
        }
        else
            player_done = 1;

        total_player_cards = addCards( player, *num_player_cards, &soft);
        // outputs PLAYER'S hand after selections
        if (output_results)
            printPlayerCards(player, *num_player_cards, total_player_cards, soft);
    }

    // ------------------------Loop for DEALER HAND AND OUTPUT---------------------------

    // for splits, the dealer doesn't deal until after the 2nd hand is played
    if (split != 1)
    {
        while (!round_over)
        {
            total_dealer_cards = addCards(dealer, *num_dealer_cards, &soft);
            printDealerCards(dealer, *num_dealer_cards, total_dealer_cards, soft);

            // dealer hits
            if (total_dealer_cards <= 16)
            {
                writer("hit\n");
                dealer[*num_dealer_cards] = grabCard(shoe, num_cards);
                (*num_dealer_cards)++;
            }
            // dealer stands
            else if (total_dealer_cards >=17 && total_dealer_cards <= 21)
            {
                writer("stand\n");
                round_over = 1;
            }
            // dealer has gone bust
            else if (total_dealer_cards > 21)
            {
                writer("Bust\n");
                round_over = 1;
            }
        }
    }

    // Output for the round
    if (!split)
    {
        total_dealer_cards = addCards(dealer, *num_dealer_cards, &soft);
        matchOutput(total_dealer_cards, total_player_cards, blackjack, insurance, *bet, cash);
    }
}

// This function just outputs the winner of the round and some stats about the round.
void matchOutput(int total_dealer_cards, int total_player_cards, int blackjack, int insurance, int bet, int *cash)
{
    char buff[80];
    
    if (total_player_cards == total_dealer_cards && total_player_cards <= 21)
        writer("Push\n");

    else if (total_dealer_cards == 21 && total_player_cards != 21 && !insurance)
    {
        *cash -= bet;
        sprintf(buff, "Dealer Wins! - You lose $%d\n", bet);
        writer(buff);
    }
    else if (total_dealer_cards == 21 && total_player_cards != 21 && insurance)
    {
        *cash -= bet;
        sprintf(buff, "You lose $%d\n", bet);
        writer(buff);
    }
    else if (total_player_cards == 21)
    {
        if (blackjack)
        {
            *cash += bet * 1.5;
            sprintf(buff, "Blackjack!! - You win $%d\n", (int)(bet * 1.5));
            writer(buff);
        }
        else
        {
            *cash += bet;
            sprintf(buff, "21!! - You win $%d\n", (int)(bet));
            writer(buff);
        }
    } 
    else if ( total_dealer_cards < 21 && total_player_cards < 21)
    {
        if (total_player_cards > total_dealer_cards)
        {
            *cash += bet;
            sprintf(buff, "You win $%d\n", (int)(bet));
            writer(buff); 
        }
        else
        {
            *cash -= bet;
            sprintf(buff, "Dealer Wins - You lose $%d\n", bet);
            writer(buff);
        }
    }
    else if (total_player_cards > 21)
    {
        *cash -= bet;
        sprintf(buff, "Busted!! - You lose $%d\n", bet);
        writer(buff);
    }
    else if (total_dealer_cards > 21)
    {
        *cash += bet;
        sprintf(buff, "You win $%d\n", (int)(bet));
        writer(buff); 
    }
    else
    {
        *cash -= bet;
        sprintf(buff, "You lose $%d\n", bet);
        writer(buff);

    }
}

// This function outputs the cards in the dealer's deck followed by the sum of those cards.
void printDealerCards(int deck[], int num_deck_cards, int total_deck_cards, int soft)
{
    int i; //loop counter
    char buff[80];
    sprintf(buff, "Dealer has : ");
    for (i = 0; i < num_deck_cards; i++)
    {
        strcat (buff, CARD(deck[i]));
        strcat (buff, " ");
    }
    writer(buff);
    sprintf(buff, "= %d - ", total_deck_cards);
    writer(buff);
}

// This function outputs the cards in the player's deck followed by the sum of those cards.
void printPlayerCards(int deck[], int num_deck_cards, int total_deck_cards, int soft)
{
    int i; //loop counter
    char buff[80];
    sprintf(buff, "You have : ");
    for (i = 0; i < num_deck_cards; i++)
    {
        strcat (buff, CARD(deck[i]));
        strcat (buff, " ");
    }
    writer(buff);
    if (soft == 1)
        sprintf(buff, "= soft %d\n", total_deck_cards);
    else
        sprintf(buff, "= %d\n", total_deck_cards);
    writer(buff);
}

// This function counts all the cards' values up for a total sum
int addCards( int deck[], int num_deck_cards, int *soft )
{
    int i; //loop counter
    int ace = 0;
    *soft = 0;
    int total_deck_cards = 0;
    for (i = 0; i < num_deck_cards; i++)
    {
        if (CARDVALUE(deck[i]) == 1)
            ace = 1;
        total_deck_cards += CARDVALUE(deck[i]);
    }
    if (ace == 1 && total_deck_cards <= 11)
    {
        total_deck_cards += 10;
        *soft = 1;
    }

    return total_deck_cards;
}

// This function grabs a card from the shoe
int grabCard( QUEUE * shoe, int num_cards )
{
    if (length(shoe) < 15)
    {
        setShoe(shoe, num_cards);
        shuffleShoe(shoe, num_cards);
    }

    return dequeue(shoe);
}

// This function deals with parsing placing bets.
int placeBet( int cash )
{
    char buff[80];
    int bet;

    if (cash <= 0)
    {
        writer("Not enough cash to play.\n");
        exit(-1);
    }

    sprintf(buff, "\nPlace bet ($%d available, 0 to quit): ", cash);
    writer(buff);

    int valid_input = 0;
    while (!valid_input)
    {
        reader(buff);
        bet = atoi(buff);
        if (bet < 0 || bet > cash)
            writer("Invalid bet; try again: ");
        else
            valid_input = 1;
    }    
    return bet;
}

// This function will be used for ports in the next program
// All output is writing through this function.
void writer ( char * buff )
{
    int test = write(1, buff, strlen( buff ));
    if (test <= 0)
    {
        printf("Error in writing output");
        exit(-1);
    }
}

// This function will be used for ports in the next program
// All input is read through this function.
void reader ( char * buff )
{
    int test = read( 0, buff, 80);
    if (test <= 0)
    {
        printf("Error in reading input");
        exit(-1);
    }
}

// This function fills the shoe with cards
void setShoe( QUEUE * shoe, int num_cards )
{
    int i;          // loop counter

    while (!empty(shoe))
    {
        dequeue( shoe );
    }

    for (i = 0; i < num_cards; i++)
    {
        enqueue( shoe, i );
    }
}

// This function shuffles the cards in the shoe
void shuffleShoe( QUEUE * shoe, int num_cards )
{
    writer ("\nShuffling shoe...\n");
    int temp;       //holds value to be switched
    int randElem;   //holds index to random element
    int i;          // loop counter
    int arr[num_cards];
    int deck_size = length( shoe );

    i = 0;
    while (!empty( shoe ))
    {
        arr[i] = dequeue( shoe );
        i++;
    }

    srand( time(NULL) );    // seed the random number generator
    for (i = 0; i < deck_size; i++)
    {
        randElem = rand() % deck_size;    //picks random num for index
        temp = arr[i];   //holds value of curr position for switch
        arr[i] = arr[randElem]; //switches rand elem to curr
        arr[randElem] = temp;          //switch curr elem to rand
    }

    for (i = 0; i < deck_size; i++)
    {
        enqueue( shoe, arr[i] );
    }
}
