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
	Rule(const Condition& myPrecondition, const Condition& myCondition, const float& myTrueScore, const float& myFalseScore);

	Condition getPrecondition() const;
	void setPrecondition(const Condition& myPrecondition);

	Condition getCondition() const;
	void setCondition(const Condition& myCondition);

	float getTrueScore() const;
	void setTrueScore(const float& myTrueScore);

	float getFalseScore() const;
	void setFalseScore(const float& myFalseScore);

private:
	Condition precondition;
	Condition condition;
	float trueScore;
	float falseScore; 
};

ostream& operator<<(ostream& out, const Rule& fs);

#endif
