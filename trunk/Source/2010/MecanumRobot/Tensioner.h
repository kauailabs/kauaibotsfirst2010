#ifndef _TENSIONER_H
#define _TENSIONER_H

#include "WPILib.h"

class Tensioner
{
protected:
	Jaguar m_TensionControlMotor;
	AnalogChannel m_TensionPotentiometer;
	
public:
	enum TensionerMotorState { Forward, Reverse, Off };
	
	Tensioner( UINT32 iTensionMotorPort, UINT32 iAnalogPotentiometerPort);
	void GetStatus( float *tensionerPotentiometerVoltage, 
					bool *tensionerPotentiometerDistanceInFeet,
					TensionerMotorState *state);
	void SetTensionerMotorState( TensionerMotorState state ); 

};

#endif // _KICKER_H

	
	
