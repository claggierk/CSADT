#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <sstream>

#include "Instance.h"
#include "Condition.h"
#include "Precondition.h"
#include "FeatureStats.h"
#include "Rule.h"

using namespace std;

const unsigned MAX_FEATURE_COUNT = 128;
const unsigned NUM_COMMAND_LINE_ARGUMENTS = 2;

vector<Instance> gMatches;
vector<Instance> gNonMatches;
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


    for(unsigned i = 0; i < matchFeatureStats.size(); i++)
    {
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), "==", i));
        //gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), "<", i));
        gConditions.at(i).push_back(Condition(static_cast<unsigned>(matchFeatureStats.at(i).getMean()+0.5f), ">", i));
    }

    for(unsigned i = 0; i < nonMatchFeatureStats.size(); i++)
    {
        //gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), "==", i));
        //gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), "<", i));
        //gConditions.at(i).push_back(Condition(static_cast<unsigned>(nonMatchFeatureStats.at(i).getMean()+0.5f), ">", i));
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
            {  //used when an instance needs to satisfy more than one condition (ex. Both condition1 and conditon2 have to be true)
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
            { //used only for "not p" where each condition of p is negated and joined by an or (De Morgan's Law:  if not p = not(condtion1 and condition2), then not(condition1 and condition2) <-> not(condition1) or not(condition2))
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
    vector<Condition> pAndC = p.getConditions();
    pAndC.push_back(c);
    vector<Condition> pAndNotC = p.getConditions();
    Condition notC = c;
    notC.setNotFlag(true);
    pAndNotC.push_back(notC);
    vector<Condition> notP = p.getConditions();
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
        cerr <<  "*****";
        for(unsigned k = 0; k < gConditions.at(j).size(); k++)
        {
            cerr << gConditions.at(j).at(k) << " ";
        }
        cerr << endl;
    }
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
    gPreconditionChosen = bestP;
    gConditionChosen = bestC;    

    gConditionsAlreadySelected.push_back(gConditionChosen);
    //cerr << "JUST ADDED: " << gConditionsAlreadySelected.back() << endl;
    
    gConditions.at(bestCIndex1).erase(gConditions.at(bestCIndex1).begin() + bestCIndex2);
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
    vector<Condition> pAndC = gPreconditionChosen.getConditions(); //hopefully this makes a copy of all conditions
    Condition c = gConditionChosen;
    pAndC.push_back(c);
    vector<Condition> pAndNotC = gPreconditionChosen.getConditions();
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
    for (unsigned i = 0; i < gRules.size(); i++)
    {
        vector<Condition> p = gRules.at(i).getPrecondition().getConditions();
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
    return score;
}

void updateWeights(float costPlus, float costMinus) {
    for (unsigned i = 0; i < gMatches.size(); i++) {
        float score = getScoreOfInstance(gMatches.at(i));
        float yi = 1.0;
        float cost = costPlus * 1.0 + costMinus * 0.0;
        gMatches.at(i).setWeight(gMatches.at(i).getWeight()*exp(-yi*score));
    }
    for (unsigned i = 0; i < gNonMatches.size(); i++) {
        float score = getScoreOfInstance(gNonMatches.at(i));
        float yi = -1.0;
        float cost = costPlus * 0.0 + costMinus * 1.0;
        gNonMatches.at(i).setWeight(gNonMatches.at(i).getWeight() * cost * exp(-yi*score));
    }    
}

void GenerateADT(float costPlus, float costMinus, unsigned numTreeNodes)
{
    //clear global vars that will be modified and used
    gPreconditionsUsed.clear();
    gPreconditionChosen.clear();
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
    tAsAPrecondition.addCondition(t);
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
    cerr << "alpha1      : " << alpha1 << endl;

    gRules.push_back(Rule(tAsAPrecondition, t, alpha1, alpha2));
    //cerr << "##### ERROR: The number of conditions the preconditon has for the initial rule (should be 1): " << gRules.at(0).getPrecondition().size() << endl;
    gPreconditionsUsed.push_back(tAsAPrecondition);
    //create remaining rules
    for(unsigned i = 0; i < numTreeNodes; i++)
    {
        cerr << endl << "--New tree iteration--" << endl;

        smoothFactor = .5 * (calculateW(tAsAVector, "and")/(gMatches.size() + gNonMatches.size()));
        computeArgMin();
        createAndUpdategPAndCAndgPandNotC();
        alpha1 = 0.5 * log((costPlus * wPlus(gPAndCChosen, "and") + smoothFactor)/(costMinus * wMinus(gPAndCChosen, "and") + smoothFactor));
        alpha2 = 0.5 * log((costPlus * wPlus(gPandNotCChosen, "and") + smoothFactor)/(costMinus * wMinus(gPandNotCChosen, "and") + smoothFactor));
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

int main(int argc, char* argv[])
{
    vector<string> commandLineArgs = DeriveCommandLineArguments(argc, argv, NUM_COMMAND_LINE_ARGUMENTS);

    string dataInputFileName = commandLineArgs[0];
    string outputFileName = commandLineArgs[1];

	PopulateInstances(dataInputFileName);
	cerr << "gMatches size   : " << gMatches.size() << endl;
    cerr << "gNonMatches size: " << gNonMatches.size() << endl;
    cerr << gMatches.at(0);
    cerr << gNonMatches.at(0);

    GenerateConditions();

    //adjust gMatches, gNonMatches to have roughly a magnitude of order less number of instances (in order to speed it up)
    gMatches.resize(9);
    gNonMatches.resize(480);
    cerr << " ## WARNING: The # of matches and nonmatches have been reduced." << endl << endl;

    //prepare input for GenerateADT and run it.
    float costPlus = 2.0f;
    float costMinus = 1.0f; 
    unsigned numTreeNodes = 1;
    PrintConditionInfo();

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
        if(gRules.at(i).getPrecondition().getConditions().size() > 0)
        {
            oss << gRules.at(i).getPrecondition().getConditions().at(0);
            for(unsigned j = 1; j < gRules.at(i).getPrecondition().getConditions().size(); j++)
            {  
                oss << "and" << gRules.at(i).getPrecondition().getConditions().at(j);
            }
        }
        string preCondition = oss.str();
        if(preCondition == "")
        {
            preCondition = "(True)";
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