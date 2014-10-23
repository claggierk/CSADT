import sys
import os
import string
import re
import math
import adt
import adt_infrastructure

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
	if len(sys.argv) != 3:
		Usage()
		return
	
	training_comparisons_file = sys.argv[1]
	adtree_file = sys.argv[2]
	
	if os.path.exists(training_comparisons_file) and os.path.exists(adtree_file):
		adtree = ReCreateADTree(adtree_file)
		
		number_of_matches, number_of_nonmatches, trainingComparisonPairs = adt_infrastructure.BuildComparisonPairsDataStructure(training_comparisons_file)
		outputDatabase = adt.evaluate(trainingComparisonPairs, adtree)
		print "LENGTH OF OUTPUTDATABASE: %s" % len(outputDatabase)
		
		correct_classifications = 0
		incorrect_classifications = 0
		
		number_of_predicted_matches = 0
		number_of_predicted_nonmatches = 0

		true_positives = 0
		true_negatives = 0
		false_positives = 0
		false_negatives = 0
		for combination_pair in outputDatabase.keys():
			# true positives
			if trainingComparisonPairs[combination_pair]["classification"] == SAME and outputDatabase[combination_pair]["classification"] == SAME:
				true_positives = true_positives + 1
				correct_classifications = correct_classifications + 1
				number_of_predicted_matches = number_of_predicted_matches + 1
			# true negatives
			elif trainingComparisonPairs[combination_pair]["classification"] == DIFFERENT and outputDatabase[combination_pair]["classification"] == DIFFERENT:
				true_negatives = true_negatives + 1
				correct_classifications = correct_classifications + 1
				number_of_predicted_nonmatches = number_of_predicted_nonmatches + 1
			else:
				# false positives
				if outputDatabase[combination_pair]["classification"] == SAME:
					#print "False Positive:", outputDatabase[combination_pair]
					false_positives = false_positives + 1
					incorrect_classifications = incorrect_classifications + 1
					number_of_predicted_matches = number_of_predicted_matches + 1
				# false negatives
				elif outputDatabase[combination_pair]["classification"] == DIFFERENT:
					#print "False Negative:", outputDatabase[combination_pair]
					false_negatives = false_negatives + 1
					incorrect_classifications = incorrect_classifications + 1
					number_of_predicted_nonmatches = number_of_predicted_nonmatches + 1
				else:
					print " **************************** THIS SHOULD NEVER HAPPEN"
		
		#print ""
		#print "Number of records             : %s" % num_records
		#print "Number of combinations        : %s" % num_combinations
		print ""
		print "Number of matches             : %s" % number_of_matches
		print "Number of nonmatches          : %s" % number_of_nonmatches
		print ""
		print "Number of predicted matches   : %s" % number_of_predicted_matches
		print "Number of predicted nonmatches: %s" % number_of_predicted_nonmatches
		print ""
		print "True Positives                : %s detected of %s (%s%%)" % (true_positives, number_of_matches, float(true_positives) / float(number_of_matches) * 100.0)
		print "True Negatives                : %s detected of %s (%s%%)" % (true_negatives, number_of_nonmatches, float (true_negatives) / float(number_of_nonmatches) * 100.0)
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
