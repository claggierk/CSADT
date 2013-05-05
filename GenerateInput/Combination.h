#ifndef __COMBINATION_H__
#define __COMBINATION_H__

class Combination
{
public:
	Combination();
	Combination(const Combination & p);
	Combination(const unsigned& index1, const unsigned& index2);

	unsigned getIndex1() const;
    unsigned getIndex2() const;
    void setIndices(unsigned index1, unsigned index2);

private:
	unsigned i1;
    unsigned i2;
};

#endif
