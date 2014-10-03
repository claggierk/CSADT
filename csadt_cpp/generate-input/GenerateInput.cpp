#include <boost/thread.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>
#include <ctime>

#include "Person.h"
#include "PersonDiff.h"
#include "RecordPair.h"
#include "Combination.h"

using namespace std;
using namespace boost; 

const unsigned NUM_THREADS = 8;
const unsigned NUM_COMMAND_LINE_ARGUMENTS = 3;

unsigned gNumRecords = 0;
unsigned gNumCombinations = 0;

vector< vector<PersonDiff> > gPeopleDifferences;
vector<Combination> gPersonCombinations;
vector<Person> gPeople;

void comparePairOfRecords(unsigned startIndex, unsigned numCombinations, unsigned peopleDifferencesIndex)
{
    cout << endl;
    cout << " ***** Starting Thread" << endl;
    cout << "    --- startIndex     : " << startIndex << endl;
    cout << "    --- numCombinations: " << numCombinations << endl;
    for(unsigned i = startIndex; i < (startIndex+numCombinations); i++)
    {
        gPeopleDifferences.at(peopleDifferencesIndex).push_back(PersonDiff(gPeople.at(gPersonCombinations.at(i).getIndex1()), gPeople.at(gPersonCombinations.at(i).getIndex2())));
    }
}

Person CreatePerson(string personRecord, unsigned index)
{
	vector<string> personInfo;
	
    unsigned id = 0;
    string idStr = "";
    unsigned first = 0;
	unsigned found = unsigned(string::npos);

    do
	{
		found = personRecord.find("|", first);
		if(found != std::string::npos) // found a '|'
		{
			personInfo.push_back(personRecord.substr(first, found-first));
			first = found + 1;
		}
		else
		{
            cout << endl << "last one...";
			personInfo.push_back(personRecord.substr(first, personRecord.size()-1));
		}
	} while(found != unsigned(string::npos));

    idStr = personRecord.substr(first, found-first);

    // get the last one! THE UNIQUE IDENTIFIER IS ALWAYS AT THE END (last cell in a row)
    id = atoi(personInfo.at(personInfo.size()-1).c_str());

	Person person(personInfo, index, id);
    return person;
}

void PopulatePeople(const string& fileName)
{
    ifstream recordsFileHandler;
	recordsFileHandler.open(fileName.c_str());
	
	if(recordsFileHandler.is_open())
    {
        string personStr = "";
        getline(recordsFileHandler, personStr); // dont want to store the FIRST one... as it is the header!
        unsigned personIndex = 0;
        while(!recordsFileHandler.eof())
        {
            getline(recordsFileHandler, personStr);
            gPeople.push_back(CreatePerson(personStr, personIndex));
            personIndex++;
        }
        recordsFileHandler.close();
    }
    else
    {
        cerr << endl << " ##### ERROR: Unable to open " << fileName << "!";
        cerr << endl;
        exit(1);
    }
	//cout << people.at(0);
	//cout << people.back();
}

// populates gPersonCombinations
void PopulatePeopleCombinations(const string& fileName)
{
    ifstream peopleCombinationsFileHandler;
	peopleCombinationsFileHandler.open(fileName.c_str());
    
    if(peopleCombinationsFileHandler.is_open())
    {
        string combinationStr1 = "";
        getline(peopleCombinationsFileHandler, combinationStr1);
        gNumRecords = atoi(combinationStr1.c_str());
        
        // initialize this vector so no resize is ever required
        gPeople.reserve(gNumRecords);
        cout << "Number of records    : " << gNumRecords << endl;
        
        getline(peopleCombinationsFileHandler, combinationStr1);
        gNumCombinations = atoi(combinationStr1.c_str());
        
        // initialize this vector so no resize is ever required
        gPersonCombinations.reserve(gNumCombinations);
        
        cout << "Number of record pairs: " << gNumCombinations << endl;
        
        string combinationStr2 = "";
        string combinationStr3 = "";
        getline(peopleCombinationsFileHandler, combinationStr1);
        while(!peopleCombinationsFileHandler.eof())
        {
            stringstream line(combinationStr1);
            line >> combinationStr2;
            line >> combinationStr3;
            ////cout << endl << combinationStr2 << " " << combinationStr3;
            gPersonCombinations.push_back(Combination(atoi(combinationStr2.c_str()), atoi(combinationStr3.c_str())));
            getline(peopleCombinationsFileHandler, combinationStr1);
        }
        
        if(gPersonCombinations.size() != gNumCombinations)
        {
            cerr << endl << " ##### ERROR: number of combinations did not match what was expected";
            cerr << endl << "              gPersonCombinations.size(): " << gPersonCombinations.size();
            cerr << endl << "              gNumCombinations          : " << gNumCombinations;
        }
    }
    else
    {
        cerr << endl << " ##### ERROR: Unable to open " << fileName << "!";
        cerr << endl;
        exit(1);
    }
    peopleCombinationsFileHandler.close();
}

void OutputPeopleDifferences(string fileName)
{
	ofstream fout;
	fout.open(fileName.c_str());
    
    if(fout.is_open())
    {
        for(unsigned i = 0; i < NUM_THREADS+1; i++)
        {
			for(unsigned j = 0; j < gPeopleDifferences.at(i).size(); j++)
			{
				fout << gPeopleDifferences.at(i).at(j).isMatch() << " ";
				
				vector<unsigned> temp = gPeopleDifferences.at(i).at(j).getDifferences();
				for(unsigned k = 0; k < temp.size(); k++)
                {
					fout << temp.at(k) << " ";
				}
				fout << endl;
			}
		}
	}
	else
	{
		cerr << " ##### ERROR: Unable to open " << fileName << "!";
	}
	
	fout.close();
}

void usage()
{
    cerr << "Usage" << endl
        << "./prepare-input [combinations_input_file.txt] [data_input_file.txt] [output_file.txt]" << endl
        << "./prepare-input UniqueCombinations.txt 100LinesPreProcessed.txt ADT_ready.txt" << endl;
}

void exit_now()
{
    cerr << endl
        << "Exiting..." << endl
        << endl;
    exit(EXIT_FAILURE);
}

vector<string> DeriveCommandLineArguments(int argc, char** argv, unsigned requiredCommandLineArgs)
{
    vector<string> commandLineArgs;
    for(int i = 1; i < argc; i++)
    {
        commandLineArgs.push_back(argv[i]);
        cerr << endl << commandLineArgs.back() << endl;
    }
    if(commandLineArgs.size() != requiredCommandLineArgs)
    {
        cerr << "Expected " << requiredCommandLineArgs << " command line arguments, but received " << commandLineArgs.size() << "..." << endl;
        usage();
        exit_now();
    }
    return commandLineArgs;
}

int main(int argc, char** argv)
{
    vector<string> commandLineArgs = DeriveCommandLineArguments(argc, argv, NUM_COMMAND_LINE_ARGUMENTS);

    string combinationsFileName = commandLineArgs[0];
    string dataInputFileName = commandLineArgs[1];
    string outputFileName = commandLineArgs[2];

    PopulatePeopleCombinations(combinationsFileName);
    unsigned comparisonsPerCore = gNumCombinations / NUM_THREADS;
    unsigned remainderComparisons = gNumCombinations - (comparisonsPerCore * NUM_THREADS);
    
    PopulatePeople(dataInputFileName);
    
    gPeopleDifferences.resize(NUM_THREADS+1);
    vector<thread*> threadPtrs(NUM_THREADS+1);
    unsigned offset = 0;
    
    const clock_t begin_time = clock();
    
    // kick off all the threads that have comparisonsPerCore to do
    unsigned i = 0;
    for(i = 0; i < NUM_THREADS; i++)
    {
        gPeopleDifferences.at(i).reserve(comparisonsPerCore);
        threadPtrs.at(i) = new thread(comparePairOfRecords, offset, comparisonsPerCore, i);
        offset += comparisonsPerCore;
        usleep(1000); // microseconds
    }

    // kick off the last thread representing the remainder
    gPeopleDifferences.at(i).reserve(comparisonsPerCore);
    threadPtrs.at(i) = new thread(comparePairOfRecords, offset, remainderComparisons, i);
    offset += remainderComparisons;
    usleep(1000); // microseconds

    // do whatever you want to while the threads are going

    // wait for all the threads to finish...
    for(unsigned i = 0; i < NUM_THREADS+1; i++)
    {
        threadPtrs.at(i)->join();
        delete threadPtrs.at(i);
        threadPtrs.at(i) = NULL;
    }
    // at this point ... all the threads are finished!

    cout << "All threads complete" << endl;
    cout << "Seconds required for comparing records: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    
    for(unsigned i = 0; i < NUM_THREADS+1; i++)
    {
        cout << endl << "Size of gPeopleDifferences[" << i << "]: " << gPeopleDifferences.at(i).size();
    }
    
    //cerr << gPeople.at(0) << endl;
    //cerr << gPeople.at(1) << endl;
    //cerr << gPeople.at(2) << endl;
    //cerr << gPeopleDifferences.at(0).at(0) << endl;
    //cerr << gPeopleDifferences.at(0).at(1) << endl;
    //cerr << gPeopleDifferences.at(0).at(2) << endl;
    //cerr << gPeopleDifferences.at(0).at(3) << endl;
    
    OutputPeopleDifferences(outputFileName);
    
    cout << endl << endl;
    return EXIT_SUCCESS;
}
