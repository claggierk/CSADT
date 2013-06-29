import pydot

def DrawTree(nodes, identifier):
	graph = pydot.Dot(graph_type='digraph')
	dictionary_of_nodes = {}
	
	root_title = "node0 = " + nodes[0]
	node0 = pydot.Node(root_title, style="filled", fillcolor="#FFFFFF")
	
	dictionary_of_nodes[root_title] = node0
	graph.add_node(dictionary_of_nodes[root_title])
	
	print ""
	for node in nodes[1:]:
		#print "node: %s" % node
		node_values = node.split("#")
		
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
		'''
		parent_edge = pydot.Edge(dictionary_of_nodes[parent], node)
		yes_edge = pydot.Edge(node, yes_node, label=yes_weight, labelfontcolor="#009933", fontsize="10.0", color="blue")
		no_edge = pydot.Edge(node, no_node, label=no_weight, labelfontcolor="#009933", fontsize="10.0", color="blue")
		
		graph.add_edge(parent_edge)
		graph.add_edge(yes_edge)
		graph.add_edge(no_edge)
		'''
		graph.add_edge(pydot.Edge(node, yes_node, label=yes_weight, labelfontcolor="#009933", fontsize="10.0", color="blue"))
		graph.add_edge(pydot.Edge(node, no_node, label=no_weight, labelfontcolor="#009933", fontsize="10.0", color="blue"))
		graph.add_edge(pydot.Edge(dictionary_of_nodes[parent], node))
			
	graph.write_png("CS_ADT_" + identifier + ".png")