#ifndef __PERSON_H__
#define __PERSON_H__

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Person
{
public:
	Person();
	Person(const Person & p);
	Person(vector<string> personInfo, unsigned myIndex, unsigned myID);

	unsigned getID() const;
	void setID(unsigned id);

	unsigned getIndex() const;
	void setIndex(unsigned i);

	vector<string> getAttributes() const;
	static vector<string> PersonConditions;

private:
	vector<string> attributes;
	unsigned id;
	unsigned index;
};

ostream& operator<<(ostream& out, const Person& p);

#endif
