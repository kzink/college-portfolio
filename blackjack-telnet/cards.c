// Kevin Zink
// Kzink
// Blackjack card program that uses ports.

#include <stdio.h>
#include<stdlib.h>
#include<time.h>
#include"queue.h"
#include<string.h>
#include<unistd.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netdb.h>

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
#define ERROR   1
#define END_LINE 0x00   /* use the null char */
#define MAX_MSG 256

// prototypes
void setShoe( QUEUE *, int );
void shuffleShoe( QUEUE *, int, int );
void writer ( char *, int );
int reader ( char *, int );
int placeBet ( int, int );
int grabCard ( QUEUE *, int, int );
int addCards( int[] , int, int * );
void printPlayerCards(int[], int, int, int, int);
void printDealerCards(int[], int, int, int, int);
void bjRound( QUEUE *, int[], int[], int, int, int, int, int, int, int, int, int *, int *, int *, int *);
void matchOutput(int, int, int, int, int, int, int *);

//int main ()
int main( int argc, char * argv[] )
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



    int sd, newSd, cliLen;

    struct sockaddr_in cliAddr, servAddr;
    char line[MAX_MSG];
    int server_port;
    struct protoent * p;
    int proto_num;

    if (argc < 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    server_port = atoi(argv[1]);

    if (server_port <= 1024)
    {
        printf("Invalid server port: port must be greater than 1024\n");
        exit(1);
    }
    p = getprotobyname( "tcp" );
    proto_num = p->p_proto;

    sd = socket(AF_INET, SOCK_STREAM, proto_num );
    if(sd<0) 
    {
        perror("cannot open socket ");
        return ERROR;
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(server_port);

    if(bind(sd, (struct sockaddr *) &servAddr, sizeof(servAddr))<0) 
    {
        perror("cannot bind port ");
        return ERROR;
    }

    /* set the queue depth for connect requests */
    listen(sd,5);

    printf("Waiting for data on port TCP %u\n", server_port);

    while(1) 
    {
        cliLen = sizeof(cliAddr);
        newSd = accept(sd, (struct sockaddr *) &cliAddr, &cliLen);
        if(newSd<0) 
        {
            perror("cannot accept connection ");
            return ERROR;
        }

        /* init line */
        memset(line,0x0,MAX_MSG);
        //reader(buff, newSd);

        
        writer ("Enter starting amount of cash: ", newSd);
        reader(buff, newSd);
        int cash = atoi(buff);
        writer ("Enter number of decks: ", newSd);
        reader(buff, newSd);
        int num_cards = atoi(buff)*52;
        int orig_cash = cash;

        sprintf(buff, "Starting money - %d\nNumber of decks - %d\n", cash, num_cards/52);
        writer (buff, newSd);
        sprintf(buff, "Number of total cards - %d\n", num_cards);
        writer (buff, newSd);

        QUEUE shoe;     // the shoe
        init( &shoe );

        //pregame set-up commands    
        setShoe(&shoe, num_cards);  //sets the deck
        shuffleShoe(&shoe, num_cards, newSd);  //shuffles the cards

        writer ("Starting game...\n", newSd);

        // This loop goes till 0 is inputted when asked for a bet

        game_done = 0;
        while (!game_done)
        {
            // resets flags for each round
            num_player_cards = num_dealer_cards = 0;
            blackjack = insurance = split = pair = double_down = player_done = dealer_done = round_over = 0;

            // ---------------------------Places Bets---------------------------
            bet = placeBet(cash, newSd);
            if (bet == 0)
            {
                game_done = 1;
            }
            else
            {
                // ------------------------game stats for round 1---------------------------
                dealer[0] = grabCard(&shoe, num_cards, newSd);
                dealer[1] = grabCard(&shoe, num_cards, newSd);
                player[0] = grabCard(&shoe, num_cards, newSd);
                player[1] = grabCard(&shoe, num_cards, newSd);
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
                    writer(buff, newSd);
                    sprintf(buff, "Do you want to purchase insurance for $%d? (y/n): ", bet / 2);
                    writer(buff, newSd);
                    reader(buff, newSd);
                    if (buff[0] == 'y')
                    {

                        if (total_dealer_cards == 21)
                        {
                            bet = (bet / 2) ;
                            printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft, newSd);
                            writer("\n", newSd);
                            round_over = 1;
                            insurance = 1;
                        }
                        else
                        {
                            cash -= (bet / 2);
                            writer("Dealer does not have 21, insurance lost\n", newSd);
                        }
                    }
                }
                // checks for dealer blackjack
                else if (total_dealer_cards == 21 && total_player_cards != 21)
                {
                    round_over = 1;
                    printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft, newSd);
                    writer("\n", newSd);
                }
                // checks for player blackjack
                else if (total_player_cards == 21 && total_dealer_cards != 21)
                {
                    blackjack = 1;
                    round_over = 1;
                    printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft, newSd);
                    writer("\n", newSd);
                }
                // checks for push
                else if (total_player_cards == 21 && total_dealer_cards == 21)
                {
                    round_over = 1;
                    printDealerCards(dealer, num_dealer_cards, total_dealer_cards, soft, newSd);
                    writer("\n", newSd);
                }
                // default output if no special cases
                else 
                {
                    // screen output for DEALER ROUND ONE
                    sprintf(buff, "Dealer shows : %s\n", CARD(dealer[0]));
                    writer(buff, newSd);
                }

                // screen output for PLAYER ROUND ONE 
                printPlayerCards(player, num_player_cards, total_player_cards, soft, newSd);

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
                    writer(buff, newSd);

                    // This loop parses the input and does the correct subroutine

                    int valid_input = 1;
                    while (valid_input)
                    {
                        valid_input = 0;
                        reader(buff, newSd);
                        char temp = tolower(buff[0]);

                        if (temp == 'h')
                        {
                            player[num_player_cards] = grabCard(&shoe, num_cards, newSd);
                            num_player_cards++;
                            output_results = 1;
                        }
                        else if (temp == 's' && total_player_cards <= 21)
                            player_done = 1;
                        else if (temp == 'd' && (bet * 2) <= cash)
                        {
                            bet *= 2;
                            sprintf(buff, "    Doubled bet to $%d\n", bet);
                            writer(buff, newSd);
                            player[num_player_cards] = grabCard(&shoe, num_cards, newSd);
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
                            writer("Invalid input; try again: ", newSd);
                        }
                    }

                    total_player_cards = addCards( player, num_player_cards, &soft);

                    // outputs PLAYER'S hand after selections
                    if (output_results)
                        printPlayerCards(player, num_player_cards, total_player_cards, soft, newSd);
                }

                // output for non-split rounds
                if (!split)
                {
                    bjRound( &shoe, player, dealer, split, double_down, blackjack, insurance, 
                            num_cards, round_over, player_done, newSd, &bet, &cash, &num_dealer_cards, &num_player_cards);
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
                    xtrahand[1] = grabCard(&shoe, num_cards, newSd);
                    int total_xtrahand_cards = addCards( xtrahand, num_xtrahand_cards, &soft);
                    writer("first hand\n", newSd);
                    printPlayerCards(xtrahand, num_xtrahand_cards, total_xtrahand_cards, soft, newSd);
                    bjRound( &shoe, xtrahand, dealer, split, double_down, blackjack, insurance, 
                            num_cards, round_over, player_done, newSd, &bet, &cash, &num_dealer_cards, &num_xtrahand_cards);

                    split++;
                    player[1] = grabCard(&shoe, num_cards, newSd);
                    total_player_cards = addCards( player, num_player_cards, &soft);
                    writer("second hand\n", newSd);
                    printPlayerCards(player, num_player_cards, total_player_cards, soft, newSd);
                    bjRound( &shoe, player, dealer, split, double_down, blackjack, insurance,
                            num_cards, round_over, player_done, newSd, &bet1, &cash, &num_dealer_cards, &num_player_cards);

                    total_xtrahand_cards = addCards( xtrahand, num_xtrahand_cards, &soft);
                    total_player_cards = addCards( player, num_player_cards, &soft);
                    total_dealer_cards = addCards( dealer, num_dealer_cards, &soft);
                    writer("first hand - ", newSd);
                    matchOutput(total_dealer_cards, total_xtrahand_cards, blackjack, insurance, bet, newSd, &cash);
                    writer("second hand - ", newSd);
                    matchOutput(total_dealer_cards, total_player_cards, blackjack, insurance, bet1, newSd, &cash);
                }
            }
        }
        sprintf(buff, "Your net cash change was: $%d\n", cash - orig_cash);
        writer (buff, newSd);
        close(newSd);
    } //while(1)
    return 0;
}

// This function is terribe ugly because it needs a bunch of flags to operate.
// It basically deals with everything that happens after the first 2 cards are dealt.
// It also figures out who won and how much money is awarded.
void bjRound( QUEUE * shoe, int player[], int dealer[], int split, int double_down, int blackjack, int insurance, 
        int num_cards, int round_over, int player_done, int newSd, int *bet, int *cash, int *num_dealer_cards, int *num_player_cards) 
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
            writer(buff, newSd);

            // This loop parses the input and does the correct subroutine

            int valid_input = 1;
            while (valid_input)
            {
                valid_input = 0;
                reader(buff, newSd);
                char temp = tolower(buff[0]);

                if (temp == 'h')
                {
                    player[*num_player_cards] = grabCard(shoe, num_cards, newSd);
                    (*num_player_cards)++;
                    output_results = 1;
                }
                else if (*num_player_cards == 2 && (temp == 'd' && (*bet * 2) <= *cash))
                {
                    (*bet) *= 2;
                    sprintf(buff, "    Doubled bet to $%d\n", *bet);
                    writer(buff, newSd);
                    player[*num_player_cards] = grabCard(shoe, num_cards, newSd);
                    (*num_player_cards)++;
                    output_results = 1;
                    double_down = 1;
                }
                else if (temp == 's' && total_player_cards <= 21)
                    player_done = 1;
                else
                {
                    valid_input = 1;
                    writer("Invalid input; try again: ", newSd);
                }
            }
        }
        else
            player_done = 1;

        total_player_cards = addCards( player, *num_player_cards, &soft);
        // outputs PLAYER'S hand after selections
        if (output_results)
            printPlayerCards(player, *num_player_cards, total_player_cards, soft, newSd);
    }

    // ------------------------Loop for DEALER HAND AND OUTPUT---------------------------

    // for splits, the dealer doesn't deal until after the 2nd hand is played
    if (split != 1)
    {
        while (!round_over)
        {
            total_dealer_cards = addCards(dealer, *num_dealer_cards, &soft);
            printDealerCards(dealer, *num_dealer_cards, total_dealer_cards, soft, newSd);

            // dealer hits
            if (total_dealer_cards <= 16)
            {
                writer("hit\n", newSd);
                dealer[*num_dealer_cards] = grabCard(shoe, num_cards, newSd);
                (*num_dealer_cards)++;
            }
            // dealer stands
            else if (total_dealer_cards >=17 && total_dealer_cards <= 21)
            {
                writer("stand\n", newSd);
                round_over = 1;
            }
            // dealer has gone bust
            else if (total_dealer_cards > 21)
            {
                writer("Bust\n", newSd);
                round_over = 1;
            }
        }
    }

    // Output for the round
    if (!split)
    {
        total_dealer_cards = addCards(dealer, *num_dealer_cards, &soft);
        matchOutput(total_dealer_cards, total_player_cards, blackjack, insurance, *bet, newSd, cash);
    }
}

// This function just outputs the winner of the round and some stats about the round.
void matchOutput(int total_dealer_cards, int total_player_cards, int blackjack, int insurance, int bet, int newSd, int *cash)
{
    char buff[80];

    if (total_player_cards == total_dealer_cards && total_player_cards <= 21)
        writer("Push\n", newSd);

    else if (total_dealer_cards == 21 && total_player_cards != 21 && !insurance)
    {
        *cash -= bet;
        sprintf(buff, "Dealer Wins! - You lose $%d\n", bet);
        writer(buff, newSd);
    }
    else if (total_dealer_cards == 21 && total_player_cards != 21 && insurance)
    {
        *cash -= bet;
        sprintf(buff, "You lose $%d\n", bet);
        writer(buff, newSd);
    }
    else if (total_player_cards == 21)
    {
        if (blackjack)
        {
            *cash += bet * 1.5;
            sprintf(buff, "Blackjack!! - You win $%d\n", (int)(bet * 1.5));
            writer(buff, newSd);
        }
        else
        {
            *cash += bet;
            sprintf(buff, "21!! - You win $%d\n", (int)(bet));
            writer(buff, newSd);
        }
    } 
    else if ( total_dealer_cards < 21 && total_player_cards < 21)
    {
        if (total_player_cards > total_dealer_cards)
        {
            *cash += bet;
            sprintf(buff, "You win $%d\n", (int)(bet));
            writer(buff, newSd); 
        }
        else
        {
            *cash -= bet;
            sprintf(buff, "Dealer Wins - You lose $%d\n", bet);
            writer(buff, newSd);
        }
    }
    else if (total_player_cards > 21)
    {
        *cash -= bet;
        sprintf(buff, "Busted!! - You lose $%d\n", bet);
        writer(buff, newSd);
    }
    else if (total_dealer_cards > 21)
    {
        *cash += bet;
        sprintf(buff, "You win $%d\n", (int)(bet));
        writer(buff, newSd); 
    }
    else
    {
        *cash -= bet;
        sprintf(buff, "You lose $%d\n", bet);
        writer(buff, newSd);

    }
}

// This function outputs the cards in the dealer's deck followed by the sum of those cards.
void printDealerCards(int deck[], int num_deck_cards, int total_deck_cards, int soft, int newSd)
{
    int i; //loop counter
    char buff[80];
    sprintf(buff, "Dealer has : ");
    for (i = 0; i < num_deck_cards; i++)
    {
        strcat (buff, CARD(deck[i]));
        strcat (buff, " ");
    }
    writer(buff, newSd);
    sprintf(buff, "= %d - ", total_deck_cards);
    writer(buff, newSd);
}

// This function outputs the cards in the player's deck followed by the sum of those cards.
void printPlayerCards(int deck[], int num_deck_cards, int total_deck_cards, int soft, int newSd)
{
    int i; //loop counter
    char buff[80];
    sprintf(buff, "You have : ");
    for (i = 0; i < num_deck_cards; i++)
    {
        strcat (buff, CARD(deck[i]));
        strcat (buff, " ");
    }
    writer(buff, newSd);
    if (soft == 1)
        sprintf(buff, "= soft %d\n", total_deck_cards);
    else
        sprintf(buff, "= %d\n", total_deck_cards);
    writer(buff, newSd);
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
int grabCard( QUEUE * shoe, int num_cards, int newSd )
{
    if (length(shoe) < 15)
    {
        setShoe(shoe, num_cards);
        shuffleShoe(shoe, num_cards, newSd);
    }

    return dequeue(shoe);
}

// This function deals with parsing placing bets.
int placeBet( int cash, int newSd )
{
    char buff[80];
    int bet;

    if (cash <= 0)
    {
        writer("Not enough cash to play.\n", newSd);
        bet = 0;
        //exit(-1);
    }
    else
    {

        sprintf(buff, "\nPlace bet ($%d available, 0 to quit): ", cash);
        writer(buff, newSd);

        int valid_input = 0;
        while (!valid_input)
        {
            reader(buff, newSd);
            bet = atoi(buff);
            if (bet < 0 || bet > cash)
                writer("Invalid bet; try again: ", newSd);
            else
                valid_input = 1;
        }
    }    
        return bet;
}

// This function will be used for ports in the next program
// All output is writing through this function.
void writer ( char * buff, int newSd )
{
    int test = write(newSd, buff, strlen( buff ));
    if (test <= 0)
    {
        printf("Error in writing output\n");
        exit(-1);
    }
}

// This function will be used for ports in the next program
// All input is read through this function.
// The core of this function is taken frmo John Clark's sample server program.
int reader ( char * line_to_return, int newSd )
{
    static int rcv_ptr=0;
    static char rcv_msg[MAX_MSG];
    static int n;
    int offset;

    offset=0;
    if( rcv_ptr == 0 ) 
    {
        /* read data from socket */
        memset(rcv_msg,0x0,MAX_MSG); /* init buffer */
        n = read(newSd, rcv_msg, MAX_MSG); /* wait for data */
        if ( n < 0 ) 
        {
            perror(" cannot receive data ");
            return ERROR;
        }
        else if ( n == 0 ) 
        {
            printf(" connection closed by client\n");
            close(newSd);
            return ERROR;
        }
    }

    /* if new data read on socket */
    /* OR */
    /* if another line is still in buffer */

    /* copy line into 'line_to_return' */
    while(( rcv_msg[rcv_ptr] != END_LINE ) && ( rcv_ptr < n ))
    {
        line_to_return[offset] = rcv_msg[rcv_ptr];
        offset++;
        rcv_ptr++;
    }

    /* end of line + end of buffer => return line */
    if( rcv_ptr == ( n - 1 ))
    { 
        /* set last byte to NULL */
        line_to_return[offset] = 0;
        rcv_ptr = 0;
        return ++offset;
    } 

    /* end of line but still some data in buffer => return line */
    /* we will resume work on the buffer when we return */
    if( rcv_ptr < ( n - 1 )) 
    {
        /* set last byte to NULL */
        line_to_return[offset] = 0;
        rcv_ptr++;
        return ++offset;
    }

    /* end of buffer but line is not ended => */
    /*  wait for more data to arrive on socket */
    if( rcv_ptr == n ) 
    {
        rcv_ptr = 0;
    } 
    return 0;
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
void shuffleShoe( QUEUE * shoe, int num_cards, int newSd )
{
    writer ("\nShuffling shoe...\n", newSd);
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
