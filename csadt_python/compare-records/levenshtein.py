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