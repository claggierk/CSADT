#ifndef __PERSON_H__
#define __PERSON_H__

#include <vector>
#include <string>

using namespace std;

class Person
{
public:
	Person();
	Person(const Person & p);
	Person(vector<string> personInfo);

	vector<string> getAttributes() const;

private:
	vector<string> attributes;
};

ostream& operator<<(ostream& out, const Person& p);
#define stringify(x) #x

enum PersonIndex
{
	eID = 0,		// 0
	eFullName,		// 1
	eFirstName,		// 2
	eLastName,		// 3
	eMiddleName,	// 4
	eAge,			// 5
	eFullAddress,	// 6
	eStreetAddress,	// 7
	eApt,			// 8
	eCity,			// 9
	eState,			// 10
	eZipCode,		// 11
	dPhone,			// 12
	eDate,			// 13
	eDob,			// 14
	eAge1,			// 15
	eSource,		// 16
	eRelatives,		// 17
	eScore,			// 18
	eLabel,			// 19
	eAreaCode,		// 20
	eExchange,		// 21
	eSubscriber,	// 22
	eSizePersonAttributes	// !
};

#endif
