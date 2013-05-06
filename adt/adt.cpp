#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>

#include "Instance.h"
#include "Condition.h"
#include "FeatureStats.h"

using namespace std;

const unsigned MAX_FEATURE_COUNT = 128;

vector<Instance> gMatches;
vector<Instance> gNonMatches;
vector<Condition> gConditions;

void PopulateInstances(string fileName)
{
	ifstream fin;
	fin.open(fileName.c_str());

    vector<unsigned> inputData;
    inputData.reserve(MAX_FEATURE_COUNT);
	
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
            if(tempStr != "0" && tempStr != "1")
            {
            	cerr << " ##### ERROR: Invalid line: \"" << line << "\" in input file: " << fileName << endl;
            	continue;
        	}
        	else
        	{
        		if(tempStr == "1")
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
            
            if(match)
            {
                // only the match instances
                gMatches.push_back(Instance(inputData, currentIndex, match));
            }
            else
            {
                // only the non match instances
                gNonMatches.push_back(Instance(inputData, currentIndex, match));
            }
            
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
    float matchWeight = (double)gMatches.size() / ((double)gMatches.size() + (double)gNonMatches.size());
    float nonMatchWeight = (double)gNonMatches.size() / ((double)gMatches.size() + (double)gNonMatches.size());
    cout << "Initial match weight    : " << matchWeight << endl;
    cout << "Initial non-match weight: " << nonMatchWeight << endl;
    
    for(unsigned i = 0; i < gMatches.size(); i++)
    {
        gMatches.at(i).setWeight(matchWeight);
    }

    for(unsigned i = 0; i < gNonMatches.size(); i++)
    {
        gNonMatches.at(i).setWeight(nonMatchWeight);
    }
}

void PopulateFeatureStatus(const vector<Instance>& instances, vector<FeatureStats>& fs)
{
    vector<unsigned> attributes = instances.at(0).getAttributes();
    for(unsigned j = 0; j < attributes.size(); j++)
    {
                                //myIndex, mySum,      mySqSum,                             myMin             myMax,            myMean, myStdDev
        fs.push_back(FeatureStats(j, attributes.at(j), attributes.at(j) * attributes.at(j), attributes.at(j), attributes.at(j), 0.0f, 0.0f));
    }

    for(unsigned i = 1; i < instances.size(); i++)
    {
        vector<unsigned> attributes = instances.at(i).getAttributes();
        for(unsigned j = 0; j < attributes.size(); j++)
        {
            // update the sum
            fs.at(j).setSum(fs.at(j).getSum() + attributes.at(j));

            // update the squared sum
            fs.at(j).setSqSum(fs.at(j).getSqSum() + attributes.at(j) * attributes.at(j));            

            // update the min (if its smaller)
            if(attributes.at(j) < fs.at(j).getMin())
            {
                fs.at(j).setMin(attributes.at(j));
            }

            // update the max (if its smaller)
            if(attributes.at(j) > fs.at(j).getMax())
            {
                fs.at(j).setMax(attributes.at(j));
            }
        }
    }

    for(unsigned i = 0; i < fs.size(); i++)
    {
        fs.at(i).setMean((double)fs.at(i).getSum() / (double)instances.size());
        fs.at(i).setStdDev((double)fs.at(i).getSqSum() / (double)instances.size());
    }
}

void GenerateConditions()
{
    vector<FeatureStats> matchFeatureStats;
    PopulateFeatureStatus(gMatches, matchFeatureStats);
    cout << matchFeatureStats.at(0);
    for(unsigned i = 0; i < matchFeatureStats.size(); i++)
    {
        gConditions.push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), '=', i));
        gConditions.push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), '<', i));
        gConditions.push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), '>', i));
    }

    vector<FeatureStats> nonMatchFeatureStats;
    PopulateFeatureStatus(gNonMatches, nonMatchFeatureStats);
    cout << nonMatchFeatureStats.at(0);
    for(unsigned i = 0; i < nonMatchFeatureStats.size(); i++)
    {
        gConditions.push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), '=', i));
        gConditions.push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), '<', i));
        gConditions.push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), '>', i));
    }
}

void GenerateADT()
{
    SetInitialWeights();
    //cout << gMatches.at(0) << endl;
    //cout << gNonMatches.at(0) << endl;
}

int main(int argc, char* argv[])
{
	PopulateInstances("input.txt");
	cout << "gMatches size   : " << gMatches.size() << endl;
    cout << "gNonMatches size: " << gNonMatches.size() << endl;
    cout << gMatches.at(0);
    cout << gNonMatches.at(0);

    GenerateConditions();
    /*
    for(unsigned i = 0; i < gConditions.size(); i++)
    {
        cout << gConditions.at(i);
    }
    */
    GenerateADT();

	cout << endl << endl;
	return EXIT_SUCCESS;
}