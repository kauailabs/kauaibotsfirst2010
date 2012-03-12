#include "Arduino.h"
#include "Timer.h"
#include "Utility.h"
#include <math.h>

void delay( int ms )
{
	Wait(double(ms / 1000));
}

long millis()
{
	return GetFPGATime() / 1000;   // microseconds / 1000 = milliseconds
}

float customround( float valToRound )
{
	return floor( valToRound + 0.5 );
}
