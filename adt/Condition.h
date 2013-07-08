#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Condition
{
public:
	Condition();
	Condition(const Condition& p);
	Condition(const bool& myBypass);
	Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex);

	unsigned getValue() const;
	void setValue(const unsigned& v);

	string getComparison() const;
	void setComparison(const string& c);

	unsigned getIndex() const;
	void setIndex(const unsigned& i);

	bool isBypass() const;

	bool evaluate(unsigned localDiff) const;

private:
	unsigned value;
	string comparison;
	unsigned index;

	bool bypass;
};

ostream& operator<<(ostream& out, const Condition& i);

#endif
