#ifndef __RECORD_PAIR__
#define __RECORD_PAIR__

#include <vector>

using namespace std;

class RecordPair
{
public:
    RecordPair();
    RecordPair(const RecordPair& rp);

private:
    unsigned record1;
    unsigned record2;
    vector<unsigned> comparisonScore;
};

#endif
