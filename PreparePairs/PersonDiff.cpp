#include <iostream>

#include "PersonDiff.h"

PersonDiff::PersonDiff()
{}

PersonDiff::PersonDiff(const PersonDiff & p)
{
    for(unsigned i = 0; i < p.differences.size(); i++)
	{
		differences.push_back(p.differences.at(i));
	}
}

PersonDiff::PersonDiff(const Person& p1, const Person &p2)
{
    for(unsigned i = 0; i < eSizePersonAttributes; i++)
	{
        /*
        string s1 = p1.getAttributes().at(i);
        string s2 = p2.getAttributes().at(i);
        unsigned levenshteinDistance = ComputeLevenshteinDistance(s1, s2);
        differences.push_back(levenshteinDistance);
		*/
        differences.push_back(ComputeLevenshteinDistance(p1.getAttributes().at(i), p2.getAttributes().at(i)));
	}
}

vector<unsigned> PersonDiff::getDifferences() const
{
	return differences;
}

ostream& operator<<(ostream& out, const PersonDiff& p)
{
	out << endl << "Requested PersonDiff:";
	if(p.getDifferences().size() == eSizePersonAttributes)
    {
        out << endl << stringify(eID) << ": " << p.getDifferences().at(eID);
        out << endl << stringify(eFullName) << ": " << p.getDifferences().at(eFullName);
        out << endl << stringify(eFirstName) << ": " << p.getDifferences().at(eFirstName);
        out << endl << stringify(eLastName) << ": " << p.getDifferences().at(eLastName);
        out << endl << stringify(eMiddleName) << ": " << p.getDifferences().at(eMiddleName);
        out << endl << stringify(eAge) << ": " << p.getDifferences().at(eAge);
        out << endl << stringify(eFullAddress) << ": " << p.getDifferences().at(eFullAddress);
        out << endl << stringify(eStreetAddress) << ": " << p.getDifferences().at(eStreetAddress);
        out << endl << stringify(eApt) << ": " << p.getDifferences().at(eApt);
        out << endl << stringify(eCity) << ": " << p.getDifferences().at(eCity);
        out << endl << stringify(eState) << ": " << p.getDifferences().at(eState);
        out << endl << stringify(eZipCode) << ": " << p.getDifferences().at(eZipCode);
        out << endl << stringify(dPhone) << ": " << p.getDifferences().at(dPhone);
        out << endl << stringify(eDate) << ": " << p.getDifferences().at(eDate);
        out << endl << stringify(eDob) << ": " << p.getDifferences().at(eDob);
        out << endl << stringify(eAge1) << ": " << p.getDifferences().at(eAge1);
        out << endl << stringify(eSource) << ": " << p.getDifferences().at(eSource);
        out << endl << stringify(eRelatives) << ": " << p.getDifferences().at(eRelatives);
        out << endl << stringify(eScore) << ": " << p.getDifferences().at(eScore);
        out << endl << stringify(eLabel) << ": " << p.getDifferences().at(eLabel);
        out << endl << stringify(eAreaCode) << ": " << p.getDifferences().at(eAreaCode);
        out << endl << stringify(eExchange) << ": " << p.getDifferences().at(eExchange);
        out << endl << stringify(eSubscriber) << ": " << p.getDifferences().at(eSubscriber);
        out << endl;
    }
    else
    {
        out << endl << "Requested PersonDiff's differences are not yet populated.";
    }
	
	return out;
}

unsigned ComputeLevenshteinDistance(string s1, string s2)
{
    return 0;
}
