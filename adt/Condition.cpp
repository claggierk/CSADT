#include "Condition.h"

Condition::Condition()
{}

Condition::Condition(const Condition& p)
{
	value = p.getValue();
	comparison = p.getComparison();
	index = p.getIndex();
}

Condition::Condition(const unsigned& myValue, const char& myComparison, const unsigned& myIndex)
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

ostream& operator<<(ostream& out, const Condition& i)
{
	out << "index: " << i.getIndex() << " " << i.getComparison() << " value: " << i.getValue() << endl;
	return out;
}