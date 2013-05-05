#include <iostream>
#include <fstream>
#include <climits>
#include <cstdlib>
#include <string>

using namespace std;

void outputPairs(string fileName, unsigned numRecords, unsigned numPairs)
{
    ofstream fileOutPairs;
    fileOutPairs.open(fileName.c_str());
    if(fileOutPairs.is_open())
    {
        fileOutPairs << numRecords << endl;
        fileOutPairs << numPairs << endl;
        for(unsigned i = 0; i < 100; i++)
        {
            for(unsigned j = i+1; j < 100; j++)
            {
                fileOutPairs << i << " " << j << endl;
            }
        }
        fileOutPairs.close();
    }
}

int main (int argc, char ** argv)
{
    unsigned numRecords = 100;
    unsigned numPairs = numRecords * (numRecords - 1) / 2;
    string fileName = "UniqueCombinations.txt";
    outputPairs(fileName, numRecords, numPairs);

    cout << "Number of records: " << numRecords << endl;
    cout << "Number of pairs  : " << numPairs << endl;
    cout << "Output           : \"" << fileName << "\"" << endl;

    return EXIT_SUCCESS;
}
