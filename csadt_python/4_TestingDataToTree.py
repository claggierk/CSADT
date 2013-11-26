import sys
import os
import string
import re
import math
import adt
import accuracy
#import orange
import pickle
import levenshtein
import adt_infrastructure

global trainingDataSet

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

def main():
	global trainingDataSet
	
	if len(sys.argv) != 3:
		Usage()
		return
	
	testing_data_file = sys.argv[1]
	adtree_file = sys.argv[2]
	
	if os.path.exists(testing_data_file) and os.path.exists(adtree_file):
		adtree = ReCreateADTree(adtree_file)
		
		(records, keys) = adt_infrastructure.BuildDataStructure(testing_data_file)
		trainingDataSet = {}
		
		#    n!
		#------------- = number of unique combinations
		#(n - r)! * r!
		
		num_records = len(records)
		print " ***** Number of records     : %s" % num_records
		
		print " Computing number of combinations..."
		num_combinations = math.factorial(num_records) / ( math.factorial(num_records - 2) * math.factorial(2) )
		print " ***** Number of combinations: %s" % num_combinations
		
		# test unique combinations for record linkage (duplicates)
		combination_pairs = []
		total_processed = 0
		for combination_1 in range(num_records):
			total_processed = total_processed + (num_records - combination_1) - 1
			percent_complete = float(total_processed) / float(num_combinations) * 100
			print " ***** %.2f%% complete ----- total processed: %s" % (percent_complete, total_processed)
			for combination_2 in range(combination_1+1, num_records):
				tempDictionary = {}
				if levenshtein.Compute_Levenshtein_Distance(records[combination_1]["eID"], records[combination_2]["eID"]) == 0:
					tempDictionary['class'] = SAME
				else:
					tempDictionary['class'] = DIFFERENT
				for key in keys[1:]:					
					levenshtein_distance = levenshtein.Compute_Levenshtein_Distance(records[combination_1][key], records[combination_2][key])
					tempDictionary[key] = levenshtein_distance
				
				combination_pair = str(combination_1) + '-' + str(combination_2)
				combination_pairs.append(combination_pair)
				trainingDataSet[combination_pair] = tempDictionary
		
		outputDatabase = adt.evaluate(trainingDataSet, adtree)
		
		correct_classifications = 0
		incorrect_classifications = 0
		
		false_positives = 0
		false_negatives = 0
		for combination_pair in outputDatabase.keys():
			pair = combination_pair.split("-")
			if records[int(pair[0])]["eID"] == records[int(pair[1])]["eID"] and outputDatabase[combination_pair]["classification"] == SAME:
				correct_classifications = correct_classifications + 1
			elif records[int(pair[0])]["eID"] != records[int(pair[1])]["eID"] and outputDatabase[combination_pair]["classification"] == DIFFERENT:
				correct_classifications = correct_classifications + 1
			else:
				if outputDatabase[combination_pair]["classification"] == SAME:
					print "False Positive:"
					false_positives = false_positives + 1
				elif outputDatabase[combination_pair]["classification"] == DIFFERENT:
					print "False Negative:"
					false_negatives = false_negatives + 1
				
				print "   record[%s][ID] = %s --- address: %s" % (pair[0], records[int(pair[0])]["eID"], records[int(pair[0])]["eFullAddress"])
				print "   record[%s][ID] = %s --- address: %s" % (pair[1], records[int(pair[1])]["eID"], records[int(pair[1])]["eFullAddress"])
				print "   classification = %s" % outputDatabase[combination_pair]["classification"]
				
				incorrect_classifications = incorrect_classifications + 1
		
		print ""
		print "Number of records        : %s" % num_records
		print "Number of combinations   : %s" % num_combinations
		
		print ""
		total_classifications = correct_classifications + incorrect_classifications
		print "Correct classifications  : %s" % correct_classifications
		print "Incorrect classifications: %s" % incorrect_classifications
		classification_accuracy_ratio = float(correct_classifications) / float(total_classifications)
		classification_accuracy_percentage = classification_accuracy_ratio * 100.0
		print "Classification Accuracy  : %.2f%%" % classification_accuracy_percentage
		
		print ""
		print "False Positives: %s" % false_positives
		print "False Negatives: %s" % false_negatives
		
	else:
		Usage()
		return
	
if __name__ == "__main__":
	main()
