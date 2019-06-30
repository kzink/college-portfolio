//Kevin Zink
//Paul Kenyon - Operating Systems
//Program 3 - Virtual Page Table
//usage: a.out /u1/pkenyon/prog3.data.txt memory_size page_size FIFO/LRU

using namespace std;

#include<iostream.h>
#include<fstream.h>
#include<string>
#include<vector>
#include<function.h>

#define INPUT_ARRAY_SIZE 80

//--------------Page Entry Class-------------
class Entry
{
    public:
        // Default constructor
        // Entry(void)
        // -------------------
        //   input:  -
        //   return: -
        Entry(void)
        {
            count = 0;
            process_num = 0;
            page_num = 0;
        }
        
        // Entry(int p, int m, int time)
        // -------------------------------------
        //   input:  -
        //   return: -
        Entry(int p, int m, int time)
        {
            count = time;
            process_num = p;
            page_num = m;
        }
        
        // This function resets an objects data
        // void SetEntry(int p, int m, int time)
        // ---------------------------------------------
        //   input:  pid, page #, time 
        //   return: -
        void SetEntry(int p, int m, int time)
        {
            count = time;
            process_num = p;
            page_num = m;
        }
        
        // This function returns an objects counter
        // int CheckCount(void)
        // ----------------------------
        //   input:  -
        //   return: int
        int CheckCount(void)
        {
            return count;
        }
        
        // This function compares the object with the argument data
        // bool CheckEntry (const int p, const int m, const int time)
        // ------------------------------------------------------------------
        //   input:  pid, page #, time
        //   return: bool
        bool CheckEntry (const int p, const int m, const int time)
        {
            bool rval = false;
            if (m == page_num && p == process_num)
            {
                rval = true;    
                count = time; //if the entry matches
            }
            return rval;
        }

    private:
        int count;  //keeps track of objects counter for FIFO/LRU.
        int process_num; //the PID.
        int page_num; //the page # corresponding to the object.
};
        
//---------------Function Prototype----------------
void Parse(char *, int&, int&, int);

//---------------Main Function---------------------
int main (int argc, char* argv[])
{
    if (argc > 4)
    {
        char input[INPUT_ARRAY_SIZE];       //array storing input line.
        vector<Entry *> table;  //the page table.
        vector<Entry *>::iterator iter;
        vector<Entry *>::iterator replace = table.begin(); //used for LRU
        int table_size = atoi(argv[2])/atoi(argv[3]); //page table size.
        int page_size = atoi(argv[3]);  //page frame size.
        string type = argv[4]; //used as FIFO/LRU switch.
        int process;    //used for storing PID.
        int memory_add; 
        int lowest_count; //used to find a spot that is LRU.
        bool miss;
        int misses = 0;
        int count = 0;

        ifstream hobi (argv[1]); //gets name of file to use.

        //Loop runs till the end of the file.
        while (hobi.getline(input, INPUT_ARRAY_SIZE))
        {
            Parse(input, process, memory_add, page_size);
            miss = true;
            lowest_count = count;

            //This loop tests for a hit.
            iter = table.begin();
            while (iter != table.end() && miss)
            {
                if ((*iter)->CheckEntry(process, memory_add, count))
                    miss = false; //if it is a hit
                iter++;
            }

            //If there is no hit, the page is stored in the page table
            //and a replacement method is used when necessary
            if (miss)
            {
                misses++;

                //in the case that there is free room in the table
                if (static_cast<int>(table.size())< table_size)
                {
                    Entry * x = new Entry(process, memory_add, count);
                    table.push_back(x);
                }
                else if (type == "LRU")  //LRU replacement method.
                {
                    for (iter=table.begin(); iter != table.end(); ++iter)
                        if (lowest_count > (*iter)->CheckCount())
                        {
                            lowest_count = (*iter)->CheckCount();
                            replace = iter;
                        }
                    (*replace)->SetEntry(process, memory_add, count);
                }
                else    //FIFO replacement method.
                {
                    table[(count % table_size)]->
                        SetEntry(process, memory_add, count);
                }
            }
            //the counter is not incremented on hits in FIFO mode.
            //Otherwise, it always is.
            if (miss || type == "LRU")
                count++;
        }
        hobi.close();   //closes filestream
        cout << "total misses: " << misses << endl;
    }
    else
        cout << "Use syntax: a.out file mem_size page_size FIFO/LRU" << endl;
    
    return 0;
}

// This function parses the inputted line into the PID and address
// Then calculates the page # corresponding to the address
// void Parse(char * x, int& p, int& m, int size)
// ----------------------------------------------
//   input: array, ints, size of the page table 
//   return: none
void Parse(char * x, int& p, int& m, int size)
{
    char * tokenPtr = strtok(x, " "); //tokens out the file input
    p = atoi(tokenPtr);
    tokenPtr = strtok(NULL, " ");
    m = atoi(tokenPtr)/size;
}
