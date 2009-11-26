#include "Potentiometer.h"

void Potentiometer::InitPotentiometer(void)
{
	SetAverageBits(kAverageBits);
	SetOversampleBits(kOversampleBits);
	float sampleRate = kSamplesPerSecond * 
		(1 << (kAverageBits + kOversampleBits));

	GetModule()->SetSampleRate(sampleRate);

	Wait(1.0);

	InitAccumulator();
	Wait(kCalibrationSampleTime);

	INT64 value;
	UINT32 count;
	GetAccumulatorOutput(&value, &count);
	
	UINT32 center = (UINT32)((float)value / (float)count + .5);

	SetAccumulatorCenter(center);
	SetAccumulatorDeadband(0); ///< TODO: compute / parameterize this
	ResetAccumulator();
}


Potentiometer::Potentiometer(UINT32 slot, UINT32 channel, float min, float max)
 : AnalogChannel(slot, channel)
{
    m_Minimum = min;
    m_Maximum = max;
    InitPotentiometer();
}


Potentiometer::Potentiometer(UINT32 channel, float min, float max)
 : AnalogChannel(channel)
{
    m_Minimum = min;
    m_Maximum = max;
    InitPotentiometer();
}


double Potentiometer::PIDGet(void)
{
  return GetAverageVoltage();
}


float Potentiometer::GetMax()
{return m_Maximum;}

float Potentiometer::GetMin()
{return m_Minimum;}
