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
	i1 = p.getIndex1();
    i2 = p.getIndex2();
}

PersonDiff::PersonDiff(const Person& p1, const Person &p2)
{
	setDifferences(p1, p2);
}

PersonDiff::PersonDiff(unsigned index1, unsigned index2) : i1(index1), i2(index2)
{}

PersonDiff::PersonDiff(const vector<unsigned>& v, const bool& isM)
{
    setDifferencesDirectly(v);
    match = false;
    if(isM)
    {
        match = true;
    }
}

unsigned PersonDiff::getIndex1() const
{
    return i1;
}

unsigned PersonDiff::getIndex2() const
{
    return i2;
}

float PersonDiff::getWeight() const
{
    return weight;
}

void PersonDiff::setWeight(float w)
{
    weight = w;
}

void PersonDiff::setIndices(unsigned index1, unsigned index2)
{
	i1 = index1;
	i2 = index2;
}

// -----------------------------------------------------------------------

vector<unsigned> PersonDiff::getDifferences() const
{
	return differences;
}

void PersonDiff::setDifferencesDirectly(const vector<unsigned>& v)
{
    differences.clear();
    differences.reserve(v.size());
    copy(v.begin(), v.end(), back_inserter(differences));
}

void PersonDiff::setDifferences(const Person& p1, const Person &p2)
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
	
	match = false;
	if(p1.getAttributes().at(eID) == p1.getAttributes().at(eID))
	{
		match = true;
	}
}

bool PersonDiff::isMatch() const
{
	return match;
}

ostream& operator<<(ostream& out, const PersonDiff& p)
{
	out << endl << "Requested PersonDiff:";
	if(p.getDifferences().size() == eSizePersonAttributes)
    {
        out << endl << "   " << stringify(eID) << ": " << p.getDifferences().at(eID);
        out << endl << "   " << stringify(eFullName) << ": " << p.getDifferences().at(eFullName);
        out << endl << "   " << stringify(eFirstName) << ": " << p.getDifferences().at(eFirstName);
        out << endl << "   " << stringify(eLastName) << ": " << p.getDifferences().at(eLastName);
        out << endl << "   " << stringify(eMiddleName) << ": " << p.getDifferences().at(eMiddleName);
        out << endl << "   " << stringify(eAge) << ": " << p.getDifferences().at(eAge);
        out << endl << "   " << stringify(eFullAddress) << ": " << p.getDifferences().at(eFullAddress);
        out << endl << "   " << stringify(eStreetAddress) << ": " << p.getDifferences().at(eStreetAddress);
        out << endl << "   " << stringify(eApt) << ": " << p.getDifferences().at(eApt);
        out << endl << "   " << stringify(eCity) << ": " << p.getDifferences().at(eCity);
        out << endl << "   " << stringify(eState) << ": " << p.getDifferences().at(eState);
        out << endl << "   " << stringify(eZipCode) << ": " << p.getDifferences().at(eZipCode);
        out << endl << "   " << stringify(dPhone) << ": " << p.getDifferences().at(dPhone);
        out << endl << "   " << stringify(eDate) << ": " << p.getDifferences().at(eDate);
        out << endl << "   " << stringify(eDob) << ": " << p.getDifferences().at(eDob);
        out << endl << "   " << stringify(eAge1) << ": " << p.getDifferences().at(eAge1);
        out << endl << "   " << stringify(eSource) << ": " << p.getDifferences().at(eSource);
        out << endl << "   " << stringify(eRelatives) << ": " << p.getDifferences().at(eRelatives);
        out << endl << "   " << stringify(eScore) << ": " << p.getDifferences().at(eScore);
        out << endl << "   " << stringify(eLabel) << ": " << p.getDifferences().at(eLabel);
        out << endl << "   " << stringify(eAreaCode) << ": " << p.getDifferences().at(eAreaCode);
        out << endl << "   " << stringify(eExchange) << ": " << p.getDifferences().at(eExchange);
        out << endl << "   " << stringify(eSubscriber) << ": " << p.getDifferences().at(eSubscriber);
        out << endl << "   " << "Match : " << p.isMatch() ? "T" : "F";
        out << endl << "   " << "Weight: " << p.getWeight();
    }
    else
    {
        out << endl << "Requested PersonDiff's differences are not yet populated... only " << p.getDifferences().size() << " of " << eSizePersonAttributes;
    }
	
	return out;
}

unsigned ComputeLevenshteinDistance(string s, string t)
{
    // for all i and j, d[i,j] will hold the Levenshtein distance between
    // the first i characters of s and the first j characters of t;
    // note that d has (m+1)x(n+1) values
    
    unsigned m = s.size() - 1;
    unsigned n = t.size() - 1;
    
    vector< vector<unsigned> > d;
    for(unsigned i = 0; i < m+2; i++)
    {
        vector<unsigned> temp;
        d.push_back(temp);
        for(unsigned j = 0; j < n+2; j++)
        {
            d.at(i).push_back(0);
        }
    }
    
    for(unsigned i = 0; i < m+2; i++)
    {
        d.at(i).at(0) = i; // the distance of any first string to an empty second string
    }
    
    for(unsigned j = 0; j < n+2; j++)
    {
        d.at(0).at(j) = j; // the distance of any second string to an empty first string
    }
    
    for(unsigned j = 1; j < n+2; j++)
    {
        for(unsigned i = 1; i < m+2; i++)
        {
            if(s.at(i-1) == t.at(j-1))
            {
                d.at(i).at(j) = d.at(i-1).at(j-1); // no operation required
            }
            else
            {
                d.at(i).at(j) = min(
                    d.at(i-1).at(j) + 1,  // a deletion
                    min(d.at(i).at(j-1) + 1,  // an insertion
                    d.at(i-1).at(j-1) + 1) // a substitution
                );
            }
        }
    }
    return d.at(m+1).at(n+1);
}
