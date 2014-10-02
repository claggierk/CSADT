import sys
import os
import copy
import string
import re

AREA_CODE_STR = "AreaCode"
EXCHANGE_STR = "Exchange"
SUBSCRIBER_STR = "Subscriber"

BLANK = "" #"bl@nk"

def Usage():
	print "**************************************************"
	print "Usage  : python %s [text (.txt) input file] [text (.txt) output file]" % sys.argv[0]
	print "Example: python %s 100Lines.txt 100LinesPreprocessed.txt" % sys.argv[0]
	print "**************************************************"

def WritePreprocessedRecords(output_file, keys, preprocessed_records):	
	keys.append(AREA_CODE_STR)
	keys.append(EXCHANGE_STR)
	keys.append(SUBSCRIBER_STR)
	
	output_list = []

	# add prefix "e" to support the C++ enum...
	first_line = ["e" + key for key in keys]
	output_list.append("|".join(first_line))
	for record in preprocessed_records:
		record_information = []
		for key in keys:
			record_information.append(record[key])
		output_list.append("|".join(record_information))
	
	file_handler = open(output_file, 'w')
	file_handler.write("\n".join(output_list))
	file_handler.close()

def BuildDataStructure(input_file):
	print " ***** BuildDataStructure *****"
	file_handler = open(input_file, 'r')
	
	lines = file_handler.readlines()
	
	first_line = lines[0]
	first_line = first_line.strip()
	keys = first_line.split("|")
	print " ***** keys     : %s" % keys
	print " ***** len(keys): %s" % len(keys)
	
	records = []
	record = {}
	for key_index, key in enumerate(keys):
		record[key] = ""
	#print "record: %s" % record
	
	for line_index, line in enumerate(lines[1:]):		
		line = line.strip()
		cell_values = line.split("|")
		print "len(cell_values): %s" % len(cell_values)
		for cell_value_index, cell_value in enumerate(cell_values):
			print cell_value_index
			record[ keys[cell_value_index] ] = cell_value
		records.append(copy.deepcopy(record))
	
	file_handler.close()
	
	return (records, keys)

def PreprocessRecords(keys, records):
	print " ***** PreprocessRecords *****"
	for record in records:
		# Replace any field that only contains whitespace with ""
		for key in keys:
			record[key] = record[key].strip()
			if record[key] == "None":
				record[key] = BLANK
			elif record[key] is None:
				record[key] = BLANK
		record = PreprocessAge(record)
		record = PreprocessDate(record)
		record = PreprocessZipCode(record)
		record = PreprocessPhone(record)
	return records

def PreprocessPhone(record):
	record[AREA_CODE_STR] = ""
	record[EXCHANGE_STR] = ""
	record[SUBSCRIBER_STR] = ""
	
	complete_phone_re = re.compile(r'(\d{3})\D*(\d{3})\D*(\d{4})', re.VERBOSE)
	complete_phone_result = complete_phone_re.search(record["Phone"])
	
	incomplete_phone_re = re.compile(r'(\d{3})\D*(\d{4})', re.VERBOSE)
	incomplete_phone_result = incomplete_phone_re.search(record["Phone"])
	
	if complete_phone_result != None:
		#####print "PHONE --- area_code : %s" % complete_phone_result.group(0)
		#print "PHONE --- area_code : %s" % complete_phone_result.group(1)
		#print "PHONE --- exchange  : %s" % complete_phone_result.group(2)
		#print "PHONE --- subscriber: %s" % complete_phone_result.group(3)
		record[AREA_CODE_STR] = complete_phone_result.group(1)
		record[EXCHANGE_STR] = complete_phone_result.group(2)
		record[SUBSCRIBER_STR] = complete_phone_result.group(3)
	
	elif incomplete_phone_result != None:
		#####print "PHONE --- area_code : %s" % incomplete_phone_result.group(0)
		#print "PHONE --- exchange  : %s" % incomplete_phone_result.group(1)
		#print "PHONE --- subscriber: %s" % incomplete_phone_result.group(2)
		record[EXCHANGE_STR] = incomplete_phone_result.group(1)
		record[SUBSCRIBER_STR] = incomplete_phone_result.group(2)

def PreprocessZipCode(record):
	if record["Zipcode"] == "0":
		record["Zipcode"] = BLANK
	return record

def PreprocessDate(record):
	if record["Date"] == "Unknown":
		record["Date"] = BLANK
	return record

def PreprocessAge(record):
	if record["Age"] == "0":
		record["Age"] = BLANK
	return record

def main():
	argc = len(sys.argv)
	
	if argc != 3:
		Usage()
		return
	
	input_file = sys.argv[1]
	output_file = sys.argv[2]
	
	if os.path.exists(input_file):
		(records, keys) = BuildDataStructure(input_file)
		preprocessed_records = PreprocessRecords(keys, records)
		WritePreprocessedRecords(output_file, keys, preprocessed_records)
	else:
		Usage()
		return
	
if __name__ == "__main__":
	main()
