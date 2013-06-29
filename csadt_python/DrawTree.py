import time
import commands
import sys
import pydot
import os.path

graph = pydot.Dot(graph_type='digraph')
dictionary_of_nodes = {}

def GetPredictionNodeValue():
	prediction_node_filename = "node_0.txt"
	while True:
		if os.path.isfile(prediction_node_filename):
			break
		else:
			print "Prediction node does not exist... waiting..."
			time.sleep(3)
	
	prediction_node_handler = open(prediction_node_filename, 'r')
	first_line = prediction_node_handler.readline()
	prediction_node_handler.close()
	return first_line

def DrawRootNode():
	prediction_node_value = GetPredictionNodeValue()
	root_title = "node0 = " + prediction_node_value
	node0 = pydot.Node(root_title, style="filled", fillcolor="#FFFFFF")
	#yes_node_title = root_title + "yes"
	#yes_node = pydot.Node(yes_node_title, style="filled", fillcolor="#00FF00")
	#no_node_title = root_title + "no"
	#no_node = pydot.Node(no_node_title, style="filled", fillcolor="#FF0000")
		
	dictionary_of_nodes.clear()
	dictionary_of_nodes[root_title] = node0
	#dictionary_of_nodes[yes_node_title] = yes_node
	#dictionary_of_nodes[no_node_title] = no_node
	
	graph.add_node(dictionary_of_nodes[root_title])
	#graph.add_node(dictionary_of_nodes[yes_node_title])
	#graph.add_node(dictionary_of_nodes[no_node_title])
	
	#graph.add_edge(pydot.Edge(dictionary_of_nodes[root_title], dictionary_of_nodes[yes_node_title]))
	#graph.add_edge(pydot.Edge(dictionary_of_nodes[root_title], dictionary_of_nodes[no_node_title]))
	
	graph.write_png('CS_ADT.png')

def DrawTree():
	node_files = []
	
	while True:
		print "Checking for new nodes..."
		node_files_string = commands.getoutput("/bin/ls node_*.txt")
		node_file_list = node_files_string.split("\n")
		
		# if there are 0 node_*.txt files... do nothing
		#>> ls node_*.txt
		#ls: node_*.txt: No such file or directory
		words = node_files_string.split(" ")
		if words[0] == "ls:":
			time.sleep(3)
			continue
		
		if "node_0.txt" in node_file_list:
			node_file_list.remove("node_0.txt")
			if len(node_file_list) == 0:
				time.sleep(3)
				continue
		
		for node_file in node_file_list:
			if node_file == "node_end.txt":
				sys.exit()
			elif node_file not in node_files:
				node_files.append(node_file)
				print "New node: %s" % node_file
				
				node_file_handler = open(node_file, 'r')
				first_line = node_file_handler.readline()
				node_file_handler.close()
				node_values = first_line.split("#")
				
				##### node title # node condition # yes weight # no weight # parent node
				title = node_values[0]
				condition = node_values[1]
				yes_weight = node_values[2]
				no_weight = node_values[3]
				parent = node_values[4]
				
				node = pydot.Node(title + " = " + condition, style="filled", fillcolor="#00BFFF")
				yes_node_title = title + "yes"
				yes_node = pydot.Node(yes_node_title, style="filled", fillcolor="#00FF00")
				no_node_title = title + "no"
				no_node = pydot.Node(no_node_title, style="filled", fillcolor="#FF0000")
				
				dictionary_of_nodes[title] = node
				dictionary_of_nodes[yes_node_title] = yes_node
				dictionary_of_nodes[no_node_title] = no_node
				
				graph.add_node(node)
				graph.add_node(yes_node)
				graph.add_node(no_node)
				
				graph.add_edge(pydot.Edge(node, yes_node, label=yes_weight, labelfontcolor="#009933", fontsize="10.0", color="blue"))
				graph.add_edge(pydot.Edge(node, no_node, label=no_weight, labelfontcolor="#009933", fontsize="10.0", color="blue"))
				graph.add_edge(pydot.Edge(dictionary_of_nodes[parent], node))
				
				graph.write_png('CS_ADT.png')
			else:
				time.sleep(3)
				continue

def main():
	DrawRootNode()
	DrawTree()
	
if __name__ == "__main__":
	main()
