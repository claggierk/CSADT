#include <boost/thread.hpp>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>

#include "Person.h"
#include "RecordPair.h"

using namespace std;
using namespace boost; 

const unsigned NUM_CORES = 4;

void comparePairOfRecords(unsigned startIndex, unsigned num)
{
    cerr << " --- thread: " << startIndex << endl;
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

vector<Person> PopulatePeople(string fileName)
{
    ifstream recordsFileHandler;
	recordsFileHandler.open(fileName.c_str());
    
	vector<Person> people;
	string personStr = "";
	if(recordsFileHandler.is_open())
    {
        while(!recordsFileHandler.eof())
        {
            getline(recordsFileHandler, personStr);
            people.push_back(CreatePerson(personStr));
        }
        recordsFileHandler.close();
    }
	cout << people.at(0);
	cout << people.back();
    
	return people;
}

int main (int argc, char ** argv)
{
    vector<Person> people = PopulatePeople("100LinesPreProcessed.txt");
    
    unsigned numRecords = 100;
    unsigned recordsPerCore = numRecords / NUM_CORES;
    
    vector<thread*> threadPtrs(NUM_CORES);    
    unsigned offset = 0;
    for(unsigned i = 0; i < NUM_CORES; i++)
    {
        threadPtrs.at(i) = new thread(comparePairOfRecords, offset, recordsPerCore);
        offset += recordsPerCore;
    }
    
    // do other stuff

    for(unsigned i = 0; i < NUM_CORES; i++)
    {
        threadPtrs.at(i)->join();
        delete threadPtrs.at(i);
        threadPtrs.at(i) = NULL;
    }

    return EXIT_SUCCESS;
}
