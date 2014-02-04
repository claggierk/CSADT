import pydot

EXPECTED_INPUTS_FOR_NODE = 4

def PopulateNodes():
	nodes = []
	tree_file = open("Tree.txt", 'r')
	for node_index, node_line in enumerate(tree_file):
		local_node = node_line.split(" ")
		if len(local_node) != EXPECTED_INPUTS_FOR_NODE:
			print "##### ERROR: Expected number of inputs for node: %d... received: %d" % (EXPECTED_INPUTS_FOR_NODE, len(local_node))
			return
		local_node = ("node" + str(node_index+1), local_node[0], local_node[1], local_node[2], local_node[3])
		nodes.append(local_node)
	tree_file.close()
	return nodes

def IllustrateTree(nodes, identifier):
	graph = pydot.Dot(graph_type='digraph')
	dictionary_of_nodes = {}
	
	title = nodes[0][0]
	parent = nodes[0][1]
	condition = nodes[0][2]
	yes_weight = nodes[0][3]
	no_weight = nodes[0][4]

	# the text we display is node1 and the arbitrary weight
	root_text = title + "\n" + yes_weight
	node0 = pydot.Node(root_text, style="filled", fillcolor="#FFFFFF")
	
	# condition is used as the title ... since the child nodes refer to the root node as "True"
	dictionary_of_nodes[condition] = node0
	graph.add_node(node0)
	
	for node in nodes[1:]:
		##### node title # node condition # yes weight # no weight # parent node
		title = node[0]
		parent = node[1]
		condition = node[2]
		yes_weight = node[3]
		no_weight = node[4]
		
		node = pydot.Node(title + "\n" + condition, style="filled", fillcolor="#00BFFF")
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
			
	graph.write_png("CS_ADT_" + identifier + ".png")

def main():
	nodes = PopulateNodes()
	IllustrateTree(nodes, "Tree")

if __name__ == "__main__":
	main()