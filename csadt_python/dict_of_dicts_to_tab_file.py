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
	
	return d[m+1][n+1]

def PopulateRecord(name, age, color):
	record = {}
	record["name"] = name
	record["age"] = age
	record["color"] = color
	return record

def OutputRecordsToTabulatedFile(records, tabulated_file, record_pairs, features):
	file_handler = open(tabulated_file, 'w')
	
	first_line = "combination" + "\t"
	for feature in features[:-1]:
		first_line = first_line + feature + "\t"
	first_line = first_line + features[-1] # no tab after the last "word" on the line
	file_handler.write("%s\n" % first_line)
	
	second_line = "U" + "\t" + "D" + "\t"
	for i in range(len(features)-1):
		second_line = second_line + "d\t"
	second_line = second_line + "d" # no tab after the last "word" on the line
	file_handler.write("%s\n" % second_line)
	
	for record in record_pairs:
		current_record = record + "\t"
		for feature in features[:-1]:
			current_record = current_record + str(records[record][feature]) + "\t"
		current_record = current_record + str(records[record][features[-1]]) # no tab after the last "word" on the line
		file_handler.write("%s\n" % current_record)
	
	file_handler.close()
			

def main():
	records = {}
	
	record0 = PopulateRecord("Clark", "27", "blue")
	record1 = PopulateRecord("Stan", "30", "blue")
	record2 = PopulateRecord("Clint", "20", "green")
	record3 = PopulateRecord("Steve", "34", "red")
	'''
	record4 = PopulateRecord("Carl", "29", "black")
	record5 = PopulateRecord("Sam", "27", "brown")
	record6 = PopulateRecord("Cameron", "26", "purple")
	record7 = PopulateRecord("Seth", "25", "orange")
	record8 = PopulateRecord("Collin", "22", "yellow")
	record9 = PopulateRecord("Sergio", "21", "aqua")
	'''
	
	record_pairs = ["0:1", "0:2", "0:3", "1:2", "1:3", "2:3"]
	features = ["name", "age", "color"]
	
	records["0:1"] = {}
	records["0:1"]["name"] = Compute_Levenshtein_Distance(record0["name"], record1["name"])
	records["0:1"]["age"] = Compute_Levenshtein_Distance(record0["age"], record1["age"])
	records["0:1"]["color"] = Compute_Levenshtein_Distance(record0["color"], record1["color"])
	
	records["0:2"] = {}
	records["0:2"]["name"] = Compute_Levenshtein_Distance(record0["name"], record2["name"])
	records["0:2"]["age"] = Compute_Levenshtein_Distance(record0["age"], record2["age"])
	records["0:2"]["color"] = Compute_Levenshtein_Distance(record0["color"], record2["color"])
	
	records["0:3"] = {}
	records["0:3"]["name"] = Compute_Levenshtein_Distance(record0["name"], record3["name"])
	records["0:3"]["age"] = Compute_Levenshtein_Distance(record0["age"], record3["age"])
	records["0:3"]["color"] = Compute_Levenshtein_Distance(record0["color"], record3["color"])
	
	records["1:2"] = {}
	records["1:2"]["name"] = Compute_Levenshtein_Distance(record1["name"], record2["name"])
	records["1:2"]["age"] = Compute_Levenshtein_Distance(record1["age"], record2["age"])
	records["1:2"]["color"] = Compute_Levenshtein_Distance(record1["color"], record2["color"])
	
	records["1:3"] = {}
	records["1:3"]["name"] = Compute_Levenshtein_Distance(record1["name"], record3["name"])
	records["1:3"]["age"] = Compute_Levenshtein_Distance(record1["age"], record3["age"])
	records["1:3"]["color"] = Compute_Levenshtein_Distance(record1["color"], record3["color"])
	
	records["2:3"] = {}
	records["2:3"]["name"] = Compute_Levenshtein_Distance(record2["name"], record3["name"])
	records["2:3"]["age"] = Compute_Levenshtein_Distance(record2["age"], record3["age"])
	records["2:3"]["color"] = Compute_Levenshtein_Distance(record2["color"], record3["color"])
	
	print "records: %s" % records
	
	OutputRecordsToTabulatedFile(records, "records.txt", record_pairs, features)

if __name__ == "__main__":
	main()
