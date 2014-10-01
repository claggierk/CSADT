import sys

class TreeBranch:
	def __init__(self, branch):
		self.pre_condition = branch[0]
		self.condition = branch[1]
		self.yes_score = branch[2]
		self.no_score = branch[3]

		self.simplify_precondition()
		self.simplify_condition()

	def simplify_precondition(self):
		self.pre_condition = self.pre_condition.replace("(", " ")
		self.pre_condition = self.pre_condition.replace(")", " ")

		parts = self.pre_condition.split()
		for part_index, part in enumerate(parts):
			if part != "and" and part != "not":
				parts[part_index] = "(" + part + ")"
			elif part == "not":
				parts[part_index] = "(" + part
				parts[part_index+1] = parts[part_index+1] + ")"

		self.pre_condition = "".join(parts)

	def simplify_condition(self):
		if self.condition[0] != '(':
			self.condition = "(" + self.condition + ")"

	def __str__(self):
		return "   Pre-Condition: %s\n   Condition: %s\n   Yes-Score: %s\n   No-Score: %s\n" % (self.pre_condition, self.condition, self.yes_score, self.no_score)

	def __repr__(self):
		return "   Pre-Condition: %s\n   Condition: %s\n   Yes-Score: %s\n   No-Score: %s\n" % (self.pre_condition, self.condition, self.yes_score, self.no_score)

def Usage():
	print "**************************************************"
	print "Usage  : python %s [input tree text file] [output tree text file]" % sys.argv[0]
	print "Example: python %s Tree.txt ReadyTree.txt" % sys.argv[0]
	print "**************************************************"
	print

def ExtractBranches(input_tree_file_name):
	branches = []
	input_file = open(input_tree_file_name, 'r')
	for line in input_file:
		branches.append( TreeBranch( line.split() ) )
	input_file.close()

	return branches

def WriteTree(output_tree_file_name, branches):
	output_file = open(output_tree_file_name, 'w')
	for branch in branches:
		output_file.write("%s %s %s %s\n" % (branch.pre_condition, branch.condition, branch.yes_score, branch.no_score))
	output_file.close()

def main():
	for i, arg in enumerate(sys.argv):
		print "arg: %s: %s" % (i, arg) 
	argc = len(sys.argv)
	
	if argc != 3:
		Usage()
		return
	
	input_tree_file_name = sys.argv[1]
	output_tree_file_name = sys.argv[2]

	branches = ExtractBranches(input_tree_file_name)
	WriteTree(output_tree_file_name, branches)

if __name__ == "__main__":
	main()
	