import sys
import math
from copy import deepcopy

from types import NoneType

global trainingDataSet, allConditions, testDataSet, weights, featuresInDataSet

SAME = "SAME"
DIFFERENT = "DIFFERENT"

class sNodeAndItsAssociatedPNodes():
    def __init__(self, preCondition, condition, alpha1, alpha2):
        self.preCondition = preCondition
        self.condition = condition
        self.alpha1 = alpha1
        self.alpha2 = alpha2
    
    def __str__(self):
        stringMe = "Precondition: " + str(self.preCondition) + "\n"
        stringMe = stringMe + "Condition   : " + str(self.condition) + "\n"
        stringMe = stringMe +  "trueScore   : " + str(self.alpha1) + "\n"
        stringMe = stringMe +  "falseScore  : " + str(self.alpha2) + "\n"
        return stringMe
    
    def __repr__(self):
        stringMe = "Precondition: " + str(self.preCondition) + "\n"
        stringMe = stringMe + "Condition   : " + str(self.condition) + "\n"
        stringMe = stringMe +  "trueScore   : " + str(self.alpha1) + "\n"
        stringMe = stringMe +  "falseScore  : " + str(self.alpha2) + "\n"
        return stringMe

def andConditions(condition1, condition2):
    return '(' + condition1 + ')' + ' and ' + '(' + condition2 + ')'

def notCondition(condition):
    return 'not ' + '(' + condition + ')'

def getFeatures(dataSet):
    features = []
    for example in dataSet.keys():
        for feature in dataSet[example].keys():
            features.append(feature)
        return features

def getExamplesThatSatisfyCondition(condition, dataSet):
    examplesThatSatisfyCondition = []
    for example in dataSet.keys():
        for feature in featuresInDataSet:
            if dataSet[example][feature] == SAME or dataSet[example][feature] == DIFFERENT:
                vars()[feature] = dataSet[example][feature]
            else:
                vars()[feature] = int(dataSet[example][feature])
        if eval(condition):
            examplesThatSatisfyCondition.append(example)
    return examplesThatSatisfyCondition

def getScoreOfExample(rule, dataSetExample):
    #print "dataSetExample:", dataSetExample
    #create variables that are the features of the example
    for feature in featuresInDataSet:
        if dataSetExample[feature] == SAME or dataSetExample[feature] == DIFFERENT:
            vars()[feature] = dataSetExample[feature]
        else:
            vars()[feature] = int(dataSetExample[feature])
        # FIVE ####vars()[feature] = dataSetExample[feature]
    #eval preCondition and condition to know what score to choose
    score = 0
    for sNodeAndItsAssociatedPNodes in rule:
        #print "Current Rule:", sNodeAndItsAssociatedPNodes
        #print "before score:", score
        if eval(sNodeAndItsAssociatedPNodes.preCondition):
            if eval(sNodeAndItsAssociatedPNodes.condition):
                #print("going to alpha1")
                score += sNodeAndItsAssociatedPNodes.alpha1
            else:
                #print("going to alpha2")
                score += sNodeAndItsAssociatedPNodes.alpha2
        else:
            score += 0
        #print "after score:", score
    return score

def weightPlus(condition):
    #first: get list of examples that satisfy the condition
    examplesThatSatisfyCondition = getExamplesThatSatisfyCondition(condition, trainingDataSet)
    #second: out of those examples get just the positively labeled ones
    positivelyLabeledExamples = []
    for example in examplesThatSatisfyCondition:
        if trainingDataSet[example]['classification'] == SAME:
            positivelyLabeledExamples.append(example)
    #???print " ++++++++++ positivelyLabeledExamples: ", positivelyLabeledExamples
    #add up the weights of all positively labeled examples
    weightPlusSum = 0
    for example in positivelyLabeledExamples:
        weightPlusSum += weights[example]
    # FIVE
    #print " ***** Condition: %s weightPlusSum: %s (from %s examples)" % (condition, weightPlusSum, len(positivelyLabeledExamples))
    return weightPlusSum

def weightMinus(condition):
    #first: get list of examples that satisfy the condition
    examplesThatSatisfyCondition = getExamplesThatSatisfyCondition(condition, trainingDataSet)
    #second: out of those examples get just the negatively labeled ones
    negativelyLabeledExamples = []
    for example in examplesThatSatisfyCondition:
        if trainingDataSet[example]['classification'] == DIFFERENT:
            negativelyLabeledExamples.append(example)
    #add up the weights of all negatively labeled examples
    weightMinusSum = 0
    for example in negativelyLabeledExamples:
        weightMinusSum += weights[example]
    # FIVE
    #print " ***** weightMinusSum: %s (from %s examples)" % (str(weightMinusSum), len(negativelyLabeledExamples))
    return weightMinusSum

def weight(condition):
    return weightPlus(condition) + weightMinus(condition)

def calculateZ(d1, d2):
    #d1 is ex. True or a condition which can be evaluated to True
    firstPart = math.sqrt( weightPlus( andConditions(d1, d2) ) * weightMinus( andConditions(d1, d2) ) )
    secondPart = math.sqrt( weightPlus( andConditions( d1, notCondition(d2) ) ) * weightMinus( andConditions( d1, notCondition(d2) ) ) )
    thirdPart = weight( notCondition(d1) )
    z = ( 2 * ( firstPart + secondPart ) ) + thirdPart
    return z

def outputCalculateZ(d1, d2):
    #d1 is ex. True or a condition which can be evaluated to True
    print "First part a: ", weightPlus( andConditions(d1, d2) )
    print "First part b: ", weightMinus( andConditions(d1, d2) )
    firstPart = math.sqrt( weightPlus( andConditions(d1, d2) ) * weightMinus( andConditions(d1, d2) ) )

    print "Second part a: ", weightPlus( andConditions( d1, notCondition(d2) ) )
    print "Second part b: " , weightMinus( andConditions( d1, notCondition(d2) ) )
    secondPart = math.sqrt( weightPlus( andConditions( d1, notCondition(d2) ) ) * weightMinus( andConditions( d1, notCondition(d2) ) ) );

    print "Third part: ", weight( notCondition(d1) )
    print "d1: ", d1
    thirdPart = weight( notCondition(d1) )
    z = ( 2 * ( firstPart + secondPart ) ) + thirdPart
    return z

def argMin(preConditionsUsed):
    z_values = []
    
    for d1 in preConditionsUsed:
        for d2 in allConditions:
            z_values.append((d1, d2, calculateZ(d1, d2)))
    z_values.sort(key=lambda tup: tup[2])
    lowestZValue = z_values[0]
    
    bestD1 = lowestZValue[0]
    bestD2 = lowestZValue[1]
    lowestZValue = lowestZValue[2]

    outputCalculateZ(bestD1, bestD2)
            
    print " ^^^^^^^^^^ lowestZValue: %s" % lowestZValue
    if len(preConditionsUsed) == 0:
        print "##### ERROR: length of preConditionsUsed is 0!"
    if len(allConditions) == 0:
        print "##### ERROR: length of allConditions is 0!"
        print allConditions
    if type(bestD1) is NoneType:
        print "##### ERROR: bestD1 is NoneType!"
    if type(bestD2) is NoneType:
        print "##### ERROR: bestD2 is NoneType!"
    
    return bestD1, bestD2

def calculateI(condition):
    if condition:
        return 1
    else:
        return 0

def updateWeights(rule, costPlus, costMinus):
    for exampleIdentifier in trainingDataSet.keys():
        score = getScoreOfExample(rule, trainingDataSet[exampleIdentifier])
        classification = trainingDataSet[exampleIdentifier]['classification']
        if classification == SAME:
            yi = 1
        else:
            yi = -1
        cost = costPlus * calculateI(yi == 1) + costMinus * calculateI(yi == -1)
        weights[exampleIdentifier] = cost * weights[exampleIdentifier] * math.pow(math.e, -yi * score)
    return weights

def getNodeNumAndYesOrNoCondition(dictOfNodeAndItsConditions, d1):
    for key in dictOfNodeAndItsConditions.keys():
        count = 0
        for condition in dictOfNodeAndItsConditions[key]:
            if condition == d1:
                if count == 0:
                    return key, "yes"
                else:
                    return key, "no"
            count += 1

def adt(costPlus, costMinus, numTreeNodes):
    print "Number of training record pairs: ", len(trainingDataSet)
    smoothFactor = 0.5 * (weight('True') / len(trainingDataSet))
    if (costMinus * weightMinus('True') + smoothFactor) == 0:
        problem = "##### ERROR: denominator is 0 and anything / 0 is not possible..."
        print problem
        sys.exit(problem)
    if (costPlus * weightPlus('True') + smoothFactor) == 0:
        problem = "##### ERROR: numerator is zero and log(0) is not possible..."
        print problem
        sys.exit(problem)
    
    print "costPlus         : %s" % costPlus
    print "costMinus        : %s" % costMinus
    print "numTreeNodes     : %s" % numTreeNodes
    print "smoothFactor     : %s" % smoothFactor
    print "weightPlus(True) : %s" % weightPlus('True')
    print "weightMinus(True): %s" % weightMinus('True')
    print "weight('True')   : %s" % weight('True')
    print "trainingDataSet  : %s" % len(trainingDataSet)

    initialAlpha1 = 0.5 * math.log( (costPlus * weightPlus('True') + smoothFactor) / (costMinus * weightMinus('True') + smoothFactor) )
    prediction_node_title = "node0 = " + str(initialAlpha1)
    
    initialAlpha2 = 0
    initialSplitterAndAssociatedPNodes = sNodeAndItsAssociatedPNodes('True', 'True', initialAlpha1, initialAlpha2)

    print "initialAlpha1    : %s" % initialAlpha1
    print "initialAlpha2    : %s" % initialAlpha2

    rules = [initialSplitterAndAssociatedPNodes]
    preConditionsUsed = {'True'}
    dictOfNodeAndItsConditions = {0: ['True']}
    
    tree_nodes = []
    tree_nodes.append(str(initialAlpha1))
    
    for i in range(numTreeNodes):
        #???print " ***** There are %s available conditions: %s" % (len(allConditions), allConditions)
        #???print " ***** %s pre-conditions used: %s" % (len(preConditionsUsed), preConditionsUsed)
        print ""
        print " ***** ITERATION: %s ************************* " % i
        #print "Weight(True): ", weight('True')
        smoothFactor = 0.5 * (weight('True') / len(trainingDataSet))
        smoothFactor = 0.5 * (1.0 / len(trainingDataSet)) # TODO FIX ME ?????
        d1, d2 = argMin(preConditionsUsed)
        #Set alpha1 and alpha2 which are the scores of positive and negative classification on all training examples that meet the d1, d2 conditions
        print "smoothFactor: ", smoothFactor
        alpha1 = 0.5 * math.log( (costPlus * weightPlus( andConditions(d1, d2) ) + smoothFactor) / (costMinus * weightMinus( andConditions(d1, d2) ) + smoothFactor)  )
        #print "alpha1 numerator  : ", (costPlus * weightPlus( andConditions(d1, d2) ) + smoothFactor)
        #print "alpha1 denominator: ", (costMinus * weightMinus( andConditions(d1, d2) ) + smoothFactor)
        print "alpha1    : %s" % alpha1
        alpha2 =  0.5 * math.log( (costPlus * weightPlus( andConditions(d1, notCondition(d2) )  ) + smoothFactor) / (costMinus * weightMinus( andConditions(d1, notCondition(d2) ) ) + smoothFactor)  )
        #print "alpha2 numerator  : ", (costPlus * weightPlus( andConditions(d1, notCondition(d2) )  ) + smoothFactor)
        #print "alpha2 denominator: ", (costMinus * weightMinus( andConditions(d1, notCondition(d2) ) ) + smoothFactor)
        print "alpha2    : %s" % alpha2
        #Set rules = rules + new sNodeAndItsAssociatedPNodes with d1 is precondition, d2 is condition, scores alpha1 for yes and alpha2 for no
        rules.append(sNodeAndItsAssociatedPNodes(d1, d2, alpha1, alpha2))
        nodeNum, yesOrNo = getNodeNumAndYesOrNoCondition(dictOfNodeAndItsConditions, d1)
        
        '''
        node 1 (cond: FullName < 3 (y: 1.50 n: -4.5)) is attached to the yes of node 0
        node 2 (cond: City < 3 (y: 4.89 n: 0.25)) is attached to the yes of node 1
        node 3 (cond: Age == 0 (y: 1.55 n: -4.0)) is attached to the no of node 2
        node 4 (cond: FirstName == 0 (y: 4.51 n: -4.3)) is attached to the yes of node 3
        '''
        
        node_info = "node " + str(i+1) + " (cond:" + str(d2) + '(y: ' + str(alpha1)[:4] + ' n: ' + str(alpha2)[:4] + ")) is attached to the " + yesOrNo + " of node " + str(nodeNum)
        print "node_info:", node_info
        
        #print "condition selected:", str(d2)
        # remove the most recently selected condition ... which is str(d2) ... from allConditions
        allConditions.remove(str(d2))
        
        '''
        node title # node condition # yes weight # no weight # parent node
                                node title           # node condition  # yes weight            # no weight             # parent node
        '''
        if str(nodeNum) == "0":
            tree_nodes_string = "node" + str(i+1) + "#(" + str(d2) + ")#" + str(alpha1)[:4] + "#" + str(alpha2)[:4] + "#" + prediction_node_title
        else:
            tree_nodes_string = "node" + str(i+1) + "#(" + str(d2) + ")#" + str(alpha1)[:4] + "#" + str(alpha2)[:4] + "#" + "node" + str(nodeNum) + yesOrNo
        tree_nodes.append(tree_nodes_string)
        
        #Add current condition ( d2 ) to preConditionsUsed
        preConditionsUsed.add( andConditions(d1, d2) )
        preConditionsUsed.add( andConditions(d1, notCondition(d2)) )
        dictOfNodeAndItsConditions[i+1] = [andConditions(d1, d2), andConditions(d1, notCondition(d2))]
        #update all weights
        weights = updateWeights(rules, costPlus, costMinus)
    
    return rules

def evaluate(parmTestDataSet, rule):
    global testDataSet, featuresInDataSet
    testDataSet = parmTestDataSet
    featuresInDataSet = getFeatures(testDataSet)
    #Run through rule and see if the conditions are met. Along the way add up the scores.
    outputDatabase = {} #ex. outputDatabase['1-2']['classification'] == SAME and outputDatabase['1-2']['score'] == -0.37
    for exampleIdentifier in testDataSet.keys():
        score = getScoreOfExample(rule, testDataSet[exampleIdentifier])
        #If total Score is negative, return negative classification or visa versa 
        if score >= 0:
            outputDatabase[exampleIdentifier] = {'classification':SAME, 'score':score}
        else:
            outputDatabase[exampleIdentifier] = {'classification':DIFFERENT, 'score':score}
    return outputDatabase

def classifier(parmTrainingDataSet, numTreeNodes, parmAllConditions):
    global trainingDataSet, allConditions, weights, featuresInDataSet
    trainingDataSet = parmTrainingDataSet
    allConditions = deepcopy(parmAllConditions)
    weights = {}
    for example in trainingDataSet.keys():
        weights[example] = 1.0/len(trainingDataSet)
    featuresInDataSet = getFeatures(trainingDataSet)
    costPlus = 2.0
    costMinus = 1.0
    #Run adt algorithm and return the tree (in the form of the last rule generated)
    rules = adt(costPlus, costMinus, int(numTreeNodes))
    
    return rules
