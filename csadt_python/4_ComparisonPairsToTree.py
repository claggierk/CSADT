import sys
import os
import copy
import string
import re
import math
import adt
import accuracy
import pickle
import levenshtein
import adt_infrastructure

global trainingDataSet

def Usage():
	print "**************************************************"
	print "Usage  : python %s [text (.txt) input file] [text (.txt) conditions file] [text (.txt) output file]" % sys.argv[0]
	print "Example: python %s ComparisonRecords.txt conditions.txt Tree.txt" % sys.argv[0]
	print "**************************************************"

def run10FoldCrossValidation(inputFile, classifier):
	learners = [classifier]
	results = accuracy.crossValidation(inputFile, learners, 10)
	return results

def PopulateConditions(conditions_file):
	conditions_list = []
	conditions_file_handler = open(conditions_file, 'r')
	for line in conditions_file_handler:
		line = line.strip()
		conditions_list.append(line)
	conditions_file_handler.close()
	print "##### conditions_list: %s" % conditions_list
	return conditions_list

def main():
	global trainingComparisonPairs
	for i, arg in enumerate(sys.argv):
		print "arg: %s: %s" % (i, arg) 
	argc = len(sys.argv)
	
	if argc != 4:
		Usage()
		return
	
	input_file = sys.argv[1]
	conditions_file = sys.argv[2]
	output_file = sys.argv[3]
	
	if os.path.exists(input_file):
		trainingComparisonPairs = adt_infrastructure.BuildComparisonPairsDataStructure(input_file)
		print trainingComparisonPairs["0-1"]
		
		allConditions = PopulateConditions(conditions_file)
		print("allConditions: %s" % allConditions)
		
		#Produce trained tree and results
		adtClassifier = adt.classifier(trainingComparisonPairs, allConditions, "people")
		print("adtClassifier: %s" % adtClassifier)
		
		adt_infrastructure.WriteTreeToFile(adtClassifier, output_file)
	else:
		Usage()
		return
	
if __name__ == "__main__":
	main()
