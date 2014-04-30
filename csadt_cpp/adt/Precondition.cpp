#include <sstream>

#include "Precondition.h"

Precondition::Precondition()
{}

Precondition::Precondition(const vector<Condition>& vc)
{
	for(unsigned i = 0; i < vc.size(); i++)
	{
		conditions.push_back(vc.at(i));
	}
}

Precondition::Precondition(const Precondition& p)
{
	vector<Condition> psConditions = p.GetConditions();
	for(unsigned i = 0; i < psConditions.size(); i++)
	{
		conditions.push_back(psConditions.at(i));
	}
}

vector<Condition> Precondition::GetConditions() const
{
	return conditions;
}

void Precondition::AddCondition(const Condition& c)
{
	conditions.push_back(c);
}

void Precondition::Clear()
{
	conditions.clear();
}

Precondition& Precondition::operator=(const Precondition& p)
{
	conditions.clear();

	vector<Condition> psConditions = p.GetConditions();
	for(unsigned i = 0; i < psConditions.size(); i++)
	{
		conditions.push_back(psConditions.at(i));
	}
	return *this;
}

bool Precondition::operator==(const Precondition& p)
{
	//check to see if size is the same
	if (conditions.size() != p.GetConditions().size())
	{
		return false;
	}
	//check to see if every corresponding condition is the same
	for (unsigned i = 0; i < p.GetConditions().size(); i++)
	{
        if (not (conditions.at(i) == p.GetConditions().at(i)))
        {
        	return false;
        }
    }
    return true;
}

ostream& operator<<(ostream& out, const Precondition& p)
{
    for (unsigned i = 0; i < p.GetConditions().size(); i++)
    {
    	if (i != (p.GetConditions().size()-1))
    	{
    		out << p.GetConditions().at(i) << " and ";
    	}
    	else
    	{
    		out << p.GetConditions().at(i);
    	}
        
    }
    return out;
}