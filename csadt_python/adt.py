import sys
import math
from copy import deepcopy
import TreeIllustrator

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
        #create variables that are the features of the example
        for feature in featuresInDataSet:
            vars()[feature] = dataSet[example][feature]
        #evaluate condition on example
        if eval(condition):
            examplesThatSatisfyCondition.append(example)
    return examplesThatSatisfyCondition

def getScoreOfExample(rule, dataSetExample):
    #create variables that are the features of the example
    for feature in featuresInDataSet:
        vars()[feature] = dataSetExample[feature]
    #eval preCondition and condition to know what score to choose
    score = 0
    for sNodeAndItsAssociatedPNodes in rule:
        if eval(sNodeAndItsAssociatedPNodes.preCondition):
            if eval(sNodeAndItsAssociatedPNodes.condition):
                score += sNodeAndItsAssociatedPNodes.alpha1
            else:
                score += sNodeAndItsAssociatedPNodes.alpha2
        else:
            score += 0
    return score

def weightPlus(condition):
    #first: get list of examples that satisfy the condition
    examplesThatSatisfyCondition = getExamplesThatSatisfyCondition(condition, trainingDataSet)
    #second: out of those examples get just the positively labeled ones
    positivelyLabeledExamples = []
    for example in examplesThatSatisfyCondition:
        if trainingDataSet[example]['class'] == SAME:
            positivelyLabeledExamples.append(example)
    #add up the weights of all positively labeled examples
    weightPlus = 0
    for example in positivelyLabeledExamples:
        weightPlus += weights[example]
    return weightPlus

def weightMinus(condition):
    #first: get list of examples that satisfy the condition
    examplesThatSatisfyCondition = getExamplesThatSatisfyCondition(condition, trainingDataSet)
    #second: out of those examples get just the negatively labeled ones
    negativelyLabeledExamples = []
    for example in examplesThatSatisfyCondition:
        if trainingDataSet[example]['class'] == DIFFERENT:
            negativelyLabeledExamples.append(example)
    #add up the weights of all negatively labeled examples
    weightMinus = 0
    for example in negativelyLabeledExamples:
        weightMinus += weights[example]
    return weightMinus

def weight(condition):
    return weightPlus(condition) + weightMinus(condition)

def calculateZ(d1, d2):
    #d1 is ex. True or a condition which can be evaluated to True
    firstPart = math.sqrt( weightPlus( andConditions(d1, d2) ) * weightMinus( andConditions(d1, d2) ) )
    secondPart = math.sqrt( weightPlus( andConditions( d1, notCondition(d2) ) ) * weightMinus( andConditions( d1, notCondition(d2) ) ) )
    thirdPart = weight( notCondition(d1) )
    z = ( 2 * ( firstPart + secondPart ) ) + thirdPart
    return z

def argMin(preConditionsUsed):
	# shitty initialization
	lowestZValue = 5 #z will never be >= 5, but probably a lot smaller than that 
	bestD1 = None
	bestD2 = None
	
	set_min = True
	
	for d1 in preConditionsUsed:
		for d2 in allConditions:
			if set_min:
				# much better guarantee of an initialization
				lowestZValue = calculateZ(d1, d2)
				bestD1 = d1
				bestD2 = d2
				set_min = False
			else:
				z = calculateZ(d1, d2)
				if z < lowestZValue:
					lowestZValue = z
					bestD1 = d1
					bestD2 = d2
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
        classification = trainingDataSet[exampleIdentifier]['class']
        if classification == SAME:
            yi = 1
        else:
            yi = -1
        cost = costPlus * calculateI(yi == 1) + costMinus * calculateI(yi == -1)
        weights[exampleIdentifier] = cost*weights[exampleIdentifier]*math.pow(math.e,-yi*score)
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

def adt(costPlus, costMinus, k, smoothFactor, graph_identifier):
	if (costMinus * weightMinus('True') + smoothFactor) == 0:
		problem = "##### ERROR: denominator is 0 and anything / 0 is not possible..."
		print problem
		sys.exit(problem)
	if (costPlus * weightPlus('True') + smoothFactor) == 0:
		problem = "##### ERROR: numerator is zero and log(0) is not possible..."
		print problem
		sys.exit(problem)
	print "costMinus: %s" % costMinus
	print "weightMinus(True): %s" % weightMinus('True')
	print "smoothFactor: %s" % smoothFactor
	initialAlpha1 = 0.5 * math.log( (costPlus * weightPlus('True') + smoothFactor) / (costMinus * weightMinus('True') + smoothFactor) )
	prediction_node_title = "node0 = " + str(initialAlpha1)
	
	initialAlpha2 = 0
	initialSplitterAndAssociatedPNodes = sNodeAndItsAssociatedPNodes('True', 'True', initialAlpha1, initialAlpha2)
	latestRule = [initialSplitterAndAssociatedPNodes]
	preConditionsUsed = {'True'}
	dictOfNodeAndItsConditions = {0: ['True']}
	
	tree_nodes = []
	tree_nodes.append(str(initialAlpha1))
	
	for i in range(k):
		smoothFactor = .5*(weight('True')/len(trainingDataSet))
		d1, d2 = argMin(preConditionsUsed)
		#Set alpha1 and alpha2 which are the scores of positive and negative classification on all training examples that meet the d1, d2 conditions
		alpha1 = 0.5 * math.log( (costPlus * weightPlus( andConditions(d1, d2) ) + smoothFactor) / (costMinus * weightMinus( andConditions(d1, d2) ) + smoothFactor)  )
		alpha2 =  0.5 * math.log( (costPlus * weightPlus( andConditions(d1, notCondition(d2) )  ) + smoothFactor) / (costMinus * weightMinus( andConditions(d1, notCondition(d2) ) ) + smoothFactor)  )
		#Set latestRule = latestRule + new sNodeAndItsAssociatedPNodes with d1 is precondition, d2 is condition, scores alpha1 for yes and alpha2 for no
		latestRule.append(sNodeAndItsAssociatedPNodes(d1, d2, alpha1, alpha2))
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
		weights = updateWeights(latestRule, costPlus, costMinus)
	
	TreeIllustrator.DrawTree(tree_nodes, graph_identifier)
	
	return latestRule

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

def classifier(parmTrainingDataSet, parmAllConditions, graph_identifier):
    global trainingDataSet, allConditions, weights, featuresInDataSet
    trainingDataSet = parmTrainingDataSet
    allConditions = deepcopy(parmAllConditions)
    weights = {}
    for example in trainingDataSet.keys():
        weights[example] = 1.0/len(trainingDataSet)
    featuresInDataSet = getFeatures(trainingDataSet)
    costPlus = 2
    costMinus = 1
    k = 5 #numberOfIterativeRounds
    smoothFactor = 0.5 * (weight('True') / len(trainingDataSet))
    #Run adt algorithm and return the tree (in the form of the last rule generated)
    rule = adt(costPlus, costMinus, k, smoothFactor, graph_identifier)
    return rule
