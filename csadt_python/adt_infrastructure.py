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

def OutputRecordsToFile(records, tabulated_file, record_pairs, features):
	file_handler = open(tabulated_file, 'w')
	file_handler.write("CombinationID|Classification|")
	file_handler.write("%s\n" % "|".join(features[1:]))
	#import pdb; pdb.set_trace()
	for record in record_pairs:
		levenshtein_distances = []
		for feature in features[1:]:
			levenshtein_distances.append(str(records[record][feature]))
		file_handler.write("%s|%s|" % (record, records[record]["class"]))
		file_handler.write("%s\n" % "|".join(levenshtein_distances))
	file_handler.close()

def WriteTreeToFile(adtClassifer, adtree_file):
	file_handler = open(adtree_file, 'w')
	
	file_handler.write("%s %s %s %s" % (adtClassifer[0].preCondition, adtClassifer[0].condition, adtClassifer[0].alpha1, adtClassifer[0].alpha2))
	for node in adtClassifer[1:]:
		preCondition = node.preCondition
		preCondition = ''.join(preCondition.split())
		
		condition = node.condition
		condition = ''.join(condition.split())
		
		alpha1 = node.alpha1
		alpha2 = node.alpha2
		
		file_handler.write("\n%s %s %s %s" % (preCondition, condition, alpha1, alpha2))
	file_handler.close()