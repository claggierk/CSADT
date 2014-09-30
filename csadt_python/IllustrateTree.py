import pydot
import sys

EXPECTED_INPUTS_FOR_NODE = 4

POSSIBLE_CONDITION_SYMBOLS = ['<', '=', '>']

def Usage():
	print "**************************************************"
	print "Usage  : python %s [tree text file] [PNG output file]" % sys.argv[0]
	print "Example: python %s Tree.txt CS_ADT_Tree.png" % sys.argv[0]
	print "**************************************************"
	print 

def PopulateNodes(input_tree_file_name):
	nodes = []
	tree_file = open(input_tree_file_name, 'r')
	for node_index, node_line in enumerate(tree_file):
		local_node = node_line.split(" ")
		if len(local_node) != EXPECTED_INPUTS_FOR_NODE:
			print "##### ERROR: Expected number of inputs for node: %d... received: %d" % (EXPECTED_INPUTS_FOR_NODE, len(local_node))
			return
		if local_node[1][0] == '(' and local_node[1][-1] == ')':
			local_node[1] = local_node[1][1:-1] # remove the parenthesis from the '('condition')'
		local_node = ("node" + str(node_index+1), local_node[0], local_node[1], local_node[2], local_node[3])
		nodes.append(local_node)
	tree_file.close()
	#print "nodes: ", nodes
	return nodes

def DetermineLastCondition(parent_line):
	condition_symbol_largest_index = 0
	for character_index, character in enumerate(reversed(parent_line)):
		if character in POSSIBLE_CONDITION_SYMBOLS:
			condition_symbol_largest_index
			condition_symbol_largest_index = len(parent_line) - character_index - 1
			break
	#print condition_symbol_largest_index
	#print parent_line[condition_symbol_largest_index:]

	last_condition_start = 0
	for character_index, character in enumerate(reversed(parent_line[:condition_symbol_largest_index])):
		if character == '(':
			last_condition_start = condition_symbol_largest_index - character_index
			break
	#print "last_condition_start:", last_condition_start
	#print parent_line[last_condition_start]

	#print "last condition:", parent_line[last_condition_start:last_condition_end]

	last_condition_end = 0
	for character_index, character in enumerate(parent_line[condition_symbol_largest_index:]):
		if character == ')':
			last_condition_end = condition_symbol_largest_index + character_index
			break
	#print "last_condition_end:", last_condition_end
	#print parent_line[last_condition_end]

	last_condition = parent_line[last_condition_start:last_condition_end]

	if parent_line[last_condition_start-4:last_condition_start-1] == 'not':
		#####print "NOT last_condition: ", last_condition
		return last_condition, False

	#####print "    last_condition: ", last_condition
	return last_condition, True

def IllustrateTree(output_tree_file_name, nodes):
	graph = pydot.Dot(graph_type='digraph')
	dictionary_of_nodes = {}
	
	title = nodes[0][0]
	parent = nodes[0][1]
	condition = nodes[0][2]
	yes_weight = nodes[0][3]
	no_weight = nodes[0][4]
	'''
	print "title     :", title
	print "parent    :", parent
	print "condition :", condition
	print "yes_weight:", yes_weight
	print "no_weight :", no_weight
	'''

	# the text we display is node1 and the arbitrary weight
	root_text = title + "\n" + yes_weight
	node0 = pydot.Node(root_text, style="filled", fillcolor="#FFFFFF")
	
	# condition is used as the title ... since the child nodes refer to the root node as "True"
	dictionary_of_nodes[condition] = (node0, '', condition)
	#print dictionary_of_nodes
	graph.add_node(node0)
	
	for node in nodes[1:]:
		##### node title # node condition # yes weight # no weight # parent node
		title = node[0]
		parent = node[1]
		condition = node[2]
		yes_weight = node[3]
		no_weight = node[4]
		'''
		print "title     :", title
		print "parent    :", parent
		print "condition :", condition
		print "yes_weight:", yes_weight
		print "no_weight :", no_weight
		'''
		last_condition, yes_no = DetermineLastCondition(parent)
		
		node = pydot.Node(title + "\n" + condition, style="filled", fillcolor="#00BFFF")
		yes_node_title = title + "yes"
		yes_node = pydot.Node(yes_node_title, style="filled", fillcolor="#00FF00")
		no_node_title = title + "no"
		no_node = pydot.Node(no_node_title, style="filled", fillcolor="#FF0000")
		
		dictionary_of_nodes[title] = (node, '', condition)
		dictionary_of_nodes[yes_node_title] = (yes_node, True, condition)
		dictionary_of_nodes[no_node_title] = (no_node, False, condition)
		
		graph.add_node(node)
		graph.add_node(yes_node)
		graph.add_node(no_node)
		
		graph.add_edge(pydot.Edge(node, yes_node, label=yes_weight, labelfontcolor="#009933", fontsize="10.0", color="blue"))
		graph.add_edge(pydot.Edge(node, no_node, label=no_weight, labelfontcolor="#009933", fontsize="10.0", color="blue"))
		
		if parent == 'True': # account for the python tree
			graph.add_edge(pydot.Edge(dictionary_of_nodes[parent][0], node))
		elif parent == '(eTrue==0)': # account for the C++ tree
			# remove the parenthesis from the parent...
			graph.add_edge(pydot.Edge(dictionary_of_nodes[parent[1:-1]][0], node))
		else:
			for node_key in dictionary_of_nodes:
				#print "node:", node
				#print "node[2]: %s ====== %s" % (node[2], last_condition)
				if dictionary_of_nodes[node_key][2] == last_condition and dictionary_of_nodes[node_key][1] == yes_no:
					#print "FOUND IT!"
					graph.add_edge(pydot.Edge(dictionary_of_nodes[node_key][0], node))
					break
			
	graph.write_png(output_tree_file_name)

def main():
	for i, arg in enumerate(sys.argv):
		print "arg: %s: %s" % (i, arg) 
	argc = len(sys.argv)
	
	if argc != 3:
		Usage()
		return
	
	input_tree_file_name = sys.argv[1]
	output_tree_file_name = sys.argv[2]

	nodes = PopulateNodes(input_tree_file_name)
	IllustrateTree(output_tree_file_name, nodes)

if __name__ == "__main__":
	main()