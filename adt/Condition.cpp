#include "Condition.h"

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

Condition::Condition() : bypass(false), value(0), comparison("=="), index(0)
{}

Condition::Condition(const Condition& p) : bypass(false)
{
	value = p.getValue();
	comparison = p.getComparison();
	index = p.getIndex();

	bypass = p.isBypass();
}

Condition::Condition(const bool& myBypass) : bypass(true), value(0), comparison("=="), index(0)
{}

Condition::Condition(const unsigned& myValue, const string& myComparison, const unsigned& myIndex) : bypass(false)
{
	setValue(myValue);
	setComparison(myComparison);
	setIndex(myIndex);
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

bool Condition::evaluate(unsigned localDiff) const
{
	if(comparison == validComparisons[eLessThan])				return (localDiff < value);
	else if(comparison == validComparisons[eGreatherThan])		return (localDiff > value);
	else if(comparison == validComparisons[eEqualTo])			return (localDiff == value);
	else if(comparison == validComparisons[eLessThanOrEqual])	return (localDiff <= value);
	else if(comparison == validComparisons[eGreaterThanOrEqual])return (localDiff >= value);
	else
	{
		cerr << "##### ERROR: unknown comparison: " << comparison << endl;
		return false;
	}
}

ostream& operator<<(ostream& out, const Condition& i)
{
	out << "Condition: " << i.getComparison() << " " << i.getValue();
	return out;
}