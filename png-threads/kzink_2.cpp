//Kevin Zink - CSCI 4500 - Paul Kenyon
//Prime Number Generator #2 - Threads

using namespace std;

#include <iostream.h>
#include <unistd.h>
#include <stdlib.h>
#include <deque>
#include <pthread.h>

//-----------Globals---------------------------
deque<int> p1, p2, p3; //deques that the threads will use
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; //mutexes
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t condition1 = PTHREAD_COND_INITIALIZER; //signal conditions
pthread_cond_t condition2 = PTHREAD_COND_INITIALIZER;
pthread_cond_t condition3 = PTHREAD_COND_INITIALIZER;
int high; //inputted highest number

//-----------Function Prototypes---------------
void * ProcessOne(void *);
void * ProcessTwo(void *);
void * ProcessThree(void *);
void ProcessFour(void);
void WriteDeque(deque<int> &, pthread_mutex_t &, pthread_cond_t &, int);
int ReadDeque(deque<int> &, pthread_mutex_t &, pthread_cond_t &);
void ErrorCheck(int);

//-----------Main Program----------------------
int main (int argc, char* argv[])
{
    //----------File IO stuff------------------
    if (argc < 2)
    {
        cout << "No Inputted Number" << endl;
        exit (-1);
    }
    
    //-----------Variable Declaractions--------
    high = atoi(argv[1]); //stores inputted max number
    pthread_t thread1, thread2, thread3;

    //-----------Thread One--------------------    
    int rc = pthread_create(&thread1, 0, &ProcessOne, 0);
    ErrorCheck(rc);
    
    //-----------Thread Two--------------------
    rc = pthread_create(&thread2, 0, &ProcessTwo, 0);
    ErrorCheck(rc);
     
    //-----------Thread Three------------------
    rc = pthread_create(&thread3, 0, &ProcessThree, 0);
    ErrorCheck(rc);
    
    //-----------Process Four------------------
    ProcessFour();
    
    return 0;
}

// void ErrorCheck(int x)
// -------------------------------------
//   Small function to error check on mutux locks, unlocks, and cond signals
//   input:  None
//   output: message if an error occurs
//   return: None
void ErrorCheck(int x)
{
    if (x != 0)
    {
        cerr << "MAJOR ERROR" << endl;
        exit(-1);
    }
}

// void WriteDeque(deque<int> &p, pthread_mutex_t &mutex, pthread_cond_t &condition, int temp)
// -------------------------------------------------------------------------------------------
//   This function does all the writing for this program.  It locks the imported mutex, writes
//   to the respective deque, unlocks the mutex, and then signals that a write has taken place.
//   input:  deque &, mutex &, signal condition &, and the int to write
//   output: none
//   return: none
void WriteDeque(deque<int> &p, pthread_mutex_t &mutex, pthread_cond_t &condition, int temp)
{
    int rc = pthread_mutex_lock(&mutex); //secure mutex
    ErrorCheck(rc);    //error checking for mutex locking
    
    p.push_back(temp);    //adds int to the deque
    rc = pthread_mutex_unlock(&mutex);  //unlocks mutex 
    ErrorCheck(rc);

    rc = pthread_cond_signal(&condition);    //signals a write has taken placE
    ErrorCheck(rc);
}


// int ReadDeque(deque<int> &p, pthread_mutex_t &mutex, pthread_cond_t &condition)
// -------------------------------------------------------------------------------
//   This function does all the reading for this program.  It locks the imported mutex,
//   and if the deque is empty, it waits for something to be written to it.  Then, it
//   grabs the top value from the deque and removes it.  Finally, it lets go of the mutex
//   and returns the int that it retrieved off the deque.
//   input:  deque &, mutex &, condition signal &
//   output: none
//   return: an int from the front of the deque
int ReadDeque(deque<int> &p, pthread_mutex_t &mutex, pthread_cond_t &condition)
{
    int rval = -1;
    int rc = pthread_mutex_lock(&mutex);
    ErrorCheck(rc);
    
    //this loop makes the thread wait till something has been written to the deque.
    while (p.empty())
    {
        rc = pthread_cond_wait(&condition, &mutex);  //wait statement - thread awakes after signal
        ErrorCheck(rc);
    }
    
    rval = p.front();   //grabs the front element off the deque.
    p.pop_front();      //removes the front element from the deque.

    rc = pthread_mutex_unlock(&mutex);
    ErrorCheck(rc);
    return rval;
}

// void * ProcessOne(void * arg)
// -----------------------------
//   This function simply writes all the odd numbers up to the inputted value to 
//   the first deque.
//   input:  none
//   output: none
//   return: none
void * ProcessOne(void * arg)
{
    for (int i=3; i<=high; i=i+2)
    {
        WriteDeque(p1, mutex1, condition1, i);
    }
    
    //this throws a -1 onto the deque to let the next thread know when this one is done writing.
    WriteDeque(p1, mutex1, condition1, -1);
    return 0;
}

// void * ProcessTwo(void * arg)
// -----------------------------
//   This function reads from the 1st deque and tests each element against 3 & 5, writing
//   only those not divisible by 3 & 5 to the deque #2.
//   output: none
//   input:  none
//   return: none
void * ProcessTwo(void * arg)
{
    int temp;

    //this loop goes till it reads a -1, which means the previous thread is done writing.
    while ( (temp = ReadDeque(p1, mutex1, condition1)) != -1) 
    {
        if (temp%3!=0 && temp%5!=0) 
        {
            WriteDeque(p2, mutex2, condition2, temp);
        }
    }
    
    //lets the next thread know this one is done.
    WriteDeque(p2, mutex2, condition2, -1); 
    return 0;
}
    
// void * ProcessThree(void * arg)
// -------------------------------
//   This function reads from deque #2 and tests each element against 7, 11, 13, & 17.
//   Those numbers not divisible are written to deque #3
//   output: none
//   input:  none
//   return: none
void * ProcessThree(void * arg)
{
    int temp;

    while ( (temp = ReadDeque(p2, mutex2, condition2)) != -1)
    {
        if (temp%7!=0 && temp%11!=0 && temp%13!=0 && temp%17!=0)
        {
            WriteDeque (p3, mutex3, condition3, temp);
        }
    }
    
    WriteDeque(p3, mutex3, condition3, -1);
    return 0;
}

// void ProcessFour(void)
// ----------------------
//   This function reads from deque #3 and tests each element against all other known primes
//   which are also discovered by this function.  Numbers that are discovered to be prime are
//   outputted and written to a deque designated for new primes.  
//   input:  none
//   output: primes
//   return: none
void ProcessFour(void)
{
    deque<int> primes;
    deque<int>::iterator Iter;
    int temp;
    
    //hardwired output of known primes
    cout << "Known Prime: 3" << endl;
    cout << "Known Prime: 5" << endl;
    cout << "Known Prime: 7" << endl;
    cout << "Known Prime: 11" << endl;
    cout << "Known Prime: 13" << endl;
    cout << "Known Prime: 17" << endl;
    
    while ( (temp = ReadDeque(p3, mutex3, condition3)) != -1)
    {
        bool isprime = true;

        //This loop tests the # against all known primes in the primes deque by using
        //an iterator to step through the primes deque
        for (Iter = primes.begin(); Iter != primes.end() && isprime; ++Iter) 
        {
            if (temp % *Iter == 0)
                isprime = false;    //if the number is divisible by a prime, it's not a prime.
        }

        //If the number is prime, then store it in the prime deque        
        //and output it with cout
        if (isprime)
        {
            cout << "Prime: " << temp << endl;
            primes.push_back(temp);    //stores new prime
        }
    }
}
