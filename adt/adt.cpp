#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <sstream>

#include "Instance.h"
#include "Condition.h"
#include "FeatureStats.h"
#include "Rule.h"

using namespace std;

const unsigned MAX_FEATURE_COUNT = 128;
const unsigned NUM_COMMAND_LINE_ARGUMENTS = 2;

vector<Instance> gMatches;
vector<Instance> gNonMatches;
vector< vector<Condition> > gConditions;
vector< vector<Condition> > gPreConditionsUsed;
vector<Condition> gPreConditionChosen;
Condition gConditionChosen;
vector<Condition> gPAndCChosen;
vector<Condition> gPandNotCChoosen;
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
    cerr << "Size of matchFeatureStats: " << matchFeatureStats.size() << endl;
    cerr << matchFeatureStats.at(0);

    vector<FeatureStats> nonMatchFeatureStats;
    PopulateFeatureStatus(gNonMatches, nonMatchFeatureStats);
    cerr << "Size of nonMatchFeatureStats: " << nonMatchFeatureStats.size() << endl;
    cerr << nonMatchFeatureStats.at(0);

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

vector<Instance> getNegInstancesThatSatisfyCondition(vector<Condition> conditionVector, string logicalOperator) {
    vector<Instance> instancesThatSatisfy;
    bool satisfy = false;
    for (unsigned i = 0; i < gNonMatches.size(); i++) {
        for (unsigned j = 0; j < conditionVector.size(); j++) {
            if (logicalOperator == "and") {  //used when an instance needs to satisfy more than one condition (ex. Both condition1 and conditon2 have to be true)
                if (conditionVector.at(j).evaluate(gNonMatches.at(i))) { 
                    satisfy = true;          
                } else {
                    satisfy = false;
                    break;
                }
            }
            else if (logicalOperator == "or") { //used only for "not p" where each condition of p is negated and joined by an or (De Morgan's Law:  if not p = not(condtion1 and condition2), then not(condition1 and condition2) <-> not(condition1) or not(condition2))
                if (conditionVector.at(j).evaluate(gNonMatches.at(i))) { 
                    satisfy = true; 
                    break;         
                } else {
                    satisfy = false;
                }
            }
        }
        if (satisfy) {
            instancesThatSatisfy.push_back(gNonMatches.at(i)); 
        }
        satisfy = false;
    }
    return instancesThatSatisfy;
}

vector<Instance> getPosInstancesThatSatisfyCondition(vector<Condition> conditionVector, string logicalOperator) {
    vector<Instance> instancesThatSatisfy;
    bool satisfy = false;
    for (unsigned i = 0; i < gMatches.size(); i++) {
        for (unsigned j = 0; j < conditionVector.size(); j++) {
            if (logicalOperator == "and") {  //used when an instance needs to satisfy more than one condition (ex. Both condition1 and conditon2 have to be true)
                if (conditionVector.at(j).evaluate(gMatches.at(i))) { 
                    satisfy = true;          
                } else {
                    satisfy = false;
                    break;
                }
            }
            else if (logicalOperator == "or") { //used only for "not p" where each condition of p is negated and joined by an or (De Morgan's Law:  if not p = not(condtion1 and condition2), then not(condition1 and condition2) <-> not(condition1) or not(condition2))
                if (conditionVector.at(j).evaluate(gMatches.at(i))) { 
                    satisfy = true; 
                    break;         
                } else {
                    satisfy = false;
                }
            }
        }
        if (satisfy) {
            instancesThatSatisfy.push_back(gMatches.at(i)); 
        }
        satisfy = false;
    }
    return instancesThatSatisfy;
}

float wMinus(vector<Condition> conditionVector, string logicalOperator) {
    vector<Instance> instancesThatSatisfy = getNegInstancesThatSatisfyCondition(conditionVector, logicalOperator);
    float sumOfWeights = 0.0;
    for (unsigned i = 0; i < instancesThatSatisfy.size(); i++) {
        sumOfWeights += sumOfWeights + instancesThatSatisfy.at(i).getWeight();
    }
    return sumOfWeights;
}

float wPlus(vector<Condition> conditionVector, string logicalOperator) {
    vector<Instance> instancesThatSatisfy = getPosInstancesThatSatisfyCondition(conditionVector, logicalOperator);
    float sumOfWeights = 0.0;
    for (unsigned i = 0; i < instancesThatSatisfy.size(); i++) {
        sumOfWeights += sumOfWeights + instancesThatSatisfy.at(i).getWeight();
    }
    return sumOfWeights;
}

float w(vector<Condition> conditionVector, string logicalOperator) {
    return wPlus(conditionVector, logicalOperator) + wMinus(conditionVector, logicalOperator);
}

float calcZ(vector<Condition> p, Condition c) {  // p = d1 ; c = d2
    //prepare w, wPlus, and wMinus input
    vector<Condition> pAndC = p;
    pAndC.push_back(c);
    vector<Condition> pAndNotC = p;
    Condition notC = c;
    notC.setNotFlag(true);
    pAndNotC.push_back(notC);
    vector<Condition> notP = p;
    for (unsigned i = 0; i < notP.size(); i++) {
        notP.at(i).setNotFlag(true);
    }
    //calculate z equation
    float firstPart = sqrt( wPlus(pAndC, "and") * wMinus(pAndC, "and") );
    float secondPart = sqrt( wPlus(pAndNotC, "and") * wMinus(pAndNotC, "and") );
    float thirdPart = w(notP, "or");
    float z = (2 * (firstPart + secondPart)) + thirdPart;
    return z;
}

void computeArgMin() {
    //Thanks for saying I had a shitty initialization :)
    float lowestZValue = 1000000.0;
    vector<Condition> bestP;
    Condition bestC;
    bool setMin = true;
    //find best preCondition and condition
    for (unsigned i = 0; i < gPreConditionsUsed.size(); i++) {
        for (unsigned j = 0; j < gConditions.size(); j++) {
            for (unsigned k = 0; k < gConditions.at(j).size(); k++) {
                if (setMin) {
                    lowestZValue = calcZ(gPreConditionsUsed.at(i), gConditions.at(j).at(k));
                    bestP = gPreConditionsUsed.at(i);
                    bestC = gConditions.at(j).at(k);
                    setMin = false;                
                } else {
                    float z = calcZ(gPreConditionsUsed.at(i), gConditions.at(j).at(k));
                    if (z < lowestZValue) {
                        lowestZValue = z;
                        bestP = gPreConditionsUsed.at(i);
                        bestC = gConditions.at(j).at(k);
                    }
                }
            }
        }
    }
    gPreConditionChosen = bestP;
    gConditionChosen = bestC;
}

void createAndUpdategPAndCAndgPandNotC() {
    //Create p and c, p and not c so they can be used several times later.
    //I need to make copies. 
    vector<Condition> pAndC = gPreConditionChosen; //hopefully this makes a copy of all conditions
    Condition c = gConditionChosen;
    pAndC.push_back(c);
    vector<Condition> pAndNotC = gPreConditionChosen;
    Condition notC = gConditionChosen;
    notC.setNotFlag(true);
    pAndNotC.push_back(notC);
    gPAndCChosen = pAndC;
    gPandNotCChoosen = pAndNotC;
    //Add them to gPreconditionsUsed
    gPreConditionsUsed.push_back(pAndC);
    gPreConditionsUsed.push_back(pAndNotC);
}

float getScoreOfInstance(Instance myInstance) {
    float score = 0.0;
    for (unsigned i = 0; i < gRules.size(); i++) {
        vector<Condition> p = gRules.at(i).getPrecondition();
        if (checkIfInstanceSatisfiesCondition(myInstance, p)) {
            vector<Condition> pAndC = p;
            Condition c = gRules.at(i).getCondition();
            pAndC.push_back(c);
            if (checkIfInstanceSatisfiesCondition(myInstance, pAndC)) {
                score += gRules.at(i).getTrueScore();
            } else {
                score += gRules.at(i).getFalseScore();
            }
        } else {
            score += 0;
        }
    }
    return score;
}

void updateWeights(float cPlus, float cMinus) {
    for (unsigned i = 0; i < gMatches.size(); i++) {
        float score = getScoreOfInstance(gMatches.at(i));
        float yi = 1.0;
        float cost = cPlus * 1.0 + cMinus * 0.0;
        gMatches.at(i).setWeight(gMatches.at(i).getWeight()*exp(-yi*score));
    }
    for (unsigned i = 0; i < gNonMatches.size(); i++) {
        float score = getScoreOfInstance(gNonMatches.at(i));
        float yi = -1.0;
        float cost = cPlus * 0.0 + cMinus * 1.0;
        gNonMatches.at(i).setWeight(gNonMatches.at(i).getWeight() * cost * exp(-yi*score));
    }    
}

void GenerateADT(float cPlus, float cMinus, unsigned k)
{
    //clear global vars that will be modified and used
    gPreConditionsUsed.clear();
    gPreConditionChosen.clear();
    gConditionChosen = Condition();
    gPAndCChosen.clear();
    gPandNotCChoosen.clear();
    gRules.clear();
    //set initial weights of instances
    double initialWeight = SetInitialWeights();
    //create initial rule
    Condition c = Condition(true);
    vector<Condition> cAsAVector;
    cAsAVector.push_back(c);
    float smoothFactor = .5 * (w(cAsAVector, "and")/(gMatches.size() + gNonMatches.size()));
    float alpha1 = 0.5 * log((cPlus * wPlus(cAsAVector, "and") + smoothFactor) / (cMinus * wMinus(cAsAVector, "and") + smoothFactor));
    float alpha2 = 0.0;
    gRules.push_back(Rule(cAsAVector, c, alpha1, alpha2));
    cerr << "##### ERROR: The number of conditions the preconditon has for the initial rule (should be 1): " << gRules.at(0).getPrecondition().size() << endl;
    gPreConditionsUsed.push_back(cAsAVector);
    //create remaining rules
    for (unsigned i = 0; i < k; i++) {
        smoothFactor = .5 * (w(cAsAVector, "and")/(gMatches.size() + gNonMatches.size()));
        computeArgMin();
        createAndUpdategPAndCAndgPandNotC();
        alpha1 = 0.5 * log((cPlus * wPlus(gPAndCChosen, "and") + smoothFactor)/(cMinus * wMinus(gPAndCChosen, "and") + smoothFactor));
        alpha2 = 0.5 * log((cPlus * wPlus(gPandNotCChoosen, "and") + smoothFactor)/(cMinus * wMinus(gPandNotCChoosen, "and") + smoothFactor));
        gRules.push_back(Rule(gPreConditionChosen, gConditionChosen, alpha1, alpha2));
        updateWeights(cPlus, cMinus);
    }
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
    float cPlus = 2.0;
    float cMinus = 1.0;
    unsigned k = 5;
    GenerateADT(cPlus, cMinus, k);

    ofstream fout;
    fout.open(outputFileName.c_str());

    //print rules
    cerr << "\nCSADT Rules Output:";
    for(unsigned i = 0; i < gRules.size(); i++) {  
        cerr << "\nRule " << i << ": " << endl;
        
        ostringstream oss;
        if(gRules.at(i).getPrecondition().size() > 0) {
            oss << gRules.at(i).getPrecondition().at(0);
            for(unsigned j = 1; j < gRules.at(i).getPrecondition().size(); j++) {  
                oss << " " << gRules.at(i).getPrecondition().at(j);
            }
        }
        string preCondition = oss.str();
        if(preCondition == "") {
            preCondition = "(true)";
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

        if(! fout.is_open()) {
            cerr << endl << "##### ERROR: Unable to open " << outputFileName << ".";
            exit_now();
        }
        else {
            fout << preCondition << " " << condition << " " << trueScore << " " << falseScore;
            if((i+1) != gRules.size()) {
                fout << endl;
            }
        }
    }
    if(fout.is_open()) {
        fout.close();
    }

	cerr << endl << endl;
	return EXIT_SUCCESS;
}