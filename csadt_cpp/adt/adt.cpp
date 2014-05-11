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
#include "Precondition.h"
#include "FeatureStats.h"
#include "Rule.h"
#include "Utils.h"

using namespace std;

const unsigned MAX_FEATURE_COUNT = 128;
const unsigned NUM_COMMAND_LINE_ARGUMENTS = 2;

vector<Instance> gMatches;
vector<Instance> gNonMatches;

// for each attribute, there is a vector of Conditions
vector< vector<Condition> > gConditions;

vector<Precondition> gPreconditionsUsed;
Precondition gPreconditionChosen;
Condition gConditionChosen;
vector<Condition> gConditionsAlreadySelected;
vector<Condition> gPAndCChosen;
vector<Condition> gPandNotCChosen;
vector<Rule> gRules;

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

string DetermineUniqueConditionSymbols()
{
    string uniqueConditionSymbols = "";
    for(unsigned conditionSymbol = 0; conditionSymbol < eNumValidComparisons; conditionSymbol++)
    {
        uniqueConditionSymbols = uniqueConditionSymbols + sValidComparisons[conditionSymbol];
    }
    
    // remove duplicates
    std::sort(uniqueConditionSymbols.begin(), uniqueConditionSymbols.end());
    uniqueConditionSymbols.erase(std::unique(uniqueConditionSymbols.begin(), uniqueConditionSymbols.end()), uniqueConditionSymbols.end());
    
    return uniqueConditionSymbols;
} 

Condition ExtractCondition(const string& condition, const string& uniqueConditionSymbols)
{
    size_t found = condition.find_first_of(uniqueConditionSymbols);
    if(found == string::npos)
    {
        cerr << endl << "##### ERROR: Could not create condition for the following: " << condition;
        return Condition(0, "?", 0); 
    }

    unsigned conditionIndex = eSizePersonAttributes;
    string conditionVariableStr = condition.substr(0, found);
    string conditionSymbolStr = "";
    string conditionValueStr = "";

    // determine if the condition symbol is 1 or 2 characters long
    bool conditionIsTwoCharactersLong = false;
    for(unsigned conditionSymbol = 0; conditionSymbol < uniqueConditionSymbols.size(); conditionSymbol++)
    {
        if(condition[found+1] == uniqueConditionSymbols[conditionSymbol])
        {
            conditionIsTwoCharactersLong = true;
            break;
        }
    }

    if(conditionIsTwoCharactersLong)
    {
        conditionSymbolStr = condition.substr(found, 2);
        conditionValueStr = condition.substr(found + 2);
    }
    else
    {
        conditionSymbolStr = condition.substr(found, 1);
        conditionValueStr = condition.substr(found + 1);
    }

    // determine the condition index (wish we had dictionaries in C++)
    bool foundCondition = false;
    for(unsigned currentConditionIndex = 0; currentConditionIndex < eSizePersonAttributes; currentConditionIndex++)
    {
        if(conditionVariableStr == sPersonConditions[currentConditionIndex])
        {
            conditionIndex = currentConditionIndex;
            foundCondition = true;
            break;
        }
    }

    if(!foundCondition)
    {
        cerr << endl << "##### ERROR: Could not create condition for " << condition << " (invalid ... was not defind in Utils.h?)";
        return Condition(0, "?", 0); 
    }

    unsigned conditionValue = (unsigned)atoi(conditionValueStr.c_str());
    /*
    cerr << endl << "conditionVariableStr: " << conditionVariableStr;
    cerr << endl << "conditionSymbolStr  : " << conditionSymbolStr;
    cerr << endl << "conditionValueStr   : " << conditionValueStr;

    cerr << endl << "conditionValue      : " << conditionValue;
    cerr << endl << "conditionSymbolStr  : " << conditionSymbolStr;
    cerr << endl << "conditionIndex      : " << conditionIndex;
    */
    return Condition(conditionValue, conditionSymbolStr, conditionIndex);
}

void GenerateConditions()
{
    for(unsigned i = 0; i < eSizePersonAttributes; i++)
    {
        vector<Condition> dummy;
        gConditions.push_back(dummy);
    }

    bool populateConditionsFromFile = true;
    if(populateConditionsFromFile)
    {
        ifstream conditionsFileHandler;
        string conditionsFile = "../../csadt_python/conditions.txt";
        conditionsFileHandler.open(conditionsFile.c_str());

        vector<string> conditions;
        
        if(conditionsFileHandler.is_open())
        {
            string condition = "";
            string uniqueConditionSymbols = DetermineUniqueConditionSymbols();
            cerr << endl << "Possible comparison symbols:" << uniqueConditionSymbols;
            unsigned lineNumber = 0;
            while(!conditionsFileHandler.eof())
            {
                getline(conditionsFileHandler, condition);
                lineNumber++;
                if(condition == "")
                {
                    cerr << endl << "skipping blank line..." << endl;
                    continue;
                }
                Condition extractedCondition = ExtractCondition(condition, uniqueConditionSymbols);
                if(extractedCondition.getComparison() == "?")
                {
                    cerr << endl << "problem extracting condition from line:" << condition;
                    continue;
                }
                cerr << endl << "Adding the following condition: " << extractedCondition;
                gConditions.at(extractedCondition.getIndex()).push_back(extractedCondition);
            }
        }
        else
        {
            cerr << endl << "##### ERROR: failed to open " << conditionsFile;
        }

        conditionsFileHandler.close();
    }

    /*
    vector<FeatureStats> matchFeatureStats;
    PopulateFeatureStatus(gMatches, matchFeatureStats);
    //cerr << "Size of matchFeatureStats: " << matchFeatureStats.size() << endl;
    //cerr << matchFeatureStats.at(0);

    vector<FeatureStats> nonMatchFeatureStats;
    PopulateFeatureStatus(gNonMatches, nonMatchFeatureStats);
    //cerr << "Size of nonMatchFeatureStats: " << nonMatchFeatureStats.size() << endl;
    //cerr << nonMatchFeatureStats.at(0);

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
        cerr << endl << "Adding the following condition: " << gConditions.at(i).back();
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), "<", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), ">", i));
        cerr << endl << "Adding the following condition: " << gConditions.at(i).back();
    }
    for(unsigned i = 0; i < nonMatchFeatureStats.size(); i++)
    {
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), "==", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), "<", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), ">", i));
    }
    */
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

bool checkIfInstanceSatisfiesCondition(Instance myInstance, vector<Condition> conditionVector) {
    //assume logical operator between conditions is an "and", because "or" never used in this function
    bool satisfy = false;
    for (unsigned i = 0; i < conditionVector.size(); i++) {
        if (conditionVector.at(i).evaluate(myInstance)) {
            satisfy = true;          
        } else {
            satisfy = false;
            break;
        }
    }
    if (satisfy) {
        return true;
    } else {
        return false;
    }
}

vector<Instance> getNegInstancesThatSatisfyCondition(vector<Condition> conditionVector, string logicalOperator)
{
    vector<Instance> instancesThatSatisfy;
    bool satisfy = false;
    for (unsigned i = 0; i < gNonMatches.size(); i++)
    {
        for (unsigned j = 0; j < conditionVector.size(); j++)
        {
            if (logicalOperator == "and")
            {
                //used when an instance needs to satisfy more than one condition (ex. Both condition1 and conditon2 have to be true)
                if (conditionVector.at(j).evaluate(gNonMatches.at(i)))
                { 
                    satisfy = true;          
                }
                else
                {
                    satisfy = false;
                    break;
                }
            }
            else if (logicalOperator == "or")
            {
                //used only for "not p" where each condition of p is negated and joined by an or (De Morgan's Law:
                //    if not p = not(condtion1 and condition2), then not(condition1 and condition2) <-> not(condition1) or not(condition2))
                if (conditionVector.at(j).evaluate(gNonMatches.at(i)))
                { 
                    satisfy = true; 
                    break;         
                }
                else
                {
                    satisfy = false;
                }
            }
        }
        if (satisfy)
        {
            instancesThatSatisfy.push_back(gNonMatches.at(i)); 
        }
        satisfy = false;
    }
    return instancesThatSatisfy;
}

vector<Instance> getPosInstancesThatSatisfyCondition(vector<Condition> conditionVector, string logicalOperator)
{
    vector<Instance> instancesThatSatisfy;
    bool satisfy = false;
    for (unsigned i = 0; i < gMatches.size(); i++)
    {
        for (unsigned j = 0; j < conditionVector.size(); j++)
        {
            //used when an instance needs to satisfy more than one condition (ex. Both condition1 and conditon2 have to be true)
            if (logicalOperator == "and")
            {
                if (conditionVector.at(j).evaluate(gMatches.at(i)))
                { 
                    satisfy = true;          
                }
                else
                {
                    satisfy = false;
                    break;
                }
            }
            //used only for "not p" where each condition of p is negated and joined by an or
            //    (De Morgan's Law:  if not p = not(condtion1 and condition2), then not(condition1 and condition2) <-> not(condition1) or not(condition2))
            else if (logicalOperator == "or")
            {
                if (conditionVector.at(j).evaluate(gMatches.at(i)))
                { 
                    satisfy = true; 
                    break;         
                }
                else
                {
                    satisfy = false;
                }
            }
        }
        if(satisfy)
        {
            instancesThatSatisfy.push_back(gMatches.at(i)); 
        }
        satisfy = false;
    }
    return instancesThatSatisfy;
}

float wMinus(vector<Condition> conditionVector, string logicalOperator)
{
    vector<Instance> instancesThatSatisfy = getNegInstancesThatSatisfyCondition(conditionVector, logicalOperator);
    float sumOfWeights = 0.0;
    for(unsigned i = 0; i < instancesThatSatisfy.size(); i++)
    {
        sumOfWeights += instancesThatSatisfy.at(i).getWeight();
    }
    // FIVE
    // cerr << " ***** Condition: " << conditionVector.back() << " weightMinusSum: " << sumOfWeights << " (from " << instancesThatSatisfy.size() << " examples)" << endl;
    return sumOfWeights;
}

float wPlus(vector<Condition> conditionVector, string logicalOperator)
{
    vector<Instance> instancesThatSatisfy = getPosInstancesThatSatisfyCondition(conditionVector, logicalOperator);
    float sumOfWeights = 0.0;
    for(unsigned i = 0; i < instancesThatSatisfy.size(); i++)
    {
        sumOfWeights += instancesThatSatisfy.at(i).getWeight();
    }
    // FIVE
    // cerr << " ***** Condition: " << conditionVector.back() << " weightPlusSum: " << sumOfWeights << " (from " << instancesThatSatisfy.size() << " examples)" << endl;
    return sumOfWeights;
}

float calculateW(vector<Condition> conditionVector, string logicalOperator)
{
    return wPlus(conditionVector, logicalOperator) + wMinus(conditionVector, logicalOperator);
}
/*
def calculateZ(d1, d2):
    #d1 is ex. True or a condition which can be evaluated to True
    firstPart = math.sqrt( weightPlus( andConditions(d1, d2) ) * weightMinus( andConditions(d1, d2) ) )
    secondPart = math.sqrt( weightPlus( andConditions( d1, notCondition(d2) ) ) * weightMinus( andConditions( d1, notCondition(d2) ) ) )
    thirdPart = weight( notCondition(d1) )
    z = ( 2 * ( firstPart + secondPart ) ) + thirdPart
    return z
*/

float calcZ(const Precondition& p, Condition c)
{
    // p = d1 ; c = d2
    //prepare w, wPlus, and wMinus input
    vector<Condition> pAndC = p.GetConditions();
    pAndC.push_back(c);
    vector<Condition> pAndNotC = p.GetConditions();
    Condition notC = c;
    notC.setNotFlag(true);
    pAndNotC.push_back(notC);
    vector<Condition> notP = p.GetConditions();
    for (unsigned i = 0; i < notP.size(); i++)
    {
        notP.at(i).setNotFlag(true);
    }

    //calculate z equation
    float firstPart = sqrt( wPlus(pAndC, "and") * wMinus(pAndC, "and") );
    float secondPart = sqrt( wPlus(pAndNotC, "and") * wMinus(pAndNotC, "and") );
    float thirdPart = calculateW(notP, "or");
    float z = (2 * (firstPart + secondPart)) + thirdPart;

    return z;
}

void PrintConditionInfo()
{
    unsigned numConditions = 0;
    for(unsigned j = 0; j < gConditions.size(); j++)
    {
        numConditions += gConditions.at(j).size();
    }
    cerr << "Considering " << numConditions << " conditions... " << endl;

    for(unsigned j = 0; j < gConditions.size(); j++)
    {
        numConditions = numConditions + gConditions.at(j).size();
        cerr <<  " Conditions relating to " << sPersonConditions[j] << ":" << endl;
        for(unsigned k = 0; k < gConditions.at(j).size(); k++)
        {
            cerr << "   " << gConditions.at(j).at(k) << " ";
        }
        cerr << endl;
    }
}

void PrintGConditions()
{
    cerr << endl;
    cerr << endl <<  " ***** Available Conditions:";
    for(unsigned i = 0; i < gConditions.size(); i++)
    {
        for(unsigned j = 0; j < gConditions.at(i).size(); j++)
        {
            cerr << endl << " *********** " << gConditions.at(i).at(j);
        }
    }
    cerr << endl;
}

void computeArgMin()
{
    //Thanks for saying I had a shitty initialization :)
    float lowestZValue = 1000000.0;
    Precondition bestP;
    Condition bestC;
    bool setMin = false;
    unsigned bestCIndex1 = 0;
    unsigned bestCIndex2 = 0;

    //PrintConditionInfo();

    //find best preCondition and condition
    for(unsigned i = 0; i < gPreconditionsUsed.size(); i++)
    {
        //setMin = true; //<-I don't know why you added that
        bestCIndex1 = 0;
        bestCIndex2 = 0;
        for(unsigned j = 0; j < gConditions.size(); j++)
        {
            for(unsigned k = 0; k < gConditions.at(j).size(); k++)
            {
                bool conditionAlreadyUsed = false;
                for(unsigned x = 0; x < gConditionsAlreadySelected.size(); x++)
                {
                    if(gConditionsAlreadySelected.at(x) == gConditions.at(j).at(k))
                    {
                        cerr << " ENCOUNTERED ALIENS! " << gConditionsAlreadySelected.at(x) << endl;
                        conditionAlreadyUsed = true;
                        break;
                    }
                }
                if(conditionAlreadyUsed)
                {
                    continue;
                }

                if(setMin)
                {
                    lowestZValue = calcZ(gPreconditionsUsed.at(i), gConditions.at(j).at(k));
                    bestP = gPreconditionsUsed.at(i);
                    bestC = gConditions.at(j).at(k);
                    bestCIndex1 = j;
                    bestCIndex2 = k;
                    setMin = false;
                }
                else
                {
                    float z = calcZ(gPreconditionsUsed.at(i), gConditions.at(j).at(k));
                    /*
                    if (z < lowestZValue) {
                        cerr << "precondition " << i << ". condition " << j << " " << k << " | " << "z: " << z << " ... lowestZValue: " << lowestZValue << " <--Current Best" << endl;
                    } else {
                        cerr << "precondition " << i << ". condition " << j << " " << k << " | " << "z: " << z << " ... lowestZValue: " << lowestZValue << endl;
                    }
                    */
                    
                    if (z < lowestZValue)
                    {
                        lowestZValue = z;
                        bestP = gPreconditionsUsed.at(i);
                        bestC = gConditions.at(j).at(k);
                        bestCIndex1 = j;
                        bestCIndex2 = k;
                    }
                }
            }
        }
    }
    cerr <<  " ^^^^^^^^^^ lowestZValue: " << lowestZValue << endl; 
    gPreconditionChosen = bestP;
    gConditionChosen = bestC;

    gConditionsAlreadySelected.push_back(gConditionChosen);
    //PrintGConditions();
    /*cerr << endl << "Size of gConditionsAlreadySelected: " << gConditionsAlreadySelected.size();

    cerr << endl << "bestCIndex1: " << bestCIndex1 << " --- bestCIndex2: " << bestCIndex2;
    cerr << endl << "gConditions.size: " << gConditions.size();
    cerr << endl << "gConditions[" << bestCIndex1 << "].size: " << gConditions.at(bestCIndex1).size();
    cerr << endl << "JUST ADDED: " << gConditionsAlreadySelected.back() << endl;
    cerr << endl << "THIS SHOULD BE THE SAME: " << gConditions.at(bestCIndex1).at(bestCIndex2);
    
    cerr << endl << "Proposed index to remove   : " << gConditions.at(bestCIndex1).size() + bestCIndex2-1;
    cerr << endl << "Size we want to remove from: " << gConditions.at(bestCIndex1).size();*/

    // erase the 6th element
    //myvector.erase (myvector.begin()+5);

    /*gConditions.at(bestCIndex1).erase( gConditions.at(bestCIndex1).begin() + bestCIndex2);
    if(gConditions.at(bestCIndex1).size() == 0)
    {
        //gConditions.at(bestCIndex1).clear();
        gConditions.erase(gConditions.begin() + bestCIndex1);
        cerr << "##### WARNING: Removing condition vector concerning " << sPersonConditions[bestCIndex1] << endl;
    }
    cerr << endl << endl;
    string foo;
    PrintGConditions();*/
    //cin >> foo;
    //cerr << "Selected " << gConditionChosen << "... " << endl;
    //cerr << endl << endl;
}

bool isPreconditionInVectorOfPreconditions(Precondition& p, vector<Precondition>& vp) {
    for (unsigned i = 0; i < vp.size(); i++) {
        if (p == vp.at(i)) {
            return true;
        }
    }
    return false;
}

void createAndUpdategPAndCAndgPandNotC() {
    //Create p and c, p and not c so they can be used several times later.
    //I need to make copies. 
    vector<Condition> pAndC = gPreconditionChosen.GetConditions(); //hopefully this makes a copy of all conditions
    Condition c = gConditionChosen;
    pAndC.push_back(c);
    vector<Condition> pAndNotC = gPreconditionChosen.GetConditions();
    Condition notC = gConditionChosen;
    notC.setNotFlag(true);
    pAndNotC.push_back(notC);
    gPAndCChosen = pAndC;
    gPandNotCChosen = pAndNotC;
    //Add them to gPreconditionsUsed
    Precondition pAndCAsPrecondition = Precondition(pAndC);
    Precondition pAndNotCAsPrecondition = Precondition(pAndNotC);
    if (not isPreconditionInVectorOfPreconditions(pAndCAsPrecondition, gPreconditionsUsed)) {
        gPreconditionsUsed.push_back(pAndCAsPrecondition);
    }
    if (not isPreconditionInVectorOfPreconditions(pAndNotCAsPrecondition, gPreconditionsUsed)) {
        gPreconditionsUsed.push_back(pAndNotCAsPrecondition);
    }
}

float getScoreOfInstance(Instance myInstance)
{
    float score = 0.0;
    //cerr << "Instance:" << myInstance << endl;
    for (unsigned i = 0; i < gRules.size(); i++)
    {
        //cerr << "Rule: " << gRules.at(i) << endl;
        //cerr << "   before score: " << score << endl;
        vector<Condition> p = gRules.at(i).getPrecondition().GetConditions();
        if (checkIfInstanceSatisfiesCondition(myInstance, p))
        {
            vector<Condition> pAndC = p;
            Condition c = gRules.at(i).getCondition();
            pAndC.push_back(c);
            if (checkIfInstanceSatisfiesCondition(myInstance, pAndC))
            {
                score += gRules.at(i).getTrueScore();
            }
            else
            {
                score += gRules.at(i).getFalseScore();
            }
        }
        else
        {
            score += 0;
        }
    }
    //cerr << "    after score: " << score << endl;
    return score;
}

void DisplayWeights()
{
    cerr << "Match weights: " << endl;
    for (unsigned i = 0; i < gMatches.size(); i++) {
        cerr << gMatches.at(i).getWeight() << " ";
    }
    cerr << endl;
    cerr << "Non-match weights: " << endl;
    for (unsigned i = 0; i < gNonMatches.size(); i++) {
        cerr << gNonMatches.at(i).getWeight() << " ";
    }
    cerr << endl;
}

void updateWeights(float costPlus, float costMinus) {
    //cerr << "Before" << endl;
    //DisplayWeights();
    for (unsigned i = 0; i < gMatches.size(); i++) {
        float score = getScoreOfInstance(gMatches.at(i));
        //cerr << "score: " << score << endl;
        float yi = 1.0;
        float cost = costPlus * 1.0 + costMinus * 0.0;
        gMatches.at(i).setWeight(gMatches.at(i).getWeight() * cost * exp(-yi * score));
    }
    for (unsigned i = 0; i < gNonMatches.size(); i++) {
        float score = getScoreOfInstance(gNonMatches.at(i));
        float yi = -1.0;
        float cost = costPlus * 0.0 + costMinus * 1.0;
        gNonMatches.at(i).setWeight(gNonMatches.at(i).getWeight() * cost * exp(-yi * score));
    }
    //cerr << "After" << endl;
    //DisplayWeights();
}

void GenerateADT(float costPlus, float costMinus, unsigned numTreeNodes)
{
    //clear global vars that will be modified and used
    gPreconditionsUsed.clear();
    gPreconditionChosen.Clear();
    gConditionChosen = Condition();
    gPAndCChosen.clear();
    gPandNotCChosen.clear();
    gRules.clear();
    //set initial weights of instances
    double initialWeight = SetInitialWeights();
    //create initial rule
    Condition t = Condition(true);
    vector<Condition> tAsAVector;
    Precondition tAsAPrecondition;
    tAsAPrecondition.AddCondition(t);
    tAsAVector.push_back(t);
    float smoothFactor = 0.5 * (calculateW(tAsAVector, "and") / (gMatches.size() + gNonMatches.size()));
    cerr << "costPlus    : " << costPlus << endl;
    cerr << "costMinus   : " << costMinus << endl;
    cerr << "numTreeNodes: " << numTreeNodes << endl; 
    cerr << "smoothFactor: " << smoothFactor << endl;
    cerr << "wPlus(tAsAVector, and) : " << wPlus(tAsAVector, "and") << endl;
    cerr << "wMinus(tAsAVector, and): " << wMinus(tAsAVector, "and") << endl;

    cerr << "weight of True         : " << calculateW(tAsAVector, "and") << endl;
    cerr << "training data set size : " << gMatches.size() + gNonMatches.size() << endl;
    
    float alpha1 = 0.5 * log((costPlus * wPlus(tAsAVector, "and") + smoothFactor) / (costMinus * wMinus(tAsAVector, "and") + smoothFactor));
    float alpha2 = 0.0;


    gRules.push_back(Rule(tAsAPrecondition, t, alpha1, alpha2));
    //cerr << "##### ERROR: The number of conditions the preconditon has for the initial rule (should be 1): " << gRules.at(0).getPrecondition().size() << endl;
    gPreconditionsUsed.push_back(tAsAPrecondition);
    //create remaining rules
    for(unsigned i = 0; i < numTreeNodes; i++)
    {
        cerr << " ------------------------------------------------ " << endl;
        cerr << " ----- Iteration: " << i << " ------------------- " << endl;
        cerr << " ------------------------------------------------ " << endl;
        // smoothFactor = .5*(weight('True')/len(trainingDataSet))
        smoothFactor = .5 * (calculateW(tAsAVector, "and") / (gMatches.size() + gNonMatches.size()));

        //cerr << "calculateW(tAsAVector, and)       : " << calculateW(tAsAVector, "and") << endl;
        cerr << "gMatches.size() + gNonMatches.size(): " << gMatches.size() + gNonMatches.size() << endl;
        cerr << "smoothFactor   : " << smoothFactor << endl;
        computeArgMin();
        createAndUpdategPAndCAndgPandNotC();
        alpha1 = 0.5 * log( (costPlus * wPlus(gPAndCChosen, "and") + smoothFactor) / (costMinus * wMinus(gPAndCChosen, "and") + smoothFactor) );
        //cerr << "alpha1 numerator   : " << (costPlus * wPlus(gPAndCChosen, "and") + smoothFactor) << endl;
        //cerr << "alpha1 denominator : " << (costMinus * wMinus(gPAndCChosen, "and") + smoothFactor) << endl;
        cerr << "alpha1             : " << alpha1 << endl;

        alpha2 = 0.5 * log( (costPlus * wPlus(gPandNotCChosen, "and") + smoothFactor) / (costMinus * wMinus(gPandNotCChosen, "and") + smoothFactor) );
        //cerr << "alpha2 numerator   : " << (costPlus * wPlus(gPandNotCChosen, "and") + smoothFactor) << endl;
        //cerr << "alpha2 denominator : " << (costMinus * wMinus(gPandNotCChosen, "and") + smoothFactor) << endl;
        cerr << "alpha2      : " << alpha2 << endl;
        //cerr << "+++++++++++++++++++++++ Sending in this condition to gRules: " << gConditionsAlreadySelected.back() << endl;
        //gRules.push_back(Rule(gPreconditionChosen, gConditionChosen, alpha1, alpha2));
        gRules.push_back(Rule(gPreconditionChosen, gConditionsAlreadySelected.back(), alpha1, alpha2));
        //cerr << "+++++++++++++++++++++++ This rule was just added to gRules: " << gRules.back() << endl;
        updateWeights(costPlus, costMinus);

        //for (unsigned j = 0; j < gPreconditionsUsed.size(); j++) {
        //    cerr << i << "th iteration: gPreconditionsUsed[" << j << "]: " << gPreconditionsUsed.at(j) << endl;
        //}
    }

    cerr << endl;
    cerr << " ***** Selected Conditions:" << endl;
    for(unsigned i = 0; i < gConditionsAlreadySelected.size(); i++)
    {
        cerr << " ***** " << gConditionsAlreadySelected.at(i) << endl;
    }
    cerr << endl;
}

void usage()
{
    cerr << "Usage" << endl
        << "./adt [data_input_file.txt] [output_file.txt]" << endl
        << "./prepare-input ADT_ready.txt Tree.txt" << endl;
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
    string outputFileName = commandLineArgs[1];

	PopulateInstances(dataInputFileName);
	cerr << "gMatches size   : " << gMatches.size() << endl;
    cerr << "gNonMatches size: " << gNonMatches.size() << endl;
    cerr << gMatches.at(0);
    cerr << gNonMatches.at(0);

    GenerateConditions();
    PrintConditionInfo();

    //prepare input for GenerateADT and run it.
    float costPlus = 2.0f;
    float costMinus = 1.0f; 
    unsigned numTreeNodes = 10;

    //smoothFactor = 0.5 * (weight('True') / len(trainingDataSet))
    // THIS IS WHERE IT ALL HAPPENS
    GenerateADT(costPlus, costMinus, numTreeNodes);

    ofstream fout;
    fout.open(outputFileName.c_str());

    //print rules
    cerr << "\nCSADT Rules Output:";
    for(unsigned i = 0; i < gRules.size(); i++)
    {  
        cerr << "\nRule " << i << ": " << endl;
        
        ostringstream oss;
        // if the precondition has at least one condition
        if(gRules.at(i).getPrecondition().GetConditions().size() > 0)
        {
            oss << gRules.at(i).getPrecondition().GetConditions().at(0);
            for(unsigned j = 1; j < gRules.at(i).getPrecondition().GetConditions().size(); j++)
            {  
                oss << "and" << gRules.at(i).getPrecondition().GetConditions().at(j);
            }
        }
        // if the precondition has zero conditions
        string preCondition = oss.str();
        if(preCondition == "")
        {
            preCondition = "(True WTF)";
        }

        ostringstream oss2;
        oss2 << gRules.at(i).getCondition();
        string condition = oss2.str();
        float trueScore = gRules.at(i).getTrueScore();
        float falseScore = gRules.at(i).getFalseScore();
        
        cerr << "   precondition: " << preCondition << endl;
        cerr << "   condition   : " << condition << endl;
        cerr << "   true score  : " << trueScore << endl;
        cerr << "   false score : " << falseScore << endl;

        if(! fout.is_open())
        {
            cerr << endl << "##### ERROR: Unable to open " << outputFileName << ".";
            exit_now();
        }
        else
        {
            fout << preCondition << " " << condition << " " << trueScore << " " << falseScore;
            if((i+1) != gRules.size())
            {
                fout << endl;
            }
        }
    }
    if(fout.is_open())
    {
        fout.close();
    }

	cerr << endl << endl;
	return EXIT_SUCCESS;
}