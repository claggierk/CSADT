#ifndef __PERSON_DIFF_H__
#define __PERSON_DIFF_H__

#include <vector>
#include <string>

#include "Person.h"

using namespace std;

class PersonDiff
{
public:
	PersonDiff();
	PersonDiff(const PersonDiff & p);
	PersonDiff(const Person& p1, const Person &p2);
	PersonDiff(unsigned index1, unsigned index2);
	PersonDiff(const vector<unsigned>& v, const bool& isMatch);

	vector<unsigned> getDifferences() const;
	void setDifferencesDirectly(const vector<unsigned>& v);
	void setDifferences(const Person& p1, const Person &p2);
	
	unsigned getIndex1() const;
    unsigned getIndex2() const;
    
    void setIndices(unsigned index1, unsigned index2);
    
    bool isMatch() const;

private:
	vector<unsigned> differences;
	unsigned i1;
    unsigned i2;
    
    bool match;
};

ostream& operator<<(ostream& out, const PersonDiff& p);
unsigned ComputeLevenshteinDistance(string s, string t);

#endif
