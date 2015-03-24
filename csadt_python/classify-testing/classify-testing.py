import sys
import os
import string
import re
import math
import adt
import pickle
import levenshtein
import adt_infrastructure

global testingDataSet

SAME = "SAME"
DIFFERENT = "DIFFERENT"

def Usage():
	print "**************************************************"
	print "Usage  : python %s [text (.txt) input file] [text (.txt) input file]" % sys.argv[0]
	print "Example: python %s testing_data.txt Tree.txt" % sys.argv[0]
	print "**************************************************"

def main():
	if len(sys.argv) != 3:
		Usage()
		return

	testing_data_file = sys.argv[1]
	adtree_file = sys.argv[2]

	if os.path.exists(testing_data_file) and os.path.exists(adtree_file):
		adtree = adt_infrastructure.ReCreateADTree(adtree_file, adt)

		(records, keys) = adt_infrastructure.GetTestingData(testing_data_file)
		testingDataSet = {}

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

		number_of_matches = 0
		number_of_nonmatches = 0

		for combination_1 in range(num_records):
			total_processed = total_processed + (num_records - combination_1) - 1
			percent_complete = float(total_processed) / float(num_combinations) * 100
			print " ***** %.2f%% complete ----- total processed: %s" % (percent_complete, total_processed)
			for combination_2 in range(combination_1+1, num_records):
				tempDictionary = {}

				for key in keys[1:]:
					levenshtein_distance = levenshtein.Compute_Levenshtein_Distance(records[combination_1][key], records[combination_2][key])
					tempDictionary[key] = levenshtein_distance

				combination_pair = str(combination_1) + '-' + str(combination_2)
				combination_pairs.append(combination_pair)
				testingDataSet[combination_pair] = tempDictionary

		outputDatabase = adt.evaluate(testingDataSet, adtree)

		for combination_pair in outputDatabase.keys():
			print combination_pair
	else:
		Usage()
		return

if __name__ == "__main__":
	main()
