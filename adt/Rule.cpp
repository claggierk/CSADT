#include "Rule.h"

Rule::Rule()
{}

Rule::Rule(const Rule& fs)
{}

Rule::Rule(const Condition& myPrecondition, const Condition& myCondition, const float& myTrueWeight, const float& myFalseWeight)
{
	precondition.setValue(myPrecondition.getValue());
	precondition.setComparison(myPrecondition.getComparison());
	precondition.setIndex(myPrecondition.getIndex());

	condition.setValue(myCondition.getValue());
	condition.setComparison(myCondition.getComparison());
	condition.setIndex(myCondition.getIndex());

	trueWeight = myTrueWeight;
	falseWeight = myFalseWeight;
}

Condition Rule::getPrecondition() const
{
	return precondition;
}

void Rule::setPrecondition(const Condition& myPrecondition)
{
	precondition.setValue(myPrecondition.getValue());
	precondition.setComparison(myPrecondition.getComparison());
	precondition.setIndex(myPrecondition.getIndex());
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

float Rule::getTrueWeight() const
{
	return trueWeight;
}

void Rule::setTrueWeight(const float& myTrueWeight)
{
	trueWeight = myTrueWeight;
}

float Rule::getFalseWeight() const
{
	return falseWeight;
}

void Rule::setFalseWeight(const float& myFalseWeight)
{
	falseWeight = myFalseWeight;
}

ostream& operator<<(ostream& out, const Rule& fs)
{
	return out;
}
