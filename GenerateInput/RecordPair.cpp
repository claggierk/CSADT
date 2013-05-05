#include "RecordPair.h"

RecordPair::RecordPair() : record1(0), record2(0)
{}

RecordPair::RecordPair(const RecordPair& rp) : record1(rp.record1), record2(rp.record2)
{}
