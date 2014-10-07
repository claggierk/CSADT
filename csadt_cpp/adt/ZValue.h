#ifndef __ZVALUE_H__
#define __ZVALUE_H__

#include <limits>
#include <iostream>

#include "Precondition.h"
#include "Condition.h"


class ZValue
{
public:
    ZValue() : z(std::numeric_limits<double>::max())
    {}

    ZValue(const ZValue& zValue)
    {
        p = zValue.GetPrecondition();
        c = zValue.GetCondition();
        z = zValue.GetZ();
        outerIndex = zValue.GetOuterIndex();
        innerIndex = zValue.GetInnerIndex();
    }

    ZValue(Precondition precondition, Condition condition, double myZ, unsigned outer, unsigned inner)
        : p(precondition), c(condition), z(myZ), outerIndex(outer), innerIndex(inner)
    {}

    ZValue(Precondition precondition, Condition condition)
        : p(precondition), c(condition), z(std::numeric_limits<double>::max()), outerIndex(0), innerIndex(0)
    {
        std::cerr << endl << "Set z-value to max double" << endl;
    }
    Precondition GetPrecondition() const { return p; }

    Condition GetCondition() const { return c; }
    double GetZ() const { return z; }
    unsigned GetOuterIndex() const { return outerIndex; }
    unsigned GetInnerIndex() const { return innerIndex; }

    void SetPrecondition(const Precondition& precondition) { p = precondition; }
    void SetCondition(const Condition& condition) { c = condition; }
    void SetZ(const double& myZ) { z = myZ; }
    void SetOuterIndex(const unsigned& outer) { outerIndex = outer; }
    void SetInnerIndex(const unsigned& inner) { innerIndex = inner; }

    ZValue& operator=(const ZValue& zValue)
    {
        p = zValue.GetPrecondition();
        c = zValue.GetCondition();
        z = zValue.GetZ();
        outerIndex = zValue.GetOuterIndex();
        innerIndex = zValue.GetInnerIndex();
        return *this;
    }

private:
    Precondition p;
    Condition c;
    double z;

    unsigned outerIndex;
    unsigned innerIndex;
};

#endif
