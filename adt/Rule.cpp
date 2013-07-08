#include "Rule.h"

Rule::Rule()
{}

Rule::Rule(const Rule& fs)
{}

Rule::Rule(const vector<Condition>& myPrecondition, const Condition& myCondition, const float& myTrueScore, const float& myFalseScore)
{
	precondition = myPrecondition;
	//setValue(myPrecondition.getValue());
	//precondition.setComparison(myPrecondition.getComparison());
	//precondition.setIndex(myPrecondition.getIndex());

	condition.setValue(myCondition.getValue());
	condition.setComparison(myCondition.getComparison());
	condition.setIndex(myCondition.getIndex());

	trueScore = myTrueScore;
	falseScore = myFalseScore;
}

vector<Condition> Rule::getPrecondition() const
{
	return precondition;
}

void Rule::addPrecondition(const Condition& myPrecondition)
{
	precondition.push_back(myPrecondition);
	//setValue(myPrecondition.getValue());
	//precondition.setComparison(myPrecondition.getComparison());
	//precondition.setIndex(myPrecondition.getIndex());
}

Condition Rule::getCondition() const
{
	return condition;
}

void Rule::setCondition(const Condition& myCondition)
{
	condition.setValue(myCondition.getValue());
	condition.setComparison(myCondition.getComparison());
	condition.setIndex(myCondition.getIndex());
}

float Rule::getTrueScore() const
{
	return trueScore;
}

void Rule::setTrueScore(const float& myTrueScore)
{
	trueScore = myTrueScore;
}

float Rule::getFalseScore() const
{
	return falseScore;
}

void Rule::setFalseScore(const float& myFalseScore)
{
	falseScore = myFalseScore;
}

ostream& operator<<(ostream& out, const Rule& fs)
{
	return out;
}
