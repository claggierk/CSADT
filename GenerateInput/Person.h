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
#define stringify(x) #x

enum PersonIndex
{
	//eID = 0,		// 0
	eFullName,		// 1  // 0
	eFirstName,		// 2  // 1
	eLastName,		// 3  // 2
	eMiddleName,	// 4  // 3
	eAge,			// 5  // 4
	eFullAddress,	// 6  // 5
	eStreetAddress,	// 7  // 6
	eApt,			// 8  // 7
	eCity,			// 9  // 8
	eState,			// 10 // 9
	eZipCode,		// 11 // 10
	dPhone,			// 12 // 11
	eDate,			// 13 // 12
	eDob,			// 14 // 13
	eAge1,			// 15 // 14
	eSource,		// 16 // 15
	eRelatives,		// 17 // 16
	eScore,			// 18 // 17
	eLabel,			// 19 // 18
	eAreaCode,		// 20 // 19
	eExchange,		// 21 // 20
	eSubscriber,	// 22 // 21
	eSizePersonAttributes // !
};

static string sPersonConditions[] = {
    stringify(eFullName),
    stringify(eFirstName),
    stringify(eLastName),
    stringify(eMiddleName),
    stringify(eAge),
    stringify(eFullAddress),
    stringify(eStreetAddress),
    stringify(eApt),
    stringify(eCity),
    stringify(eState),
    stringify(eZipCode),
    stringify(eDate),
    stringify(eDob),
    stringify(eAge1),
    stringify(eSource),
    stringify(eRelatives),
    stringify(eScore),
    stringify(eLabel),
    stringify(eAreaCode),
    stringify(eExchange),
    stringify(eSubscriber)
};

#endif
