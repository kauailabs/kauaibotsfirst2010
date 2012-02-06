#include "RangeFinder.h"

RangeFinder::RangeFinder(UINT32 channel) :
	AnalogChannel(channel)
{
	
}

double RangeFinder::GetRangeInches()
{
	double dVoltage = GetAverageVoltage();
	// Datasheet says Inches = ( Vcc / 512 ), so 5.0 / 512 = .009765
	double dInches = dVoltage / .009765;
	return dInches;
}
