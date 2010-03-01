#ifndef _TENSIONER_H
#define _TENSIONER_H

#include "WPILib.h"
#include "PIDController.h"
class Tensioner
{
protected:
	Jaguar m_TensionControlMotor;
	AnalogChannel m_TensionPotentiometer;
	PIDController tensionControl;
	
public:
	enum TensionerMotorState { Forward, Reverse, Off };
	
	Tensioner( UINT32 iTensionMotorPort, UINT32 iAnalogPotentiometerPort);
	void GetStatus( float *tensionerPotentiometerVoltage, 
					float *tensionerPotentiometerDistanceInFeet,
					TensionerMotorState *state);
	void SetTensioner( float value ); 
    void IncreaseTension();
    void DecreaseTension();
    void StopMotor();
    void SetContinuousMode(bool bOn);
	
};

#endif // _KICKER_H

	
	
