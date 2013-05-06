#ifndef __CONDITION_H__
#define __CONDITION_H__

#include <vector>
#include <iostream>

using namespace std;

class Condition
{
public:
	Condition();
	Condition(const Condition& p);
	Condition(const unsigned& myValue, const char& myComparison, const unsigned& myIndex);

	unsigned getValue() const;
	void setValue(const unsigned& v);

	char getComparison() const;
	void setComparison(const char& c);

	unsigned getIndex() const;
	void setIndex(const unsigned& i);

private:
	unsigned value;
	char comparison;
	unsigned index;
};

ostream& operator<<(ostream& out, const Condition& i);

#endif
