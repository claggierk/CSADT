#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

#include "Instance.h"
#include "Condition.h"
#include "FeatureStats.h"
#include "Rule.h"

using namespace std;

const unsigned MAX_FEATURE_COUNT = 128;

vector<Instance> gMatches;
vector<Instance> gNonMatches;
vector< vector<Condition> > gConditions;

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
    cout << "Size of matchFeatureStats: " << matchFeatureStats.size() << endl;
    cout << matchFeatureStats.at(0);

    vector<FeatureStats> nonMatchFeatureStats;
    PopulateFeatureStatus(gNonMatches, nonMatchFeatureStats);
    cout << "Size of nonMatchFeatureStats: " << nonMatchFeatureStats.size() << endl;
    cout << nonMatchFeatureStats.at(0);

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
    }


    for(unsigned i = 0; i < matchFeatureStats.size(); i++)
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

double SetInitialWeights()
{   
    double totalInstances = static_cast<double>(gMatches.size()) + static_cast<double>(gNonMatches.size());
    double initialWeight = 1.0 / totalInstances;
    for(unsigned i = 0; i < gMatches.size(); i++)
    {
        gMatches.at(i).setWeight(initialWeight);
    }

    for(unsigned i = 0; i < gNonMatches.size(); i++)
    {
        gNonMatches.at(i).setWeight(initialWeight);
    }

    return initialWeight;
}

void GenerateADT()
{
    double initialWeight = SetInitialWeights();
    //cout << gMatches.at(0) << endl;
    //cout << gNonMatches.at(0) << endl;

    //------------NOTES added 7/7/13--------------- Stan wrote:
    //it doesn't matter if conditions are capable of being compared with each other
    //need to have a mechanism that tells me if an instance satisfies a condition (does it have something to do with features.
    //   like instead of "FullName < 2", the condition has the fields  value = myValue; comparison = <; index = myIndex; where value can be compared to instance.attributes.at(i)?
    //   to see if the instance satisfies the condition.
    
    //------------NOTES added 7/8/13--------------- Clark wrote
    // responding to the request above:
    // an instance                : gMatches.at(0)
    // an attribute of an instance: gMatches.at(0).getAttribute(0) 
    // several conditions: gConditions.at(0) is a vector of conditions ... that all have to do with attribute[0]
    cout << gMatches.at(0) << endl;
    for(unsigned i = 0; i < gConditions.at(0).size(); i++)
    {
        cout << "Comparing: " << gMatches.at(0).getAttribute(0) << " --- " << gConditions.at(0).at(i) << endl;
        cout << "   " << gConditions.at(0).at(i).evaluate(gMatches.at(0).getAttribute(0)) << endl;
    }

    //turn wPlus and wMinus into functions (capable of taking in either a condition or a preCondition)
    //  I can do that ... need to understand what will go into those functions
    double wPlus = initialWeight * static_cast<double>(gMatches.size());
    double wMinus = initialWeight * static_cast<double>(gNonMatches.size());
    float a = 0.5 * log(wPlus / wMinus);

    //create a vector of conditions that's been selected to be used and a vector of preConditions that's been selected to be used---each preCondition is a vector of Conditions
    //  gConditions is a vector of vector of conditions...
    //    gConditions.at(0) is a vector of conditions who all deal with an instance's first attribute (FullName... remember, adt has no perspective of what the data is about)
    //    gConditions.at(1) is a vector of conditions who all deal with an instance's second attribute (FirstName)
    //    gConditions.at(2) is a vector of conditions who all deal with an instance's third attribute (LastName)

    //change rule- a precondition is not a condition, but vector of conditions --- DONE
    vector<Rule> rules;
    vector<Condition> dummy;
    dummy.push_back(Condition(true));
    rules.push_back(Rule(dummy, Condition(true), a, 0.0f));
}

int main(int argc, char* argv[])
{
	PopulateInstances("input.txt");
	cout << "gMatches size   : " << gMatches.size() << endl;
    cout << "gNonMatches size: " << gNonMatches.size() << endl;
    cout << gMatches.at(0);
    cout << gNonMatches.at(0);

    GenerateConditions();
    GenerateADT();

	cout << endl << endl;
	return EXIT_SUCCESS;
}