#ifndef __PRECONDITION_H__
#define __PRECONDITION_H__

#include <vector>
#include <iostream>

#include "Condition.h"

using namespace std;

class Precondition
{
public:
	Precondition();

	Precondition(const vector<Condition>& vc);

	Precondition(const Precondition& p);

	vector<Condition> getConditions() const;

	void addCondition(const Condition& c);

	void clear();

	Precondition& operator=(const Precondition& c);

	bool operator==(const Precondition& c);

private:
	vector<Condition> conditions;
};

ostream& operator<<(ostream& out, const Precondition& p);

#endif
