import adt

def makelistOfFolds(initialTrainingDataSet, k):
    initialTrainingDataSetCopy = initialTrainingDataSet.copy()
    listOfFolds = []
    foldLength = int(len(initialTrainingDataSetCopy.keys())/k)
    for i in range(k):
        listOfFolds.append({})
        for j in range(foldLength):
            recordPair = initialTrainingDataSetCopy.popitem()
            listOfFolds[i][recordPair[0]] = recordPair[1]
    return listOfFolds

def getIthTrainingAndTestingData(listOfFolds, i):
    trainingData = listOfFolds[i]
    testingData = {}
    for fold in listOfFolds:
        if fold != listOfFolds[i]:
            testingData = dict(testingData.items() + fold.items())
    return trainingData, testingData

def getAccuracy(testingDataOutput, initialTrainingDataSet):
    numberWrong = 0
    for recordPairID in testingDataOutput.keys():
        if testingDataOutput[recordPairID]['classification'] != initialTrainingDataSet[recordPairID]["class"]:
            numberWrong += 1
    acc = float(len(testingDataOutput)-numberWrong)/float(len(testingDataOutput))
    return acc

def computeFinalAccuracy(kAccuracies):
    sum = 0.0
    for accuracy in kAccuracies:
        sum += accuracy
    return sum/float(len(kAccuracies))

def crossValidation(initialTrainingDataSet, allConditions, k):
    listOfFolds = makelistOfFolds(initialTrainingDataSet, k)
    kAccuracies = []
    for i in range(k):
        trainingData, testingData = getIthTrainingAndTestingData(listOfFolds, i)
        adtClassifer = adt.classifer(trainingData, allConditions)
        testingDataOutput = adt.evaluate(testingData, adtClassifer)
        kAccuracies.append(getAccuracy(testingDataOutput, initialTrainingDataSet))
    return computeFinalAccuracy(kAccuracies)