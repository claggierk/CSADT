#include "Combination.h"

Combination::Combination() : i1(0), i2(0)
{}

Combination::Combination(const Combination & p) : i1(p.i1), i2(p.i2)
{}

Combination::Combination(const unsigned& index1, const unsigned& index2) : i1(index1), i2(index2)
{}

unsigned Combination::getIndex1() const
{
    return i1;
}

unsigned Combination::getIndex2() const
{
    return i2;
}

void Combination::setIndices(unsigned index1, unsigned index2)
{
	i1 = index1;
	i2 = index2;
}
