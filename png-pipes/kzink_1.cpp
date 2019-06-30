//Kevin Zink - CSCI 4500 - Paul Kenyon
//Prime Number Generator 

#include <iostream.h>
#include <unistd.h>
#include <stdlib.h>

//-----------Function Prototypes------------
void ProcessOne(int*, int);
void ProcessTwo(int*, int*);
void ProcessThree(int*, int*);
void ProcessFour(int*);
void WritePipe(int*, int);
int ReadPipe(int*, int &);

//-----------Main Program----------------------
int main (int argc, char* argv[])
{
    if (argc < 2)
    {
        cout << "No Inputted Number" << endl;
        exit (-1);
    }
    
    //Variable Declaractions
    int high = atoi(argv[1]); //stores inputted max number
    int p[2];       //pipes
    int p2[2];
    int p3[2];

    //First pipe/fork    
    pipe(p);
    int pid = fork();

    //-----------Process One--------------------    
    if (pid == -1)
    {
        cerr << "ERROR" << endl;
        exit(-1);
    }

    if (pid == 0)     
    {
        close(p[0]);
        ProcessOne(p, high);
        exit(0);
    }
    close(p[1]);
    
    pipe(p2);
    pid = fork();
    
    //-----------Process Two--------------------
    if (pid == -1)
    {
        cerr << "ERROR" << endl;
        exit(-1);
    }
    
    if (pid == 0)      
    {
        close(p2[0]);
        ProcessTwo(p, p2);        
        exit(0);
    }
    close (p2[1]);
     
    pipe(p3);
    pid = fork();

    //-----------Process Three------------------
    if (pid == -1)
    {
        cerr << "ERROR" << endl;
        exit(-1);
    }
    
    if (pid == 0)
    {
        close(p3[0]);
        ProcessThree(p2,p3);
        exit(0);
    }
    close(p3[1]);
    
    //---------------Process Four---------------
    cout << "Known Prime: 3" << endl;
    cout << "Known Prime: 5" << endl;
    cout << "Known Prime: 7" << endl;
    cout << "Known Prime: 11" << endl;
    cout << "Known Prime: 13" << endl;
    cout << "Known Prime: 17" << endl;
    
    ProcessFour(p3);
    
    return 0;
}

//void WritePipe(int* p, int temp)
//Writes the designated int to the designated pipe
//input: pointer to the pipe & the int to insert
//output: none
//return: none
void WritePipe(int* p, int temp)
{
    int rc = write(p[1], &temp, sizeof(int));
    if (rc != sizeof(temp))
    {
        cerr << "ERROR WRITING TO PIPE" << endl;
        exit(-1);
    }
}

//int ReadPipe(int* p, int &temp)
//Reads the next int from the designated pipe and stores it in var temp
//input: Pointer to pipe to read from and a call-by-reference to temp
//output: none
//return: error checking flag from the read function
int ReadPipe(int* p, int &temp)
{
    temp = 0;
    int rc = read(p[0], &temp, sizeof(int));
    if (rc != sizeof(temp) && temp != 0)
    {
        cerr << "ERROR READING FROM PIPE" << endl;
        exit(-1);
    }
    return rc;
}


//void ProcessOne(int* p, int high)
//Executes phase #1 of the program, generating all odd numbers
//up to the inputted number.
//input: pointer to the pipe and the inputted number
//output: none
//return: none
void ProcessOne(int* p, int high)
{
    for (int i=3; i<=high; i=i+2)
    {
        WritePipe(p, i);
    }
}

//void ProcessTwo(int* p, int* p2)
//Executes process #2, takes numbers from process #1's pipe
//and tests to see if they're divisible by 3 or 5.
//input: pointers to the pipes to read from and write to
//output: none
//return: none
void ProcessTwo(int* p, int* p2)
{
    int temp = 0;
    int rc = ReadPipe (p, temp);
    while (rc != 0) //loop runs till the pipe is empty
    {
        if (temp%3!=0 && temp%5!=0)
        {
            WritePipe(p2, temp);
        }
        rc = ReadPipe (p, temp);
    }
}

//void ProcessThree(int* p2, int* p3)
//Executes process #3. Tests to see if the #'s from process #2's pipe are 
//divisible by 7, 11, 13, and 17. Those that are not are put into the 3rd pipe
//input: pointers to the pipes to read from and write to
//output: none
//return: none
void ProcessThree(int* p2, int* p3)
{
    int temp = 0;
    int rc = ReadPipe (p2, temp);

    while (rc != 0)
    {
        if (temp%7!=0 && temp%11!=0 &&temp%13!=0 && temp%17!=0)
        {
            WritePipe (p3, temp);
        }

        rc = ReadPipe (p2, temp);
    }
}

//void ProcessFour(int* p3)
//Executes process #4. Reads from process #3's pipe and compares the
//numbers against known primes. If a prime is found, it is stored as 
//a known prime.
//input: pointer to the pipe to read from
//output: Known primes
//return: none
void ProcessFour(int* p3)
{
    int *primes;    //array to store primes
    primes = new int[100];
    int size_of_array = 100;
    int loc_in_array = 0;
    int temp;

    int rc = ReadPipe(p3, temp); 
    
    while (rc != 0)
    {
        int i = 0;
        bool isprime = true;

        //This loop tests the # against all known primes
        while (i < loc_in_array && isprime)
        {
            if (temp % primes[i] == 0)
                isprime = false;
            i++;
        }

        //If the number is prime, then store it in the prime array        
        //and output it to cout
        if (isprime)
        {
            cout << "Prime: " << temp << endl;

            //This conditional increases the array's size when necessary.
            if (loc_in_array+1 == size_of_array)
            {
                int nsize = size_of_array + 100;
                int *narray = new int[nsize];
                for (int i=0; i < size_of_array; i++)
                {
                    narray[i] = primes[i];
                }
                delete []primes;
                primes = narray;
                size_of_array = nsize;
            }

            primes[loc_in_array] = temp;    //stores new prime
            loc_in_array++;
        }
        
        rc = ReadPipe(p3, temp);  //gets next # from pipe
    }
}
