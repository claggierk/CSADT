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

const unsigned NUM_THREADS = 4;

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

	found = personRecord.find("|", first);
    if(found != std::string::npos) // found a '|'
    {
        idStr = personRecord.substr(first, found-first);
        id = atoi(idStr.c_str());
        first = found + 1;
    }
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
        getline(recordsFileHandler, personStr); // dont want to store the FIRST one...
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
        for(unsigned i = 0; i < NUM_THREADS; i++)
        {
			for(unsigned j = 0; j < gPeopleDifferences.at(i).size(); j++)
			{
				if(gPeopleDifferences.at(i).at(j).isMatch())
					fout << "T ";
				else
					fout << "F ";
				
				vector<unsigned> temp = gPeopleDifferences.at(i).at(j).getDifferences();
				for(unsigned k = 0; k < temp.size(); k++)
					fout << temp.at(k) << " ";
				
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

int main(int argc, char ** argv)
{
    PopulatePeopleCombinations("UniqueCombinations.txt");
    unsigned comparisonsPerCore = gNumCombinations / NUM_THREADS;
    
    PopulatePeople("100LinesPreProcessed.txt");
    
    gPeopleDifferences.resize(NUM_THREADS);
    vector<thread*> threadPtrs(NUM_THREADS);
    unsigned offset = 0;
    
    const clock_t begin_time = clock();
    for(unsigned i = 0; i < NUM_THREADS; i++)
    {
        gPeopleDifferences.at(i).reserve(comparisonsPerCore);
        threadPtrs.at(i) = new thread(comparePairOfRecords, offset, comparisonsPerCore, i);
        offset += comparisonsPerCore;
        usleep(1000); // microseconds
    }
    
    // do other stuff

    for(unsigned i = 0; i < NUM_THREADS; i++)
    {
        threadPtrs.at(i)->join();
        delete threadPtrs.at(i);
        threadPtrs.at(i) = NULL;
    }
    cout << "All threads complete" << endl;
    cout << "Seconds required for comparing records: " << float( clock () - begin_time ) /  CLOCKS_PER_SEC << endl;
    
    for(unsigned i = 0; i < NUM_THREADS; i++)
    {
        cout << endl << "Size of gPeopleDifferences[" << i << "]: " << gPeopleDifferences.at(i).size();
    }
    
    cout << gPeople.at(0) << endl;
    cout << gPeople.at(1) << endl;
    cout << gPeople.at(2) << endl;
    cout << gPeopleDifferences.at(0).at(0) << endl;
    cout << gPeopleDifferences.at(0).at(1) << endl;
    
    OutputPeopleDifferences("input.txt");
    
    cout << endl << endl;
    return EXIT_SUCCESS;
}
