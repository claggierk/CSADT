import sys
import os
import string
import re
import math
import adt
import pickle
import levenshtein
import csv

global testingDataSet

SAME = "SAME"
DIFFERENT = "DIFFERENT"

def Usage():
	print "**************************************************"
	print "Usage  : python %s [text (.txt) input file] [text (.txt) input file]" % sys.argv[0]
	print "Example: python %s testing_data.txt Tree.txt" % sys.argv[0]
	print "**************************************************"

def ReCreateADTree(adtree_file):
    file_handler = open(adtree_file, 'r')
    lines = file_handler.readlines()
    adtree = []
    for line in lines:
        node_values = line.split(" ")
        print "line: %s" % line
        print "node_values: %s" % node_values
        adtree.append(adt.sNodeAndItsAssociatedPNodes(node_values[0], node_values[1], float(node_values[2]), float(node_values[3])))
    return adtree

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

def main():
	if len(sys.argv) != 3:
		Usage()
		return
	
	testing_data_file = sys.argv[1]
	adtree_file = sys.argv[2]
	
	if os.path.exists(testing_data_file) and os.path.exists(adtree_file):
		adtree = ReCreateADTree(adtree_file)
		
		(records, keys) = GetTestingData(testing_data_file)
		testingDataSet = {}
		
		#    n!
		#------------- = number of unique combinations
		#(n - r)! * r!
		
		num_records = len(records)
		print " ***** Number of records     : %s" % num_records
		
		print " Computing number of combinations..."
		num_combinations = math.factorial(num_records) / ( math.factorial(num_records - 2) * math.factorial(2) )
		print " ***** Number of combinations: %s" % num_combinations

		testingTrainingData = True
		if "eDuplicateID" in records[0]:
			print "THIS IS TRAINING DATA ... WILL COMPUTE ACCURACY"
		else:
			print "THIS IS REAL TESTING DATA ... CANNOT COMPUTE ACCURACY"
			testingTrainingData = False
		
		# test unique combinations for record linkage (duplicates)
		combination_pairs = []
		total_processed = 0
		
		number_of_matches = 0
		number_of_nonmatches = 0

		for combination_1 in range(num_records):
			total_processed = total_processed + (num_records - combination_1) - 1
			percent_complete = float(total_processed) / float(num_combinations) * 100
			print " ***** %.2f%% complete ----- total processed: %s" % (percent_complete, total_processed)
			for combination_2 in range(combination_1+1, num_records):
				tempDictionary = {}

				#if levenshtein.Compute_Levenshtein_Distance(records[combination_1]["eID"], records[combination_2]["eID"]) == 0:
				if testingTrainingData:
					if records[combination_1]["eDuplicateID"] == records[combination_2]["eDuplicateID"]:
						tempDictionary['class'] = SAME
						number_of_matches = number_of_matches + 1
						#print " ***** MATCH"
					else:
						tempDictionary['class'] = DIFFERENT
						number_of_nonmatches = number_of_nonmatches + 1
					
				for key in keys[1:]:					
					levenshtein_distance = levenshtein.Compute_Levenshtein_Distance(records[combination_1][key], records[combination_2][key])
					tempDictionary[key] = levenshtein_distance
				
				combination_pair = str(combination_1) + '-' + str(combination_2)
				combination_pairs.append(combination_pair)
				testingDataSet[combination_pair] = tempDictionary
		
		outputDatabase = adt.evaluate(testingDataSet, adtree)
		print "LENGTH OF OUTPUTDATABASE: %s" % len(outputDatabase)
		
		correct_classifications = 0
		incorrect_classifications = 0
		
		if testingTrainingData:
			number_of_predicted_matches = 0
			number_of_predicted_nonmatches = 0

			true_positives = 0
			true_negatives = 0
			false_positives = 0
			false_negatives = 0
			for combination_pair in outputDatabase.keys():
				pair = combination_pair.split("-")
				if records[int(pair[0])]["eDuplicateID"] == records[int(pair[1])]["eDuplicateID"] and outputDatabase[combination_pair]["classification"] == SAME:
					true_positives = true_positives + 1
					correct_classifications = correct_classifications + 1
					number_of_predicted_matches = number_of_predicted_matches + 1
				elif records[int(pair[0])]["eDuplicateID"] != records[int(pair[1])]["eDuplicateID"] and outputDatabase[combination_pair]["classification"] == DIFFERENT:
					true_negatives = true_negatives + 1
					correct_classifications = correct_classifications + 1
					number_of_predicted_nonmatches = number_of_predicted_nonmatches + 1
				else:
					if outputDatabase[combination_pair]["classification"] == SAME:
						#print "False Positive:", outputDatabase[combination_pair]
						false_positives = false_positives + 1
						incorrect_classifications = incorrect_classifications + 1
						number_of_predicted_matches = number_of_predicted_matches + 1
					elif outputDatabase[combination_pair]["classification"] == DIFFERENT:
						#print "False Negative:", outputDatabase[combination_pair]
						false_negatives = false_negatives + 1
						incorrect_classifications = incorrect_classifications + 1
						number_of_predicted_nonmatches = number_of_predicted_nonmatches + 1
					else:
						print " **************************** THIS SHOULD NEVER HAPPEN"
			
			print ""
			print "Number of records             : %s" % num_records
			print "Number of combinations        : %s" % num_combinations
			print ""
			print "Number of matches             : %s" % number_of_matches
			print "Number of nonmatches          : %s" % number_of_nonmatches
			print ""
			print "Number of predicted matches   : %s" % number_of_predicted_matches
			print "Number of predicted nonmatches: %s" % number_of_predicted_nonmatches
			print ""
			print "True Positives                : %s" % true_positives
			print "True Negatives                : %s" % true_negatives
			print ""
			print "False Positives               : %s" % false_positives
			print "False Negatives               : %s" % false_negatives
			print ""
			total_classifications = correct_classifications + incorrect_classifications
			print "Correct classifications       : %s" % correct_classifications
			print "Incorrect classifications     : %s" % incorrect_classifications
			classification_accuracy_ratio = float(correct_classifications) / float(total_classifications)
			classification_accuracy_percentage = classification_accuracy_ratio * 100.0
			print "Classification Accuracy       : %.2f%%" % classification_accuracy_percentage
		
	else:
		Usage()
		return
	
if __name__ == "__main__":
	main()
