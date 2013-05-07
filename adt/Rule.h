#ifndef __RULE_H__
#define __RULE_H__

#include <iostream>

#include "Condition.h"

using namespace std;

class Rule
{
public:
	Rule();
	Rule(const Rule& fs);
	Rule(const Condition& myPrecondition, const Condition& myCondition, const float& myTrueWeight, const float& myFalseWeight);

	Condition getPrecondition() const;
	void setPrecondition(const Condition& myPrecondition);

	Condition getCondition() const;
	void setCondition(const Condition& myCondition);

	float getTrueWeight() const;
	void setTrueWeight(const float& myTrueWeight);

	float getFalseWeight() const;
	void setFalseWeight(const float& myFalseWeight);

private:
	Condition precondition;
	Condition condition;
	float trueWeight;
	float falseWeight; 
};

ostream& operator<<(ostream& out, const Rule& fs);

#endif
