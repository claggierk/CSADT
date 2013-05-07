#include "Condition.h"

Condition::Condition() : bypass(false)
{}

Condition::Condition(const Condition& p) : bypass(false)
{
	value = p.getValue();
	comparison = p.getComparison();
	index = p.getIndex();

	bypass = p.isBypass();
}

Condition::Condition(const bool& myBypass) : bypass(true)
{}

Condition::Condition(const unsigned& myValue, const char& myComparison, const unsigned& myIndex) : bypass(false)
{
	value = myValue;
	comparison = myComparison;
	index = myIndex;
}

unsigned Condition::getValue() const
{
	return value;
}

void Condition::setValue(const unsigned& v)
{
	value = v;
}

char Condition::getComparison() const
{
	return comparison;
}

void Condition::setComparison(const char& c)
{
	comparison = c;
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

ostream& operator<<(ostream& out, const Condition& i)
{
	out << "index: " << i.getIndex() << " " << i.getComparison() << " value: " << i.getValue() << endl;
	return out;
}