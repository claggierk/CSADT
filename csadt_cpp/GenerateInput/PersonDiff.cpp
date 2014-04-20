#include "PersonDiff.h"
#include "Utils.h"

PersonDiff::PersonDiff()
{}

// copy constructor
PersonDiff::PersonDiff(const PersonDiff & p)
{
    for(unsigned i = 0; i < p.differences.size(); i++)
	{
		differences.push_back(p.differences.at(i));
	}

	i1 = p.getIndex1();
    i2 = p.getIndex2();

    id1 = p.getID1();
    id2 = p.getID2();

    match = p.isMatch();
    weight = p.getWeight();
}

// compute the difference of two 'Person's
PersonDiff::PersonDiff(const Person& p1, const Person &p2)
{
	setDifferences(p1, p2);
    i1 = p1.getIndex();
    i2 = p2.getIndex();
    id1 = p1.getID();
    id2 = p2.getID();
    deduceMatch();
}

void PersonDiff::deduceMatch()
{
    match = false;
    if(id1 == id2)
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

unsigned PersonDiff::getID1() const
{
    return id1;
}

void PersonDiff::setID1(unsigned id)
{
    id1 = id;
}

unsigned PersonDiff::getID2() const
{
    return id2;
}

void PersonDiff::setID2(unsigned id)
{
    id2 = id;
}

void PersonDiff::setIDs(unsigned i1, unsigned i2)
{
    id1 = i1;
    id2 = i2;
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
    // fast deep copy
    differences.clear();
    differences.reserve(v.size());
    copy(v.begin(), v.end(), back_inserter(differences));
}

void PersonDiff::setDifferences(const Person& p1, const Person &p2)
{
    for(unsigned i = 0; i < eSizePersonAttributes; i++)
	{
        differences.push_back(ComputeLevenshteinDistance(p1.getAttributes().at(i), p2.getAttributes().at(i)));
	}
}

bool PersonDiff::isMatch() const
{
	return match;
}

ostream& operator<<(ostream& out, const PersonDiff& p)
{
    string matchStr = "non-match";
    if(p.isMatch())
    {
        matchStr = "match";
    }
	out << endl << "Requested PersonDiff: people[" << p.getIndex1() << "] cmp people[" << p.getIndex2() << "] (" << matchStr << ")";
	if(p.getDifferences().size() == eSizePersonAttributes)
    {
        for(unsigned differenceIndex = 0; differenceIndex < p.getDifferences().size(); differenceIndex++)
        {
            out << endl << "   " << sPersonConditions[differenceIndex] << ": " << p.getDifferences().at(differenceIndex);
        }
        out << endl << "   " << "Weight: " << p.getWeight();
        out << endl;
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
                    d.at(i-1).at(j) + 1,        // a deletion
                    min(d.at(i).at(j-1) + 1,    // an insertion
                    d.at(i-1).at(j-1) + 1)      // a substitution
                );
            }
        }
    }
    return d.at(m+1).at(n+1);
}
