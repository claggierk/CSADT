#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "PersonDiff.h"
#include "Person.h"

using namespace std;

vector<PersonDiff> PersonPairsCompared;

void PopulatePersonPairsCompared(string fileName)
{
	ifstream fin;
	fin.open(fileName.c_str());

    vector<unsigned> tempDifferences;
    tempDifferences.reserve(eSizePersonAttributes);
	
	if(fin.is_open())
    {
    	string line = "";
        getline(fin, line);
        do
        {
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
                    cout << "MATCH" << endl;
        		}
        	}
            
            tempDifferences.clear();
            iss >> tempStr;
            do
            {
            	tempDifferences.push_back(atoi(tempStr.c_str()));
                iss >> tempStr;
            } while(iss);
            
            PersonPairsCompared.push_back(PersonDiff(tempDifferences, match));
            getline(fin, line);
        } while(!fin.eof());
    }
    else
    {
    	cerr << endl << " ##### ERROR: Unable to open " << fileName << "!";
    }
	
	fin.close();
}

void SetInitialWeights()
{
    // PersonPairsCompared is m
    unsigned totalMatches = 0;
    for(unsigned i = 0; i < PersonPairsCompared.size(); i++)
    {
        if(PersonPairsCompared.at(i).isMatch())
        {
            totalMatches++;
        }
    }

    float matchWeight = (float)totalMatches / (float)PersonPairsCompared.size();
    float nonMatchWeight = ((float)PersonPairsCompared.size() - (float)totalMatches) / (float)PersonPairsCompared.size();
    cout << "totalMatches  : " << totalMatches << endl;
    cout << "matchWeight   : " << matchWeight << endl;
    cout << "nonMatchWeight: " << nonMatchWeight << endl;
    float currentWeight = 0.0f;
    for(unsigned i = 0; i < PersonPairsCompared.size(); i++)
    {
        currentWeight = nonMatchWeight;
        if(PersonPairsCompared.at(i).isMatch())
        {
            currentWeight = matchWeight;
        }
        PersonPairsCompared.at(i).setWeight(currentWeight);
    }
}

void GenerateADT()
{
    SetInitialWeights();
    cout << PersonPairsCompared.at(0);
}

int main(int argc, char* argv[])
{
	PopulatePersonPairsCompared("PersonToPersonComparisons.txt");
	cout << "PersonPairsCompared size: " << PersonPairsCompared.size();

    GenerateADT();

	cout << endl << endl;
	return EXIT_SUCCESS;
}