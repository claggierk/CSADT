import copy

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
	
	first_line = lines[0]
	first_line = first_line.strip()
	keys = first_line.split("|")
	
	trainingComparisonPairs = {}
	for line_index, line in enumerate(lines[1:]):		
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
	
	return trainingComparisonPairs


def OutputRecordsToFile(records, tabulated_file, record_pairs, features):
	file_handler = open(tabulated_file, 'w')
	file_handler.write("CombinationID|classification|")
	file_handler.write("%s\n" % "|".join(features))
	
	for record in record_pairs:
		levenshtein_distances = []
		for feature in features:
			levenshtein_distances.append(str(records[record][feature]))
		file_handler.write("%s|%s|" % (record, records[record]["class"]))
		file_handler.write("%s\n" % "|".join(levenshtein_distances))
	file_handler.close()

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