#include <sstream>

#include "Precondition.h"

Precondition::Precondition()
{
	vector<Condition> conditions;
}

Precondition::Precondition(const vector<Condition>& vc) {
	conditions = vc;
}

Precondition::Precondition(const Precondition& p)
{
	conditions = p.getConditions();
}

vector<Condition> Precondition::getConditions() const {
	return conditions;
}

void Precondition::addCondition(const Condition& c) {
	conditions.push_back(c);
}

void Precondition::clear() {
	conditions.clear();
}

Precondition& Precondition::operator=(const Precondition& p) {
	conditions = p.getConditions();
	return *this;
}

bool Precondition::operator==(const Precondition& p) {
	//check to see if size is the same
	if (conditions.size() != p.getConditions().size()) {
		return false;
	}
	//check to see if every corresponding condition is the same
	for (unsigned i = 0; i < p.getConditions().size(); i++) {
        if (not (conditions.at(i) == p.getConditions().at(i))) {
        	return false;
        }
    }
    return true;
}

ostream& operator<<(ostream& out, const Precondition& p)
{
    for (unsigned i = 0; i < p.getConditions().size(); i++) {
    	if (i != (p.getConditions().size()-1)) {
    		out << p.getConditions().at(i) << " and ";
    	} else {
    		out << p.getConditions().at(i);
    	}
        
    }
    return out;
}