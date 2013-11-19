#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <vector>
#include <iostream>
#include <string>

#include "Instance.h"

using namespace std;

class Condition
{
public:
	Condition();
	Condition(const Condition& p);
	Condition(const bool& myTrueFlag);
	Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex);

	bool getNotFlag() const;
	void setNotFlag(const bool& n);
	bool considerNotFlag(const bool& c) const;

	bool getTrueFlag() const;
	void setTrueFlag(const bool& t);

	unsigned getValue() const;
	void setValue(const unsigned& v);

	string getComparison() const;
	void setComparison(const string& c);

	// This corresponds to which field is being compared
	// index: 1 === eFullName
	unsigned getIndex() const;
	void setIndex(const unsigned& i);

	bool isBypass() const;

	bool evaluate(const Instance& instance) const;

private:
	bool notFlag;
	bool trueFlag;
	unsigned value;
	string comparison;
	unsigned index;

	bool bypass;
};

ostream& operator<<(ostream& out, const Condition& i);

#endif
