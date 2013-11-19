#include "Condition.h"
#include "Instance.h"
#include "Person.h"

enum comparisonIndices
{
	eLessThan,
	eGreatherThan,
	eEqualTo,
	eLessThanOrEqual,
	eGreaterThanOrEqual,
	eNumValidComparisons
};
string validComparisons[eNumValidComparisons] = {"<", ">", "==", "<=", ">="};

Condition::Condition() : bypass(false), value(0), comparison("=="), index(0), trueFlag(false), notFlag(false)
{}

Condition::Condition(const Condition& p) : bypass(false)
{
	value = p.getValue();
	comparison = p.getComparison();
	index = p.getIndex();
	trueFlag = p.getTrueFlag();
	notFlag = p.getNotFlag();

	bypass = p.isBypass();
}

Condition::Condition(const bool& myTrueFlag) : bypass(false)
{
	setValue(0);
	setComparison("==");
	setIndex(0);
	setTrueFlag(myTrueFlag);
	setNotFlag(false);
}

Condition::Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex) : bypass(false)
{
	setValue(myValue);
	setComparison(myComparison);
	setIndex(myIndex);
	setTrueFlag(false);
	setNotFlag(false);
}
bool Condition::getNotFlag() const
{
	return notFlag;
}

void Condition::setNotFlag(const bool& n)
{
	notFlag = n;
}

bool Condition::considerNotFlag(const bool& c) const
{
	if (notFlag) {
		return not c;
	} else {
		return c;
	}
}

bool Condition::getTrueFlag() const
{
	return trueFlag;
}

void Condition::setTrueFlag(const bool& t)
{
	trueFlag = t;
}

unsigned Condition::getValue() const
{
	return value;
}

void Condition::setValue(const unsigned& v)
{
	value = v;
}

string Condition::getComparison() const
{
	return comparison;
}

void Condition::setComparison(const string& c)
{
	for(unsigned i = 0; i < eNumValidComparisons; i++)
	{
		if(c == validComparisons[i])
		{
			comparison = c;
			return;
		}
	}
	cerr << "##### ERROR: Invalid comparison: " << c << endl;	
}

unsigned Condition::getIndex() const
{
	return index;
}

void Condition::setIndex(const unsigned& i)
{
	index = i;
}

bool Condition::isBypass() const
{
	return bypass;
}

bool Condition::evaluate(const Instance& instance) const
{
	//if Condition's trueFlag is true i.e Condition(true), always return true
	if (trueFlag) {
		return true;
	}

	//use index so that I can run evaluate like conditionObject.evaluate(instanceObject) 
	unsigned localDiff = instance.getAttribute(index);

	if(comparison == validComparisons[eLessThan])				return considerNotFlag(localDiff < value);
	else if(comparison == validComparisons[eGreatherThan])		return considerNotFlag(localDiff > value);
	else if(comparison == validComparisons[eEqualTo])			return considerNotFlag(localDiff == value);
	else if(comparison == validComparisons[eLessThanOrEqual])	return considerNotFlag(localDiff <= value);
	else if(comparison == validComparisons[eGreaterThanOrEqual])return considerNotFlag(localDiff >= value);
	else
	{
		cerr << "##### ERROR: unknown comparison: " << comparison << endl;
		return false;
	}
}

ostream& operator<<(ostream& out, const Condition& i)
{
	out << "Condition: Attribute[" << i.getIndex() << "] ... " << sPersonConditions[i.getIndex()] << " " << i.getComparison() << " " << i.getValue();
	return out;
}