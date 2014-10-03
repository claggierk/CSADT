#include <sstream>

using std::ostringstream;

#include "Condition.h"
#include "Instance.h"
#include "Person.h"
#include "Utils.h"

Condition::Condition() : value(0), comparison("=="), index(0), notFlag(false)
{}

Condition::Condition(const Condition& c)
{
	value = c.getValue();
	comparison = c.getComparison();
	index = c.getIndex();
	notFlag = c.getNotFlag();
}

Condition::Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex)
{
	value = myValue;
	comparison = myComparison;
	index = myIndex;
	notFlag = false;
}

Condition::Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex, const bool& myNotFlag)
{
	value = myValue;
	comparison = myComparison;
	index = myIndex;
	notFlag = myNotFlag;
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

bool Condition::evaluate(const Instance& instance) const
{
    if (index == eTrue)
    {
		return considerNotFlag(true);
	}

	//use index so that I can run evaluate like conditionObject.evaluate(instanceObject) 
	unsigned localDiff = instance.getAttribute(index);

	if(comparison == sValidComparisons[eLessThan])
	{
		return considerNotFlag(localDiff < value);
	}
	else if(comparison == sValidComparisons[eGreatherThan])
	{
		return considerNotFlag(localDiff > value);
	}
	else if(comparison == sValidComparisons[eEqualTo])
	{
		return considerNotFlag(localDiff == value);
	}
	else if(comparison == sValidComparisons[eLessThanOrEqual])
	{
		return considerNotFlag(localDiff <= value);
	}
	else if(comparison == sValidComparisons[eGreaterThanOrEqual])
	{
		return considerNotFlag(localDiff >= value);
	}
	else
	{
		cerr << "##### ERROR: unknown comparison: " << comparison << endl;
		return false;
	}
}

string Condition::toString() const
{
	//out << "Condition: Attribute[" << i.getIndex() << "] ... " << sPersonConditions[i.getIndex()] << " " << i.getComparison() << " " << i.getValue();
	ostringstream oss;
	if (getNotFlag()) {
		oss << "not(" << sPersonConditions[getIndex()] << getComparison() << getValue() << ")";
	} else {
		oss << sPersonConditions[getIndex()] << getComparison() << getValue();
	}
	return oss.str();
}

Condition& Condition::operator=(const Condition& c)
{
	value = c.getValue();
	comparison = c.getComparison();
	index = c.getIndex();
	notFlag = c.getNotFlag();
	
	return *this;
}

bool Condition::operator==(const Condition& c) {
	if ((value == c.getValue()) and (comparison == c.getComparison()) and (index == c.getIndex()) and (notFlag == c.getNotFlag())) {
		return true;
	} else {
		return false;
	}
}

ostream& operator<<(ostream& out, const Condition& i)
{
	out << "(" << i.toString() << ")";
	return out;
}
