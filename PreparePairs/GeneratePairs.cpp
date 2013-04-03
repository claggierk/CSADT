#include <boost/thread.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <sstream>

#include "Person.h"
#include "PersonDiff.h"
#include "RecordPair.h"
#include "Combination.h"

using namespace std;
using namespace boost; 

const unsigned NUM_CORES = 4;

unsigned gNumRecords = 0;
unsigned gNumCombinations = 0;

vector< vector<PersonDiff> > gPeopleDifferences;
vector<Combination> gPersonCombinations;
vector<Person> gPeople;

void comparePairOfRecords(unsigned startIndex, unsigned numCombinations, unsigned peopleDifferencesIndex)
{
    cerr << " --- startIndex     : " << startIndex << endl;
    cerr << " --- numCombinations: " << numCombinations << endl;
    for(unsigned i = startIndex; i < (startIndex+numCombinations); i++)
    {
        gPeopleDifferences.at(peopleDifferencesIndex).push_back(PersonDiff(gPeople.at(gPersonCombinations.at(i).getIndex1()), gPeople.at(gPersonCombinations.at(i).getIndex2())));
    }
}

Person CreatePerson(string personRecord)
{
	vector<string> personInfo;
	unsigned first = 0;
	unsigned found = unsigned(string::npos);
	do
	{
		found = personRecord.find("|", first);
		//cout << endl << "found: " << found;
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
        //cin >> first;
	} while(found != unsigned(string::npos));

	return Person(personInfo);
}

void PopulatePeople(const string& fileName)
{
    ifstream recordsFileHandler;
	recordsFileHandler.open(fileName.c_str());
	
	if(recordsFileHandler.is_open())
    {
        string personStr = "";
        while(!recordsFileHandler.eof())
        {
            getline(recordsFileHandler, personStr);
            gPeople.push_back(CreatePerson(personStr));
        }
        recordsFileHandler.close();
    }
    else
    {
        cerr << endl << " ##### ERROR: Unable to open " << fileName << "!";
    }
	//cout << people.at(0);
	//cout << people.back();
}

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
        
        getline(peopleCombinationsFileHandler, combinationStr1);
        gNumCombinations = atoi(combinationStr1.c_str());
        
        // initialize this vector so no resize is ever required
        gPersonCombinations.reserve(gNumCombinations);
        
        cerr << endl << gNumCombinations;
        
        string combinationStr2 = "";
        string combinationStr3 = "";
        getline(peopleCombinationsFileHandler, combinationStr1);
        while(!peopleCombinationsFileHandler.eof())
        {
            stringstream line(combinationStr1);
            line >> combinationStr2;
            line >> combinationStr3;
            cerr << endl << combinationStr2 << " " << combinationStr3;
            gPersonCombinations.push_back(Combination(atoi(combinationStr2.c_str()), atoi(combinationStr3.c_str())));
            getline(peopleCombinationsFileHandler, combinationStr1);
        }
        peopleCombinationsFileHandler.close();
        
        if(gPersonCombinations.size() != gNumCombinations)
        {
            cerr << endl << " ##### ERROR: number of combinations did not match what was expected";
            cerr << endl << " gPersonCombinations.size(): " << gPersonCombinations.size();
            cerr << endl << " gNumCombinations          : " << gNumCombinations;
        }
    }
    else
    {
        cerr << endl << " ##### ERROR: Unable to open " << fileName << "!";
    }
}

int main(int argc, char ** argv)
{
    PopulatePeopleCombinations("Pair_IDs.txt");
    unsigned comparisonsPerCore = gNumCombinations / NUM_CORES;
    
    PopulatePeople("100LinesPreProcessed.txt");
    
    gPeopleDifferences.resize(NUM_CORES);
    vector<thread*> threadPtrs(NUM_CORES);
    unsigned offset = 0;
    for(unsigned i = 0; i < NUM_CORES; i++)
    {
        gPeopleDifferences.at(i).reserve(comparisonsPerCore);
        threadPtrs.at(i) = new thread(comparePairOfRecords, offset, comparisonsPerCore, i);
        offset += comparisonsPerCore;
    }
    
    // do other stuff

    for(unsigned i = 0; i < NUM_CORES; i++)
    {
        threadPtrs.at(i)->join();
        delete threadPtrs.at(i);
        threadPtrs.at(i) = NULL;
    }
    
    for(unsigned i = 0; i < NUM_CORES; i++)
    {
        cout << endl << "Size of gPeopleDifferences[" << i << "]: " << gPeopleDifferences.at(i).size();
    }
    
    cout << endl << endl;
    return EXIT_SUCCESS;
}
