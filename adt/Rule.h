#ifndef __RULE_H__
#define __RULE_H__

#include <iostream>
#include <vector>

#include "Condition.h"
#include "Precondition.h"

using namespace std;

class Rule
{
public:
	Rule();
	Rule(const Rule& fs);
	Rule(const Precondition& myPrecondition, const Condition& myCondition, const float& myTrueScore, const float& myFalseScore);

	Precondition getPrecondition() const;
	void addPrecondition(const Condition& myPrecondition);

	Condition getCondition() const;
	void setCondition(const Condition& myCondition);

	float getTrueScore() const;
	void setTrueScore(const float& myTrueScore);

	float getFalseScore() const;
	void setFalseScore(const float& myFalseScore);

	Rule& operator=(const Rule& c);

private:
	Precondition precondition;
	Condition condition;
	float trueScore;
	float falseScore; 
};

ostream& operator<<(ostream& out, const Rule& fs);

#endif
