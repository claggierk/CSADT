#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "PersonDiff.h"

using namespace std;

vector<PersonDiff> PersonPairsCompared;

void PopulatePersonPairsCompared(string fileName)
{
	ifstream fin;
	fin.open(fileName.c_str());
	
	if(fin.is_open())
    {
    	string line = "";
        while(!fin.eof())
        {
            getline(fin, line);
            istringstream iss(line);
            
            bool match = false;
            string tempStr = "";
            iss >> tempStr;
            if(tempStr != "T" && tempStr != "F")
            {
            	cerr << " ##### ERROR: Invalid line: \"" << line << "\" in input file: " << fileName << endl;
            	continue;
        	}
        	else
        	{
        		if(tempStr == "T")
        		{
        			match = true;
        		}
        	}
            
            vector<unsigned> tempDifferences;
            while(iss)
            {
            	iss >> tempStr;
            	tempDifferences.push_back(atoi(tempStr.c_str()));
            }
            PersonPairsCompared.push_back(PersonDiff(tempDifferences, match));
        }
    }
    else
    {
    	cerr << endl << " ##### ERROR: Unable to open " << fileName << "!";
    }
	
	fin.close();
}

int main(int argc, char* argv[])
{
	PopulatePersonPairsCompared("PersonToPersonComparisons.txt");
	cout << "PersonPairsCompared size: " << PersonPairsCompared.size();

	cout << endl << endl;
	return EXIT_SUCCESS;
}