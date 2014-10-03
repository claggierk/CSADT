#ifndef __UTILS_H__
#define __UTILS_H__

#include <string>

using namespace std;

#define stringify(x) #x

enum PersonIndex
{
	eID = 0,
	eNameTitle,
	eFirstName,
	eMiddleName,
	eLastName,
	eNameSuffix,
	eAge,
	eDOB,
	ePhone,
	eStreet,
	eApt,
	eCity,
	eCounty,
	eState,
	eZIP,
	eZIP4,
	eLat,
	eLong,
	eRelatives,
	eRecordDate,
	eYearMined,
	eTrue,
	eDuplicateID,
	eSizePersonAttributes, // !
};

static string sPersonConditions[] = {
	stringify(eID),
	stringify(eNameTitle),
	stringify(eFirstName),
	stringify(eMiddleName),
	stringify(eLastName),
	stringify(eNameSuffix),
	stringify(eAge),
	stringify(eDOB),
	stringify(ePhone),
	stringify(eStreet),
	stringify(eApt),
	stringify(eCity),
	stringify(eCounty),
	stringify(eState),
	stringify(eZIP),
	stringify(eZIP4),
	stringify(eLat),
	stringify(eLong),
	stringify(eRelatives),
	stringify(eRecordDate),
	stringify(eYearMined),
	stringify(eTrue),
	stringify(eDuplicateID),
};

#endif
