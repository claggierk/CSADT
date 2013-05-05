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

	unsigned getID1() const;
	void setID1(unsigned id);
	unsigned getID2() const;
	void setID2(unsigned id);
	void setIDs(unsigned i1, unsigned i2);

    float getWeight() const;
    void setWeight(float w);
    
    void setIndices(unsigned index1, unsigned index2);
    
    bool isMatch() const;

private:
	vector<unsigned> differences;
	unsigned i1;
    unsigned i2;

    unsigned id1;
    unsigned id2;
    
    bool match;
    float weight;
};

ostream& operator<<(ostream& out, const PersonDiff& p);
unsigned ComputeLevenshteinDistance(string s, string t);

#endif
