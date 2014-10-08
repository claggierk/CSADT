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
        preconditionIndex = zValue.GetPreconditionIndex();
        conditionOuterIndex = zValue.GetConditionOuterIndex();
        conditionInnerIndex = zValue.GetConditionInnerIndex();
    }

    ZValue(Precondition precondition, Condition condition, double myZ, unsigned preCondIndex, unsigned conditionOuter, unsigned conditionInner)
        : p(precondition), c(condition), z(myZ), preconditionIndex(preCondIndex), conditionOuterIndex(conditionOuter), conditionInnerIndex(conditionInner)
    {}

    ZValue(Precondition precondition, Condition condition)
        : p(precondition), c(condition), z(std::numeric_limits<double>::max()), preconditionIndex(0), conditionOuterIndex(0), conditionInnerIndex(0)
    {
        std::cerr << endl << "Set z-value to max double" << endl;
    }
    Precondition GetPrecondition() const { return p; }

    Condition GetCondition() const { return c; }
    double GetZ() const { return z; }
    unsigned GetPreconditionIndex() const { return preconditionIndex; }
    unsigned GetConditionOuterIndex() const { return conditionOuterIndex; }
    unsigned GetConditionInnerIndex() const { return conditionInnerIndex; }

    void SetPrecondition(const Precondition& precondition) { p = precondition; }
    void SetCondition(const Condition& condition) { c = condition; }
    void SetZ(const double& myZ) { z = myZ; }
    void SetPreconditionIndex(const unsigned& preCondIndex) { preconditionIndex = preCondIndex; }
    void SetConditionOuterIndex(const unsigned& conditionOuter) { conditionOuterIndex = conditionOuter; }
    void SetConditionInnerIndex(const unsigned& conditionInner) { conditionInnerIndex = conditionInner; }

    ZValue& operator=(const ZValue& zValue)
    {
        p = zValue.GetPrecondition();
        c = zValue.GetCondition();
        z = zValue.GetZ();

        preconditionIndex = zValue.GetPreconditionIndex();
        conditionOuterIndex = zValue.GetConditionOuterIndex();
        conditionInnerIndex = zValue.GetConditionInnerIndex();
        return *this;
    }

private:
    Precondition p;
    Condition c;
    double z;

    unsigned preconditionIndex;
    unsigned conditionOuterIndex;
    unsigned conditionInnerIndex;
};

#endif
