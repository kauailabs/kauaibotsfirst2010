#ifndef __RANGE_FINDER_H
#define __RANGE_FINDER_H
#include "WPILib.h"
class RangeFinder : public AnalogChannel
{
public:
	RangeFinder(UINT32 channel);
	double GetRangeInches();
};

#endif // __RANGE_FINDER_H
