import copy
import csv

def BuildDataStructure(input_file):
	file_handler = open(input_file, 'r')

	lines = file_handler.readlines()

	first_line = lines[0]
	first_line = first_line.strip()
	keys = first_line.split("|")

	records = []
	record = {}
	for key_index, key in enumerate(keys):
		record[key] = ""

	for line_index, line in enumerate(lines[1:]):
		line = line.strip()
		cell_values = line.split("|")
		for cell_value_index, cell_value in enumerate(cell_values):
			record[ keys[cell_value_index] ] = cell_value
		records.append(copy.deepcopy(record))

	file_handler.close()

	return (records, keys)

def BuildComparisonPairsDataStructure(input_file):
	file_handler = open(input_file, 'r')
	lines = file_handler.readlines()
	number_of_matches = lines[0].split()[0]
	number_of_non_matches = lines[0].split()[1]

	keys = lines[1].strip().split("|")

	trainingComparisonPairs = {}
	for line_index, line in enumerate(lines[2:]):
		line = line.strip()
		cell_values = line.split("|")

		trainingComparisonPair = {}
		for cell_value_index, cell_value in enumerate(cell_values[1:]):
			currentKey = keys[cell_value_index+1] # ridiculous! ... although I told it to go from 1:END ... it starts iterating at 0!
			trainingComparisonPair[currentKey] = cell_value
			#print "currentKey: %s === %s" % (currentKey, cell_value)
		comparisonRecordID = cell_values[0]
		trainingComparisonPairs[comparisonRecordID] = trainingComparisonPair

	file_handler.close()

	return number_of_matches, number_of_non_matches, trainingComparisonPairs

def OutputRecordsToFile(records, tabulated_file, record_pairs, features, number_of_matches, number_of_nonmatches):
	file_handler = open(tabulated_file, 'w')
	file_handler.write("%s %s\n" % (number_of_matches, number_of_nonmatches))
	file_handler.write("CombinationID|classification|")
	file_handler.write("%s\n" % "|".join(features))

	for record in record_pairs:
		levenshtein_distances = []
		for feature in features:
			levenshtein_distances.append(str(records[record][feature]))
		file_handler.write("%s|%s|" % (record, records[record]["class"]))
		file_handler.write("%s\n" % "|".join(levenshtein_distances))
	file_handler.close()

def ReCreateADTree(adtree_file, adt):
    file_handler = open(adtree_file, 'r')
    lines = file_handler.readlines()
    adtree = []
    for line in lines:
        node_values = line.split(" ")
        print "line: %s" % line
        print "node_values: %s" % node_values
        adtree.append(adt.sNodeAndItsAssociatedPNodes(node_values[0], node_values[1], float(node_values[2]), float(node_values[3])))
    return adtree

def WriteTreeToFile(adtClassifer, adtree_file):
	file_handler = open(adtree_file, 'w')

	file_handler.write("%s %s %s %s" % (adtClassifer[0].preCondition, adtClassifer[0].condition, adtClassifer[0].alpha1, adtClassifer[0].alpha2))
	print "TREE"
	print "   %s %s %s %s" % (adtClassifer[0].preCondition, adtClassifer[0].condition, adtClassifer[0].alpha1, adtClassifer[0].alpha2)
	for node in adtClassifer[1:]:
		preCondition = node.preCondition
		preCondition = ''.join(preCondition.split())

		condition = node.condition
		condition = ''.join(condition.split())

		alpha1 = node.alpha1
		alpha2 = node.alpha2

		file_handler.write("\n%s %s %s %s" % (preCondition, condition, alpha1, alpha2))
		print "   %s %s %s %s" % (preCondition, condition, alpha1, alpha2)
	file_handler.close()

def GetTestingData(input_file):
	testing_data = []

	input_file_handler = open(input_file, 'r')
	with open(input_file, 'rb') as csvfile:
		dataCSV = csv.reader(csvfile, delimiter=',', quotechar='"')
		keys = dataCSV.next()
		for row in dataCSV:
			row[-1] = row[-1].strip()
			aRecord = {}
			for key_index, key in enumerate(keys):
				aRecord[key] = row[key_index]
			testing_data.append(aRecord)

	print "There are %s testing records" % len(testing_data)
	return (testing_data, keys)
