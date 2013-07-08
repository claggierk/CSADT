#ifndef __INSTANCE_H__
#define __INSTANCE_H__

#include <vector>
#include <iostream>

using namespace std;

class Instance
{
public:
	Instance();
	Instance(const Instance& p);
	Instance(const vector<unsigned>& personInfo, const unsigned& myIndex, const bool& myMatch);

	vector<unsigned> getAttributes() const;
	unsigned getAttribute(unsigned attributeIndex) const;
	bool isMatch() const;

	unsigned getIndex() const;
	void setIndex(unsigned i);

	float getWeight() const;
	void setWeight(const float& w);

private:
	vector<unsigned> attributes;
	unsigned index;
	bool match;
	float weight;
};

ostream& operator<<(ostream& out, const Instance& i);

#endif
