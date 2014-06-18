#include <cstdlib>

#include "Condition.h"
#include "Instance.h"
#include "Person.h"
#include "Utils.h"

Condition::Condition() : value(0), comparison("=="), index(0), trueFlag(false), notFlag(false)
{}

Condition::Condition(const Condition& p)
{
	value = p.getValue();
	comparison = p.getComparison();
	index = p.getIndex();
	trueFlag = p.getTrueFlag();
	notFlag = p.getNotFlag();
}

Condition::Condition(const bool& myTrueFlag)
{
	// this is only used to make a 'TRUE' condition
	// initialize to 1!
	value = 1;
	comparison = "==";
	index = eTrue;
	notFlag = false;
	trueFlag = myTrueFlag;
}

Condition::Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex)
{
	value = myValue;
	comparison = myComparison;
	index = myIndex;
	notFlag = false;
	trueFlag = false;
}

Condition::Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex, const bool& myNotFlag, const bool& myTrueFlag)
{
	value = myValue;
	comparison = myComparison;
	index = myIndex;
	notFlag = myNotFlag;
	trueFlag = myTrueFlag;
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
		if(c == sValidComparisons[i])
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
	if (notFlag)
	{
		return not c;
	}
	else
	{
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

bool Condition::evaluate(const Instance& instance) const
{
	//if Condition's trueFlag is true i.e Condition(true), always return true
	if (trueFlag) {
		return true;
	}

	//use index so that I can run evaluate like conditionObject.evaluate(instanceObject) 
	unsigned localDiff = instance.getAttribute(index);

	if(comparison == sValidComparisons[eLessThan])				return considerNotFlag(localDiff < value);
	else if(comparison == sValidComparisons[eGreatherThan])		return considerNotFlag(localDiff > value);
	else if(comparison == sValidComparisons[eEqualTo])			return considerNotFlag(localDiff == value);
	else if(comparison == sValidComparisons[eLessThanOrEqual])	return considerNotFlag(localDiff <= value);
	else if(comparison == sValidComparisons[eGreaterThanOrEqual])return considerNotFlag(localDiff >= value);
	else
	{
		cerr << "##### ERROR: unknown comparison: " << comparison << endl;
		return false;
	}
}

Condition& Condition::operator=(const Condition& c)
{
	value = c.getValue();
	comparison = c.getComparison();
	index = c.getIndex();
	trueFlag = c.getTrueFlag();
	notFlag = c.getNotFlag();
	
	return *this;
}

bool Condition::operator==(const Condition& c) {
	if ((value == c.getValue()) and (comparison == c.getComparison()) and (index == c.getIndex()) and (trueFlag == c.getTrueFlag()) and (notFlag == c.getNotFlag())) {
		return true;
	} else {
		return false;
	}
}

ostream& operator<<(ostream& out, const Condition& i)
{
	/*cerr << "check-";
	i.getTrueFlag();
	i.getNotFlag();
	i.getIndex();
	i.getComparison();
	i.getValue();
	cerr << "checked.";*/
	//out << "Condition: Attribute[" << i.getIndex() << "] ... " << sPersonConditions[i.getIndex()] << " " << i.getComparison() << " " << i.getValue();
	if (i.getNotFlag()) {

		out << "(not (" << sPersonConditions[i.getIndex()] << i.getComparison() << i.getValue() << "))";
	} else {
		out << "(" << sPersonConditions[i.getIndex()] << i.getComparison() << i.getValue() << ")";
	}
	return out;
}