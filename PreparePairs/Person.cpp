#include <iostream>

#include "Person.h"

Person::Person()
{}

Person::Person(const Person & p)
{
    for(unsigned i = 0; i < p.attributes.size(); i++)
	{
		attributes.push_back(p.attributes.at(i));
	}
}

Person::Person(vector<string> personInfo)
{
	if(personInfo.size() != eSizePersonAttributes)
	{
		cerr << endl << " ##### ERROR: Supplied vector has incompatible length. Expected: " << eSizePersonAttributes << " --- Received: " << personInfo.size();
		return;
	}

    for(unsigned i = 0; i < personInfo.size(); i++)
	{
		attributes.push_back(personInfo.at(i));
	}
}

vector<string> Person::getAttributes() const
{
	return attributes;
}

ostream& operator<<(ostream& out, const Person& p)
{
	out << endl << "Requested Person:";
	if(p.getAttributes().size() == eSizePersonAttributes)
    {
        out << endl << stringify(eID) << ": " << p.getAttributes().at(eID);
        out << endl << stringify(eFullName) << ": " << p.getAttributes().at(eFullName);
        out << endl << stringify(eFirstName) << ": " << p.getAttributes().at(eFirstName);
        out << endl << stringify(eLastName) << ": " << p.getAttributes().at(eLastName);
        out << endl << stringify(eMiddleName) << ": " << p.getAttributes().at(eMiddleName);
        out << endl << stringify(eAge) << ": " << p.getAttributes().at(eAge);
        out << endl << stringify(eFullAddress) << ": " << p.getAttributes().at(eFullAddress);
        out << endl << stringify(eStreetAddress) << ": " << p.getAttributes().at(eStreetAddress);
        out << endl << stringify(eApt) << ": " << p.getAttributes().at(eApt);
        out << endl << stringify(eCity) << ": " << p.getAttributes().at(eCity);
        out << endl << stringify(eState) << ": " << p.getAttributes().at(eState);
        out << endl << stringify(eZipCode) << ": " << p.getAttributes().at(eZipCode);
        out << endl << stringify(dPhone) << ": " << p.getAttributes().at(dPhone);
        out << endl << stringify(eDate) << ": " << p.getAttributes().at(eDate);
        out << endl << stringify(eDob) << ": " << p.getAttributes().at(eDob);
        out << endl << stringify(eAge1) << ": " << p.getAttributes().at(eAge1);
        out << endl << stringify(eSource) << ": " << p.getAttributes().at(eSource);
        out << endl << stringify(eRelatives) << ": " << p.getAttributes().at(eRelatives);
        out << endl << stringify(eScore) << ": " << p.getAttributes().at(eScore);
        out << endl << stringify(eLabel) << ": " << p.getAttributes().at(eLabel);
        out << endl << stringify(eAreaCode) << ": " << p.getAttributes().at(eAreaCode);
        out << endl << stringify(eExchange) << ": " << p.getAttributes().at(eExchange);
        out << endl << stringify(eSubscriber) << ": " << p.getAttributes().at(eSubscriber);
        out << endl;
    }
    else
    {
        out << endl << "Requested Person's attributes are not yet populated.";
    }
    
	return out;
}
