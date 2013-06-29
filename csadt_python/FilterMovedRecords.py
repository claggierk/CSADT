import sys
import os
import copy
import string
import re

def Compute_Levenshtein_Distance(s, t):
	m = len(s) - 1
	n = len(t) - 1
	# for all i and j, d[i,j] will hold the Levenshtein distance between
	# the first i characters of s and the first j characters of t;
	# note that d has (m+1)x(n+1) values
	d = []
	for i in range(m+2):
		d.append([])
		for j in range(n+2):
			d[i].append(0)
	
	for i in range(m+2):
		d[i][0] = i # the distance of any first string to an empty second string
	
	for j in range(n+2):
		d[0][j] = j # the distance of any second string to an empty first string
	
	for j in range(1, n+2):
		for i in range(1, m+2):
			#print "t[j]: %s --- s[i]: %s" % (t[j-1], s[i-1])
			if s[i-1] == t[j-1]:
				d[i][j] = d[i-1][j-1] # no operation required
			else:
				d[i][j] = min(
					d[i-1][j] + 1,  # a deletion
					d[i][j-1] + 1,  # an insertion
					d[i-1][j-1] + 1 # a substitution
				)
	
	return d[m][n]

def WriteRecords(output_file, keys, records):
	output_list = []
	
	output_list.append("|".join(keys))
	for record in records:
		record_information = []
		for key in keys:
			record_information.append(record[key])
		output_list.append("|".join(record_information))
	
	file_handler = open(output_file, 'w')
	file_handler.write("\n".join(output_list))
	file_handler.close()

def Usage():
	print "**************************************************"
	print "Usage  : python %s [text (.txt) input file] [text (.txt) output file]" % sys.argv[0]
	print "Example: python %s PreProcessedRecordLinkageInput.txt Records_Moved.txt" % sys.argv[0]
	print "**************************************************"

def BuildDataStructure(input_file):
	try:
		file_handler = open(input_file, 'r')
	except IOError as e:
		print "%s could not be opened" % input_file
	
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

def main():
	argc = len(sys.argv)
	
	if argc != 3:
		print "argc: %d" % argc
		Usage()
		return
	
	input_file = sys.argv[1]
	output_file = sys.argv[2]
	
	if os.path.exists(input_file):
		(records, keys) = BuildDataStructure(input_file)
		
		ids_that_moved = []
		for record in records:
			#print "record[Label] = %s" % record["Label"]
			if record["Label"] == "M":
				print "record[%s] moved" % record["ID"]
				if record["ID"] not in ids_that_moved:
					ids_that_moved.append(record["ID"])
		
		print "%s records moved" % len(ids_that_moved)
		records_that_moved = []
		for record in records:
			if record["ID"] in ids_that_moved:
				records_that_moved.append(record)
		
		WriteRecords(output_file, keys, records_that_moved)
				
	else:
		print "input_file: %s does not exist" % input_file
		Usage()
		return
	
if __name__ == "__main__":
	main()
