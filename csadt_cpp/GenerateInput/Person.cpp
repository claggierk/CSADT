#include "Person.h"
#include "Utils.h"

Person::Person()
{}

// copy constuctor
Person::Person(const Person & p)
{
    for(unsigned i = 0; i < p.attributes.size(); i++)
	{
		attributes.push_back(p.attributes.at(i));
	}

    // fast deep copy
    vector<string> pAttributes = p.getAttributes();
    attributes.clear();
    attributes.reserve(pAttributes.size());
    copy(pAttributes.begin(), pAttributes.end(), back_inserter(attributes));

    id = p.getID();
    index = p.getIndex();
}

Person::Person(vector<string> personInfo, unsigned myIndex, unsigned myID)
{
	if(personInfo.size() != eSizePersonAttributes)
	{
		cerr << endl << " ##### ERROR: Supplied vector has incompatible length. Expected: " << eSizePersonAttributes << " --- Received: " << personInfo.size();
		return;
	}

    // fast deep copy
    attributes.clear();
    attributes.reserve(personInfo.size());
    copy(personInfo.begin(), personInfo.end(), back_inserter(attributes));

    index = myIndex;
    id = myID;
}

// set the unique identifier of THIS person
void Person::setID(unsigned i)
{
    id = i;
}

// retrieve the unique identifier of THIS person
unsigned Person::getID() const
{
    return id;
}

unsigned Person::getIndex() const
{
    return index;    
}

void Person::setIndex(unsigned i)
{
    index = i;
}

vector<string> Person::getAttributes() const
{
	return attributes;
}

ostream& operator<<(ostream& out, const Person& p)
{
	out << endl << "Requested Person: people[" << p.getIndex() << "] (Duplicate ID: " << p.getID() << ")";
	if(p.getAttributes().size() == eSizePersonAttributes)
    {
        for(unsigned attributeIndex = 0; attributeIndex < p.getAttributes().size(); attributeIndex++)
        {
            out << endl << "   " << sPersonConditions[attributeIndex] << ": " << p.getAttributes().at(attributeIndex);
        }
        out << endl;
    }
    else
    {
        out << endl << "Requested Person's attributes are not yet populated.";
    }
    
	return out;
}