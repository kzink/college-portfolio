// Kevin Zink
// account: kzink

// This is a client program to interface with the Bjack server.
// Some of this code is taken from John Clark's sample "C" client program.
// In particular, the reader subroutine and the socket/connection handling
// is virtually copy/paste from John Clark's sample program.

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* close */
#include <regex.h>

#define ERROR   1
#define MAX_MSG 1000
#define END_LINE '\n'
#define PATTERN_NUM 18

void key_reader ( char * );
int server_reader ( int,  char * );
void writer ( int, char *);
int calc_multiplier( char *, int *, int * );

//-------------------------------- TABLE ARRAYS-------------------------------------
//                               2    3    4    5    6    7    8    9   10    A
static const char *hardv[][10]={{"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"}, //0
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //1
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //2
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //3
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //4
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //5
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //6
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //7
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //8
                               {"h", "d", "d", "d", "d", "h", "h", "h", "h", "h"}, //9
                               {"d", "d", "d", "d", "d", "d", "d", "d", "h", "h"}, //10
                               {"d", "d", "d", "d", "d", "d", "d", "d", "d", "h"}, //11
                               {"h", "h", "s", "s", "s", "h", "h", "h", "h", "h"}, //12
                               {"s", "s", "s", "s", "s", "h", "h", "h", "h", "h"}, //13
                               {"s", "s", "s", "s", "s", "h", "h", "h", "h", "h"}, //14
                               {"s", "s", "s", "s", "s", "h", "h", "h", "h", "h"}, //15
                               {"s", "s", "s", "s", "s", "h", "h", "h", "h", "h"}, //16
                               {"s", "s", "s", "s", "s", "s", "s", "s", "s", "s"}};//17

//                               2    3    4    5    6    7    8    9   10    A
static const char * softv[][10]={{"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"}, //0
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //1
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //2
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //3
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //4
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //5
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //6
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //7
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //8
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //9
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //10
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //11
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //12
                               {"h", "h", "h", "d", "d", "h", "h", "h", "h", "h"}, //13
                               {"h", "h", "d", "d", "d", "h", "h", "h", "h", "h"}, //14
                               {"h", "h", "d", "d", "d", "h", "h", "h", "h", "h"}, //15
                               {"h", "h", "d", "d", "d", "h", "h", "h", "h", "h"}, //16
                               {"h", "d", "d", "d", "d", "h", "h", "h", "h", "h"}, //17
                               {"s", "d", "d", "d", "d", "s", "s", "h", "h", "h"}, //18
                               {"s", "s", "s", "s", "s", "s", "s", "s", "s", "s"}};//19

static const char * splits[][10]={{"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},       //0
                               {"p", "p", "p", "p", "p", "p", "p", "p", "p", "p"},          //A
                               {"h", "h", "p", "p", "p", "p", "h", "h", "h", "h"},          //2
                               {"h", "h", "p", "p", "p", "p", "h", "h", "h", "h"},          //3
                               {"h", "h", "h", "h", "h", "h", "h", "h", "h", "h"},          //4
                               {"d", "d", "d", "d", "d", "d", "d", "d", "h", "h"},          //5
                               {"h", "p", "p", "p", "p", "h", "h", "h", "h", "h"},          //6
                               {"p", "p", "p", "p", "p", "p", "h", "h", "h", "h"},          //7
                               {"p", "p", "p", "p", "p", "p", "p", "p", "p", "p"},          //8
                               {"p", "p", "p", "p", "p", "p", "p", "p", "s", "s"},          //9
                               {"s", "s", "s", "s", "s", "s", "s", "s", "s", "s"}};        //10


int main (int argc, char *argv[]) 
{
    // variable initiations
    int not_done;
    int match;
    int sd;
    int rc;
    int i;
    struct sockaddr_in localAddr;
    struct sockaddr_in servAddr;
    struct hostent *h;
    int server_port;
    char buff[MAX_MSG];
    char serverline[MAX_MSG];
    size_t n = 5;
    char output[10];
    regmatch_t p[5];
    char * patterns[PATTERN_NUM];
    regex_t rxs[PATTERN_NUM];
    int count = 0;
    int total_hands = 0;
    int basebet;
    int stop_amount;
    int soft = 0;
    char dealer_card1[10];
    char dealer_card2[10];
    char player_count[10];
    char player_card1[10];
    char player_card2[10];
    int pc, dc;
    int multiplier = 1;
    int num_cards;
    int running_cards;
    int total_cash;
    int no_double;
    int hands = 0;
    int split_me = 0;
   
    // Connection handling
    if(argc < 3) 
    {
        printf("usage: %s <server> <port>\n", argv[0]);
        exit(1);
    }

    h = gethostbyname(argv[1]);
    if(h==NULL) 
    {
        printf("%s: unknown host '%s'\n",argv[0],argv[1]);
        exit(1);
    }

    server_port = atoi( argv[2] );
    if ( server_port == 0 ) 
    {
        printf("%s: invalid port '%s'\n",argv[0],argv[2]);
        exit(1);
    }

    servAddr.sin_family = h->h_addrtype;
    memcpy((char *) &servAddr.sin_addr.s_addr, h->h_addr_list[0], h->h_length);
    servAddr.sin_port = htons(server_port);

    /* create socket */
    sd = socket(AF_INET, SOCK_STREAM, getprotobyname( "tcp" )->p_proto );
    if(sd<0) 
    {
        perror("cannot open socket ");
        exit(1);
    }

    /* bind any port number */
    localAddr.sin_family = AF_INET;
    localAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    localAddr.sin_port = htons(0);

    rc = bind(sd, (struct sockaddr *) &localAddr, sizeof(localAddr));
    if(rc<0) 
    {
        printf("%s: cannot bind port TCP %u\n",argv[0],server_port);
        perror("error ");
        exit(1);
    }

    /* connect to server */
    rc = connect(sd, (struct sockaddr *) &servAddr, sizeof(servAddr));
    if(rc < 0) 
    {
        perror("cannot connect ");
        exit(1);
    }

// --------------------------- PATTERNS ---------------------------
    patterns[0] = "Enter starting amount of cash:";
    regcomp( &rxs[0], patterns[0], 0 );
    patterns[1] = "Enter number of decks:";
    regcomp( &rxs[1], patterns[1], 0 );

    patterns[2] = "(H)it (S)tand :";
    regcomp( &rxs[2], patterns[2], 0 );
    patterns[3] = "(H)it (S)tand (D)ouble :";
    regcomp( &rxs[3], patterns[3], 0 );
    patterns[4] = "(H)it (S)tand (D)ouble s(P)lit :";
    regcomp( &rxs[4], patterns[4], 0 );

    patterns[5] = "Place bet \\(\\$(.*) available, 0 to quit\\):";
    regcomp( &rxs[5], patterns[5], REG_EXTENDED );
    patterns[6] = "Dealer shows : (.)";
    regcomp( &rxs[6], patterns[6], REG_EXTENDED );
    patterns[7] = "You have : (.) (.) = ([0-9]+)";
    regcomp( &rxs[7], patterns[7], REG_EXTENDED );
    patterns[8] = "You have : .+ (.) = ([0-9]+)";
    regcomp( &rxs[8], patterns[8], REG_EXTENDED );
    patterns[9] = "You have : (.) (.) = soft ([0-9]+)";
    regcomp( &rxs[9], patterns[9], REG_EXTENDED );
    patterns[10] = "You have : .+ (.) = soft ([0-9]+)";
    regcomp( &rxs[10], patterns[10], REG_EXTENDED );
    patterns[11] = "Dealer has : (.+) (.) = [0-9]+";
    regcomp( &rxs[11], patterns[11], REG_EXTENDED );
    patterns[12] = ".*insurance.*";
    regcomp( &rxs[12], patterns[12], 0 );
    patterns[13] = "Your net cash change was:.*";
    regcomp( &rxs[13], patterns[13], 0 );
    patterns[14] = "Blackjack!! - You win.*";
    regcomp( &rxs[14], patterns[14], 0 );
    patterns[15] = "Dealer blackjack!!.*";
    regcomp( &rxs[15], patterns[15], 0 );
    patterns[16] = ".*Shuffling.*";
    regcomp( &rxs[16], patterns[16], 0 );
    patterns[17] = "(H)it (S)tand s(P)lit :";
    regcomp( &rxs[17], patterns[17], 0 );

    //------------------------User Input Prompts--------------------------
    printf("Insert base bet amount:\n");
    key_reader(buff);
    basebet = atoi(buff);
    
    printf("Insert maximum number of hands to play:\n");
    key_reader(buff);
    total_hands = atoi(buff);
    
    printf("Insert dollar amount to stop at:\n");
    key_reader(buff);
    stop_amount = atoi(buff);
    
    // This loop contains all of the game engine and goes until the program exits.
    while(1)
    {
        server_reader(sd, serverline);
        
        not_done = 1;
        for (i = 0; i < PATTERN_NUM && not_done; i++)
        {
            match = regexec( &rxs[i], serverline, n, p, 0 );

            if ( (i == PATTERN_NUM - 1) && match != 0 )
            {
                printf ("%s\n", serverline);
            }
            else if ( match == 0 )
            {
                not_done = 0;
                //printf ("regex match: %s\n", patterns[i]);
                if (i != 5)
                    printf ("%s\n", serverline);
                
                // Switch statement determines which regex pattern was matched and does the appropriate action.
                switch (i)
                {
                    // Starting cash
                    // # of Decks
                    case 0:
                        key_reader(buff);
                        total_cash = atoi(buff);
                        writer(sd, buff);
                        break;
                    case 1:
                        key_reader(buff);
                        num_cards = running_cards = atoi(buff) * 52;
                        writer(sd, buff);
                        break;
                    case 12:
                        if (count >= 2)
                        {
                            printf("client: y\n");
                            writer(sd, "y");
                        }
                        else
                        {
                            printf("client: n\n");
                            writer(sd, "n");
                        }
                        break;

                        // H S, H S D, H S D P
                    case 2:
                    case 3:
                    case 4:
                    case 17:
                        if (i == 2 || i == 17)
                        {
                            no_double = 1;
                        }
                        else
                        {
                            no_double = 0;
                        }

                        pc = atoi(player_count);
                        
                        if (strcmp(dealer_card1, "T") == 0 || strcmp(dealer_card1, "J") == 0 ||
                            strcmp(dealer_card1, "Q") == 0 || strcmp(dealer_card1, "K") == 0 )
                        {
                            strcpy( dealer_card1, "10" );
                        }
                        
                        if (strcmp(dealer_card1, "A") == 0)
                        {
                            strcpy( dealer_card1, "11" );
                        }
                        
                        dc = atoi(dealer_card1) - 2;
                        //printf ("client sees %d %d\n", pc, dc);

                        // for splits
                        if (i == 4)
                        {
                            int splitindex;
                            if (strcmp(player_card1, "T") == 0 || strcmp(player_card1, "J") == 0 ||
                                    strcmp(player_card1, "Q") == 0 || strcmp(player_card1, "K") == 0 )
                            {
                               // strcpy( player_card1, "10" );
                                splitindex = 10;
                            }
                            else if (strcmp(player_card1, "A") == 0)
                            {
                                splitindex = 1;
                                //strcpy( player_card1, "1" );
                            }
                            else
                            {
                                splitindex = atoi(player_card1);
                            }

                            if (splits[splitindex][dc] == "d" && no_double == 1)
                            {
                                printf ("client: cannot to double - h\n");
                                writer(sd, "h"); 
                            }
                            else
                            {
                                strcpy(buff, splits[splitindex][dc]);
                                printf ("client: %s\n", splits[splitindex][dc]);
                                if (strcmp( splits[splitindex][dc], "p") == 0)
                                {
                                    split_me = 1;
                                }
                                writer(sd, buff); 
                            }
                        }
                        else if (soft == 1)
                        { 
                            if (softv[pc][dc] == "d" && no_double == 1)
                            {
                                printf ("client detects soft: cannot double - h\n");
                                writer(sd, "h"); 
                            }
                            else
                            {
                                strcpy(buff, softv[pc][dc]);
                                printf ("client detects soft: %s\n", softv[pc][dc]);
                                writer(sd, buff); 
                            }
                        }
                        else 
                        {
                            if (hardv[pc][dc] == "d" && no_double == 1)
                            {
                                printf ("client: cannot double - h\n");
                                writer(sd, "h"); 
                            }
                            else
                            {
                                strcpy(buff, hardv[pc][dc]);
                                printf ("client: %s\n", hardv[pc][dc]);
                                writer(sd, buff); 
                            }
                        }
                        break;

                        // Placing Bet
                    case 5:
                        printf("client: Hand #%d Data\n", hands);
                        printf("client: ------------------\n");
                        printf("client: running count: %d\nclient: Remaining cards: %d\n", count, running_cards);
                                
                        split_me = 0;
                        int mult = 1;
                        strncpy( output, &serverline[ p[1].rm_so ], p[1].rm_eo - p[1].rm_so);
                        output [ p[1].rm_eo - p[1].rm_so ] = 0;
                        //printf( "what REGEX found: %s\n", output );
                        total_cash = atoi(output);
                        
                        if (total_cash < basebet || hands >= total_hands || total_cash >= stop_amount)
                        {
                            printf("\nclient: Program Finished\n");
                            printf( "client: %d total hands played\n", hands);
                            writer(sd, "0");
                        }
                        else
                        {
                            float true_count = count * 1.0 / (running_cards * 1.0 / 52 * 1.0);
                            if (true_count < 2.0)
                            {
                                mult = 1;
                            }
                            else if (true_count >= 2.0 && true_count < 4.0)
                            {
                                mult = 2;
                            }
                            else if (true_count >= 4.0 && true_count < 6.0)
                            {
                                mult = 3;
                            }
                            else if (true_count >= 6.0 && true_count < 8.0)
                            {
                                mult = 4;
                            }
                            else if (true_count >= 8.0)
                            {
                                mult = 5;
                            }

                            printf( "client: True Count is %f --> Multiplier is %d\n", true_count, mult);

                            printf ("\n%s\n", serverline);

                            if (total_cash < mult * basebet)
                            {
                                printf("client: not enough to bet, quitting\n");
                                writer(sd, "0");
                            }
                            else
                            {
                                sprintf(buff, "%d", mult * basebet);
                                printf("client: betting %s\n", buff);
                                writer(sd, buff);
                            }
                        }

                        hands++;
                        break;

                        // Dealer shows: 
                    case 6:
                        strncpy( dealer_card1, &serverline[ p[1].rm_so ], p[1].rm_eo - p[1].rm_so);
                        dealer_card1 [ p[1].rm_eo - p[1].rm_so ] = 0;
                        //printf( "what REGEX found: %s\n", dealer_card1 );
                        //printf( "multipliers: %s\n", dealer_card1 );
                        
                        multiplier = calc_multiplier( dealer_card1, &count, &running_cards );
                        break;

                        // You have : x x = xx
                    case 7:
                        strncpy( player_card1, &serverline[ p[1].rm_so ], p[1].rm_eo - p[1].rm_so);
                        strncpy( player_card2, &serverline[ p[2].rm_so ], p[2].rm_eo - p[2].rm_so);
                        strncpy( player_count, &serverline[ p[3].rm_so ], p[3].rm_eo - p[3].rm_so);
                        player_card1 [ p[1].rm_eo - p[1].rm_so ] = 0;
                        player_card2 [ p[2].rm_eo - p[2].rm_so ] = 0;
                        player_count [ p[3].rm_eo - p[3].rm_so ] = 0;
                        
                        if (atoi(player_count) >= 17)
                        {
                            strcpy(player_count, "17");
                        }
                        
                        if (!split_me)
                        {
                            multiplier = calc_multiplier( player_card1, &count, &running_cards );
                            //printf( "multipliers: %s %s\n", player_card1, player_card2 );
                        }
                        else
                        {
                            //printf( "just multiplier: %s\n", player_card2 );
                        }
                        multiplier = calc_multiplier( player_card2, &count, &running_cards );
                        
                        //printf( "what REGEX found: %s %s %s\n", player_card1, player_card2, player_count );
                        soft = 0;
                        break;

                        // You have : x* x = xx
                    case 8:
                        strncpy( player_card2, &serverline[ p[1].rm_so ], p[1].rm_eo - p[1].rm_so);
                        strncpy( player_count, &serverline[ p[2].rm_so ], p[2].rm_eo - p[2].rm_so);
                        player_card2 [ p[1].rm_eo - p[1].rm_so ] = 0;
                        player_count [ p[2].rm_eo - p[2].rm_so ] = 0;
                        if (atoi(player_count) >= 17)
                        {
                            strcpy(player_count, "17");
                        }
                        
                        multiplier = calc_multiplier( player_card2, &count, &running_cards );
                        
                        //printf( "what REGEX found: %s %s\n", player_card2, player_count );
                        //printf( "multipliers: %s\n", player_card2 );
                        soft = 0;
                        break;

                        // You have : x x = soft xx
                    case 9:
                        strncpy( player_card1, &serverline[ p[1].rm_so ], p[1].rm_eo - p[1].rm_so);
                        strncpy( player_card2, &serverline[ p[2].rm_so ], p[2].rm_eo - p[2].rm_so);
                        strncpy( player_count, &serverline[ p[3].rm_so ], p[3].rm_eo - p[3].rm_so);
                        player_card1 [ p[1].rm_eo - p[1].rm_so ] = 0;
                        player_card2 [ p[2].rm_eo - p[2].rm_so ] = 0;
                        player_count [ p[3].rm_eo - p[3].rm_so ] = 0;
                        if (atoi(player_count) >= 19)
                        {
                            strcpy(player_count, "19");
                        }
                        
                        if (!split_me)
                        {
                            multiplier = calc_multiplier( player_card1, &count, &running_cards );
                            //printf( "multipliers: %s %s\n", player_card1, player_card2 );
                        }
                        else 
                        {
                            //printf( "just multiplier: %s\n", player_card2 );
                        }
                        multiplier = calc_multiplier( player_card2, &count, &running_cards );
                        //printf( "what REGEX found: %s %s %s\n", player_card1, player_card2, player_count );
                        
                        soft = 1;
                        break;
                    
                        // You have : x* x = soft xx
                    case 10:
                        strncpy( player_card2, &serverline[ p[1].rm_so ], p[1].rm_eo - p[1].rm_so);
                        strncpy( player_count, &serverline[ p[2].rm_so ], p[2].rm_eo - p[2].rm_so);
                        player_card2 [ p[1].rm_eo - p[1].rm_so ] = 0;
                        player_count [ p[2].rm_eo - p[2].rm_so ] = 0;
                        if (atoi(player_count) >= 19)
                        {
                            strcpy(player_count, "19");
                        }
                        
                        multiplier = calc_multiplier( player_card2, &count, &running_cards );
                        
                        //printf( "what REGEX found: %s %s\n", player_card2, player_count );
                        //printf( "multipliers: %s\n", player_card2 );
                        soft = 1;
                        break;
                    
                        // Dealer has: x* x = xx
                    case 11:
                        strncpy( dealer_card1, &serverline[ p[1].rm_so ], p[1].rm_eo - p[1].rm_so);
                        strncpy( dealer_card2, &serverline[ p[2].rm_so ], p[2].rm_eo - p[2].rm_so);
                        dealer_card1 [ p[1].rm_eo - p[1].rm_so ] = 0;
                        dealer_card2 [ p[2].rm_eo - p[2].rm_so ] = 0;
                        
                        multiplier = calc_multiplier( dealer_card2, &count, &running_cards );
                        
                        //printf( "what REGEX found: %s %s\n", dealer_card1, dealer_card2 );
                        //printf( "multipliers: %s\n", dealer_card2 );
                        break;
                        
                    case 13:
                        exit(-1);
                        break;

                    case 14:
                    case 15:
                        multiplier = calc_multiplier( dealer_card1, &count, &running_cards );
                        //printf( "additional multipliers: %s\n", dealer_card1 );
                        break;
                    case 16:
                        printf( "client: resetting card count\n");
                        running_cards = num_cards;
                        count = 0;
                        break;
                        
                    default:
                        break;
                }
            }
        }
    }

    return 0;
}


// This function alters the running count and the remaining card count as necessary.
// The returning value is not used.
int calc_multiplier( char * card, int *count, int *running_cards )
{
    int card_value;
    int multiplier = 1;
    
    if (strcmp(card, "T") == 0 || strcmp(card, "J") == 0 ||
            strcmp(card, "Q") == 0 || strcmp(card, "K") == 0  || 
            strcmp(card, "A") == 0 )
    {
        card_value = 10;
    } 
    else
    {
        card_value = atoi(card);
    }
    
    switch (card_value)
    {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            (*count)++;
            break;

        case 10:
            (*count)--;
            break;
            
        default:
            break;
    }

    if (*count < 2)
    {
        multiplier = 1;
    }
    else if (*count >= 2 && *count < 4)
    {
        multiplier = 2;
    }
    else if (*count >= 4 && *count < 6)
    {
        multiplier = 3;
    }
    else if (*count >= 6 && *count < 8)
    {
        multiplier = 4;
    }
    else if (*count >= 8)
    {
        multiplier = 5;
    }
    (*running_cards)--;
    //printf ("Count is: %d\n", *count);
    //printf ("Number of cards left: %d\n", *running_cards);
    return multiplier;
}

// This function just reads input in from the keyboard.
void key_reader ( char * buff )
{
    int test = read( 0, buff, MAX_MSG);
    if (test <= 0)
    {
        printf("Error in reading input");
        exit(-1);
    }
}

// This function reads input in from the server
int server_reader ( int newSd, char * line_to_return )
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

// This function sends information to the server
void writer(int sd, char * data) 
{
    int rc = write(sd, data, strlen(data) + 1);
    if( rc < 0) 
    {
        perror("cannot send data ");
        close(sd);
        exit(1);
    }
    //  printf("%s: data sent\n", data);
}
