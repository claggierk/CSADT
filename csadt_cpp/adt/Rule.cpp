#include "Rule.h"

Rule::Rule()
{}

Rule::Rule(const Rule& fs)
{
	precondition = fs.getPrecondition();
	condition = fs.getCondition();
	trueScore = fs.getTrueScore();
	falseScore = fs.getFalseScore();
}

Rule::Rule(const Precondition& myPrecondition, const Condition& myCondition, const float& myTrueScore, const float& myFalseScore)
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

Precondition Rule::getPrecondition() const
{
	return precondition;
}

void Rule::addPrecondition(const Condition& myPrecondition)
{
	precondition.AddCondition(myPrecondition);
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

Rule& Rule::operator=(const Rule& r)
{
	precondition = r.getPrecondition();
	condition = r.getCondition();
	trueScore = r.getTrueScore();
	falseScore = r.getFalseScore();
	
	return *this;
}

ostream& operator<<(ostream& out, const Rule& fs)
{
	out << endl;
	out << "Precondition: ";
	if(fs.getPrecondition().GetConditions().size() > 0)
	{
		out << fs.getPrecondition().GetConditions().at(0);
	}
	for(unsigned i = 1; i < fs.getPrecondition().GetConditions().size(); i++)
	{
		out << " and " << fs.getPrecondition().GetConditions().at(i);
	}
	out << endl;
	out << "Condition   : " << fs.getCondition() << endl;
	out << "trueScore   : " << fs.getTrueScore() << endl;
	out << "falseScore  : " << fs.getFalseScore() << endl;
	out << endl;
	return out;
}
