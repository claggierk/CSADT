#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <sstream>
#include <algorithm>
#include <csignal>
#include <execinfo.h>
#include <unistd.h>

#include "Instance.h"
#include "Condition.h"
#include "FeatureStats.h"
#include "Utils.h"

using namespace std;

const unsigned MAX_FEATURE_COUNT = 128;
const unsigned NUM_COMMAND_LINE_ARGUMENTS = 2;

vector<Instance> gMatches;
vector<Instance> gNonMatches;

// for each attribute, there is a vector of Conditions
vector< vector<Condition> > gConditions;

void exit_now()
{
    cerr << endl
        << "Exiting..." << endl
        << endl;
    exit(EXIT_FAILURE);
}

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
            // the unique identifier of records must be the FIRST cell in a row!
            istringstream iss(line);
            
            bool match = false;
            string tempStr = "";
            iss >> tempStr;
            if(tempStr != "0" && tempStr != "1")
            {
            	cerr << "##### ERROR: Invalid line: \"" << line << "\" in input file: " << fileName << endl;
            	continue;
        	}
        	else
        	{
                // a match is represented by a 1 (we set it to 1)
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
        cout << "Filed in " << currentIndex << " instances..." << endl;
        cout << "Filed in " << gMatches.size() + gNonMatches.size() << " instances..." << endl;
    }
    else
    {
    	cerr << endl << "##### ERROR: Unable to open " << fileName << "!";
        exit_now();
    }
	
	fin.close();
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
    for(unsigned i = 0; i < eSizePersonAttributes; i++)
    {
        vector<Condition> dummy;
        gConditions.push_back(dummy);
    }

    vector<FeatureStats> matchFeatureStats;
    PopulateFeatureStatus(gMatches, matchFeatureStats);

    vector<FeatureStats> nonMatchFeatureStats;
    PopulateFeatureStatus(gNonMatches, nonMatchFeatureStats);

    if(matchFeatureStats.size() == nonMatchFeatureStats.size())
    {
        unsigned numAttributes = matchFeatureStats.size();
        for(unsigned i = 0; i < numAttributes; i++)
        {
            vector<Condition> dummy;
            gConditions.push_back(dummy);
        }
    }
    else
    {
        cerr << "##### ERROR: number of attributes disagreement" << endl;
        exit_now();
    }
    
    for(unsigned i = 1; i < matchFeatureStats.size(); i++)
    {
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), "==", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), "<", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), ">", i));
    }
    for(unsigned i = 0; i < nonMatchFeatureStats.size(); i++)
    {
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), "==", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), "<", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), ">", i));
    }
}

void usage()
{
    cerr << "Usage" << endl
        << "./generate-conditions [data_input_file.txt] [output_file.txt]" << endl
        << "./prepare-input ADT_ready.txt Conditions.txt" << endl;
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

void handler(int sig)
{
    void *array[10];
    size_t size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 10);

    // print out all the frames to stderr
    fprintf(stderr, "Error: signal %d:\n", sig);
    backtrace_symbols_fd(array, size, STDERR_FILENO);
    exit(1);
}

int main(int argc, char* argv[])
{
    signal(SIGSEGV, handler);
    vector<string> commandLineArgs = DeriveCommandLineArguments(argc, argv, NUM_COMMAND_LINE_ARGUMENTS);

    string dataInputFileName = commandLineArgs[0];
    string conditionsOutputFileName = commandLineArgs[1];

    PopulateInstances(dataInputFileName);
    cerr << "gMatches size   : " << gMatches.size() << endl;
    cerr << "gNonMatches size: " << gNonMatches.size() << endl;
    cerr << gMatches.at(0);
    cerr << gNonMatches.at(0);

    GenerateConditions();

    ofstream fout;
    fout.open(conditionsOutputFileName.c_str());

    for(unsigned i = 0; i < gConditions.size(); i++)
    {
        for(unsigned j = 0; j < gConditions.at(i).size(); j++)
        {
            fout << gConditions.at(i).at(j) << endl;
        }
    }

    if(fout.is_open())
    {
        fout.close();
    }

	cerr << endl << endl;
	return EXIT_SUCCESS;
}
