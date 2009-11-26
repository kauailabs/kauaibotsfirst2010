#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include <WPILib.H>

class Potentiometer : public AnalogChannel
{
  public:
    Potentiometer(UINT32 slot, UINT32 channel, float min =-1.0, float max =1.0);
    Potentiometer(UINT32 channel, float min =-1.0, float max =1.0);
    double PIDGet(void);
    float GetMax(void);
    float GetMin(void);

  protected:
	static const UINT32 kOversampleBits = 7;
	static const UINT32 kAverageBits = 3;
	static const float kSamplesPerSecond = 50.0;
	static const float kCalibrationSampleTime = 5.0;

	float m_Minimum;
	float m_Maximum;

  private:
	void InitPotentiometer(void);

};


#endif
