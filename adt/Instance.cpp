#include "Instance.h"

using namespace std;

Instance::Instance()
{}

Instance::Instance(const Instance& p)
{
	vector<unsigned> v = p.getAttributes();

	// fast deep vector copy
    attributes.reserve(v.size());
    copy(v.begin(), v.end(), back_inserter(attributes));

    index = p.getIndex();
    match = p.isMatch();
    weight = p.getWeight();
}

Instance::Instance(const vector<unsigned>& inputData, const unsigned& myIndex, const bool& myMatch)
{
	attributes.reserve(inputData.size());
	copy(inputData.begin(), inputData.end(), back_inserter(attributes));
	index = myIndex;
	match = myMatch;
	weight = 0.0f;
}

vector<unsigned> Instance::getAttributes() const
{
	return attributes;
}

bool Instance::isMatch() const
{
	return match;
}

unsigned Instance::getIndex() const
{
	return index;
}

void Instance::setIndex(unsigned i)
{
	index = i;
}

float Instance::getWeight() const
{
	return weight;
}

void Instance::setWeight(const float& w)
{
	weight = w;
}

ostream& operator<<(ostream& out, const Instance& inst)
{
	out << " Instance[" << inst.getIndex() << "]: --- Match: " << inst.isMatch() << endl;
	out << "   ";
	vector<unsigned> attributes = inst.getAttributes();
	for(unsigned i = 0; i < attributes.size(); i++)
	{
		out << attributes.at(i) << " ";
	}
	out << endl;
	return out;
}