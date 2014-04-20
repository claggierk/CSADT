#ifndef __FEATURE_STATS_H__
#define __FEATURE_STATS_H__

#include <iostream>

using namespace std;

class FeatureStats
{
public:
	FeatureStats();
	FeatureStats(const FeatureStats& fs);
	FeatureStats(const unsigned& myIndex, const unsigned& mySum, const unsigned& mySqSum, const unsigned& myMin, const unsigned& myMax, const float& myMean, const float& myStdDev);

	unsigned getIndex() const;
	void setIndex(const unsigned& i);

	unsigned getSum() const;
	void setSum(const unsigned& s);

	unsigned getSqSum() const;
	void setSqSum(const unsigned& ss);

	unsigned getMin() const;
	void setMin(const unsigned& m);

	unsigned getMax() const;
	void setMax(const unsigned& m);

	float getMean() const;
	void setMean(const float& m);

	float getStdDev() const;
	void setStdDev(const float& s);

private:
	unsigned index;

	unsigned long long sum;
	unsigned long long sqSum;
	unsigned min;
	unsigned max;
	float mean;
	float stdDev; 
};

ostream& operator<<(ostream& out, const FeatureStats& fs);

#endif
