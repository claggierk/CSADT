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

	vector<unsigned> getDifferences() const;

private:
	vector<unsigned> differences;
};

ostream& operator<<(ostream& out, const PersonDiff& p);
unsigned ComputeLevenshteinDistance(string s, string t);

#endif
