#include "FeatureStats.h"

FeatureStats::FeatureStats()
{}

FeatureStats::FeatureStats(const FeatureStats& fs)
{
	index = fs.getIndex();

	sum = fs.getSum();
	sqSum = fs.getSqSum();
	max = fs.getMax();
	min = fs.getMin();
	mean = fs.getMean();
	stdDev = fs.getStdDev(); 
}

FeatureStats::FeatureStats(const unsigned& myIndex, const unsigned& mySum, const unsigned& mySqSum, const unsigned& myMin, const unsigned& myMax, const float& myMean, const float& myStdDev)
{
	index = myIndex;

	sum = mySum;
	sqSum = mySqSum;
	min = myMin;
	max = myMax;
	mean = myMean;
	stdDev = myStdDev;
}

unsigned FeatureStats::getIndex() const
{
	return index;
}

void FeatureStats::setIndex(const unsigned& i)
{
	index = i;
}

unsigned FeatureStats::getSum() const
{
	return sum;
}

void FeatureStats::setSum(const unsigned& s)
{
	sum = s;
}

unsigned FeatureStats::getSqSum() const
{
	return sqSum;
}

void FeatureStats::setSqSum(const unsigned& ss)
{
	sqSum = ss;
}

unsigned FeatureStats::getMin() const
{
	return min;
}

void FeatureStats::setMin(const unsigned& m)
{
	min = m;
}

unsigned FeatureStats::getMax() const
{
	return max;
}

void FeatureStats::setMax(const unsigned& m)
{
	max = m;
}

float FeatureStats::getMean() const
{
	return mean;
}

void FeatureStats::setMean(const float& m)
{
	mean = m;
}

float FeatureStats::getStdDev() const
{
	return stdDev;
}

void FeatureStats::setStdDev(const float& s)
{
	stdDev = s;
}

ostream& operator<<(ostream& out, const FeatureStats& fs)
{
	out << "Index: " << fs.getIndex() << endl
	    << "   Sum: " << fs.getSum() << endl
	    << "   Sqared Sum: " << fs.getSqSum() << endl
		<< "   Min: " << fs.getMin() << endl
		<< "   Max: " << fs.getMax() << endl
		<< "   Mean: " << fs.getMean() << endl
		<< "   Std Dev: " << fs.getStdDev() << endl
		<< endl;
	return out;
}