import sys
import csv

def WriteDataToFile(file_name, list):
	file_handler = open(file_name, 'w')
	file_handler.write("\n".join(list))
	file_handler.close()

def GetData(input_file):
	data = []

	input_file_handler = open(input_file, 'r')
	recordIDs = {}
	with open(input_file, 'rb') as csvfile:
		dataCSV = csv.reader(csvfile, delimiter=',', quotechar='"')
		for row in dataCSV:
			if row[0] in recordIDs:
				recordIDs[row[0]] = recordIDs[row[0]] + 1
			else:
				recordIDs[row[0]] = 1
			row[-1] = row[-1].strip()
			data.append(row)
	for recordID in recordIDs:
		if recordIDs[recordID] != 1:
			print "Match %s ... %s" % (recordID, recordIDs[recordID])
		else:
			print "Record %s has no matches" % recordID
	print "There are %s unique records" % len(recordIDs.keys())
	return data

def main():
	if(len(sys.argv) != 3):
		print "**************************************************"
		print "Usage  : python %s [CSV (.csv) input file] [text (.txt) output file]" % sys.argv[0]
		print "Example: python %s VA_UniqueGoodExplod_2009-2010-2012_Training.csv 2_100Lines.txt" % sys.argv[0]
		print "**************************************************"
		return
	
	input_file = sys.argv[1]
	text_output_file = sys.argv[2]
	
	data = GetData(input_file)
	
	output_txt_list = []
	output_html_list = []
	
	output_html_list.append("<table border=\"4\">\n")
	
	for row_index, row in enumerate(data):
		output_html_list.append("<tr>")
		
		cell_details = []
		for cell_index, cell in enumerate(row):
			output_html_list.append("<td>")
			output_html_list.append(str(cell))
			output_html_list.append("</td>")
			cell_details.append(str(cell))
		output_txt_list.append("|".join(cell_details))
		output_html_list.append("</tr>")
	
	output_html_list.append("</table>")
	
	WriteDataToFile(text_output_file, output_txt_list)
	WriteDataToFile("output.html", output_html_list)

if __name__ == "__main__":
	main()
	
