#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <string>

using namespace std;

unsigned gNumRecords = 0;

void outputPairs(string fileName, unsigned numRecords, unsigned numPairs)
{
    ofstream fileOutPairs;
    fileOutPairs.open(fileName.c_str());
    if(fileOutPairs.is_open())
    {
        fileOutPairs << numRecords << endl;
        fileOutPairs << numPairs << endl;
        for(unsigned i = 0; i < gNumRecords; i++)
        {
            for(unsigned j = i+1; j < gNumRecords; j++)
            {
                fileOutPairs << i << " " << j << endl;
            }
        }
        fileOutPairs.close();
    }
}

int main (int argc, char ** argv)
{
    if(argc != 2)
    {
        cerr << endl << "Missing numRecords command line argument" << endl;
        return EXIT_FAILURE;
    }
    gNumRecords = atoi(argv[1]);

    unsigned numPairs = gNumRecords * (gNumRecords - 1) / 2;
    string fileName = "UniqueCombinations.txt";
    outputPairs(fileName, gNumRecords, numPairs);

    cout << "Number of records: " << gNumRecords << endl;
    cout << "Number of pairs  : " << numPairs << endl;
    cout << "Output           : \"" << fileName << "\"" << endl;

    return EXIT_SUCCESS;
}
