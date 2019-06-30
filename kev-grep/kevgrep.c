// Kevin Zink
// Account: kzink

// This program is a light version of grep that uses regexes to match lines.
// The regex portion of the code is partially from John Clark's "last" command program.

#include<stdio.h>
#include<stdlib.h>
#include<regex.h>
#include<string.h>

int main(int argc, char ** argv)
{
    int invert = 0;
    int silent = 0;
    int casei = 0;
    int i; //loop counter
    int match;
    int count = 0;
    char input[256];
    char line[256];
    size_t n = 5;
    regex_t rx;
    regmatch_t p[5];
    FILE * fp = stdin; // default input is from standard in
    
    if (argc < 2)
    {
        printf("\nSyntax is as follows:\n\t%s -icv -f [file] [pattern]\n\n", argv[0]);
        exit(-1);
    }

    // Command-line handling
    for (i = 1; i < argc; ++i)
    {
        if (!strncmp("-i", argv[i], 2))
        {
            casei = 1;
        }
        else if (!strncmp("-c", argv[i], 2))
        {
            silent = 1;
        }
        else if (!strncmp("-v", argv[i], 2))
        {
            invert = 1;
        }
        else if (!strncmp("-f", argv[i], 2))
        {
            fp = fopen(argv[++i], "r");
            if ( !fp ) // if the file descriptor doesn't exist
            {
                printf("\nError: Invalid File\n\n");
                exit(-1);
            }
        }
        else if (i == argc - 1)
        {
            strcpy(input, argv[i]);
        }
        else
        {
            printf("\nSyntax is as follows:\n\t%s -icv -f [file] [pattern]\n\n", argv[0]);
            exit(-1);
        }
    }

    // Regex pattern string
    char pattern[] = ".*";
    strcat(pattern, input);
    strcat(pattern, ".*$");
    
    if (casei) // for case insensitive compares
    {
        regcomp( &rx, pattern, REG_ICASE );
    }
    else
    {
        regcomp( &rx, pattern, 0 );
    }

    // This loop goes through input line by line and checks for matches
    while( fgets( line, sizeof( line ) - 1, fp ) )
    {
        match = regexec( &rx, line, n, p, 0 );
        
        if ( !match && !invert )
        {
            if ( !silent )
            {
                printf( "%s", line );
            }
            else
            {
                count++;
            }
        }
        else if ( match && invert )
        {
            if ( !silent )
            {
                printf( "%s", line );
            }
            else
            {
                count++;
            }
        }
    }

    if (silent)
    {
        printf( "\nNumber of matches: %d\n\n", count );
    }
    
    regfree( &rx );
    return 0;
}
