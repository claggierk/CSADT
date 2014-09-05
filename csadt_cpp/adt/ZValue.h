#ifndef __ZVALUE_H__
#define __ZVALUE_H__

#include "Precondition.h"
#include "Condition.h"

class ZValue
{
public:
    ZValue() {}
    ZValue(const ZValue& zValue)
    {
        p = zValue.GetPrecondition();
        c = zValue.GetCondition();
        z = zValue.GetZ();
    }
    ZValue(Precondition precondition, Condition condition, float myZ) : p(precondition), c(condition), z(myZ) {}
    
    Precondition GetPrecondition() const { return p; }
    Condition GetCondition() const { return c; }
    float GetZ() const { return z; }

    void SetPrecondition(const Precondition& precondition) { p = precondition; }
    void SetCondition(const Condition& condition) { c = condition; }
    void SetZ(const float& myZ) { z = myZ; }

private:
    Precondition p;
    Condition c;
    float z;
};

#endif