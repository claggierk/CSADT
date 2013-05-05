#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "PersonDiff.h"
#include "Person.h"
#include "Instance.h"

using namespace std;

vector<Instance> gInstances;

void PopulatePersonPairsCompared(string fileName)
{
	ifstream fin;
	fin.open(fileName.c_str());

    vector<unsigned> inputData;
    inputData.reserve(eSizePersonAttributes);
	
	if(fin.is_open())
    {
    	string line = "";
        getline(fin, line);
        unsigned currentIndex = 0;
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
        		}
        	}
            
            inputData.clear();
            iss >> tempStr;
            do
            {
            	inputData.push_back(atoi(tempStr.c_str()));
                iss >> tempStr;
            } while(iss);
            
            gInstances.push_back(Instance(inputData, currentIndex, match));
            getline(fin, line);
            currentIndex++;
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
    // gInstances is m
    unsigned totalMatches = 0;
    for(unsigned i = 0; i < gInstances.size(); i++)
    {
        if(gInstances.at(i).isMatch())
        {
            totalMatches++;
        }
    }

    float matchWeight = (float)totalMatches / (float)gInstances.size();
    float nonMatchWeight = ((float)gInstances.size() - (float)totalMatches) / (float)gInstances.size();
    cout << "totalMatches  : " << totalMatches << endl;
    cout << "matchWeight   : " << matchWeight << endl;
    cout << "nonMatchWeight: " << nonMatchWeight << endl;
    float currentWeight = 0.0f;
    for(unsigned i = 0; i < gInstances.size(); i++)
    {
        currentWeight = nonMatchWeight;
        if(gInstances.at(i).isMatch())
        {
            currentWeight = matchWeight;
        }
        gInstances.at(i).setWeight(currentWeight);
    }
}

void GenerateADT()
{
    SetInitialWeights();
    cout << gInstances.at(0) << endl;
}

int main(int argc, char* argv[])
{
	PopulatePersonPairsCompared("input.txt");
	cout << "gInstances size: " << gInstances.size() << endl;

    GenerateADT();

	cout << endl << endl;
	return EXIT_SUCCESS;
}