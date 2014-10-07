#ifndef __ZVALUE_H__
#define __ZVALUE_H__

#include <limits>
#include <iostream>

#include "Precondition.h"
#include "Condition.h"


class ZValue
{
public:
    ZValue() : z(std::numeric_limits<double>::max()) {}
    ZValue(const ZValue& zValue)
    {
        p = zValue.GetPrecondition();
        c = zValue.GetCondition();
        z = zValue.GetZ();
    }
    ZValue(Precondition precondition, Condition condition, double myZ) : p(precondition), c(condition), z(myZ) {}
    ZValue(Precondition precondition, Condition condition) : p(precondition), c(condition), z(std::numeric_limits<double>::max())
    {
        std::cerr << endl << "Set z-value to max double" << endl;
    }
    
    Precondition GetPrecondition() const { return p; }
    Condition GetCondition() const { return c; }
    double GetZ() const { return z; }

    void SetPrecondition(const Precondition& precondition) { p = precondition; }
    void SetCondition(const Condition& condition) { c = condition; }
    void SetZ(const double& myZ) { z = myZ; }

private:
    Precondition p;
    Condition c;
    double z;
};

#endif