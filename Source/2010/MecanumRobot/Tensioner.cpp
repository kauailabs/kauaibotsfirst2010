#include "Tensioner.h"
#include <math.h>

const float cMinVoltage = 100;   // Fully Tightened
const float cMaxVoltage = 900.0; // Fully Loosened

const float cMinDistanceInFeet = 6;
const float cMaxDistanceInFeet = 25;

// voltageRange = cMaxVoltage = cMinVoltage
// distanceRange = cMaxDistanceInFeet - cMinDistanceInFeet
//orig
//tensionControl(0.1, 0.001, 0.0,
//best
//tensionControl(0.01, 0.0001, 0.005, 

Tensioner::Tensioner( 
		UINT32 iTensionMotorPort, UINT32 iAnalogPotentiometerPort) :
			m_TensionControlMotor(iTensionMotorPort),
			m_TensionPotentiometer(iAnalogPotentiometerPort),
			tensionControl(0.01, 0.0001, 0.005, 
					&m_TensionPotentiometer, 
					&m_TensionControlMotor,
					.25)
{
			tensionControl.SetInputRange(0.00,1000.00);			// potentiometers always give these values to PIDGet()
			tensionControl.SetOutputRange(-1,1);				// Jaguars always use these values ... already the default
			tensionControl.SetTolerance(1);						//  This is percent -- Controls the results of OnTarget()
			tensionControl.SetContinuous(true);
//			tensionControl.Enable();
}

void Tensioner::GetStatus( float *tensionerPotentiometerVoltage, 
					float *tensionerPotentiometerDistanceInFeet,
					TensionerMotorState *state)
{
	*tensionerPotentiometerVoltage = m_TensionPotentiometer.PIDGet();
//	*tensionerPotentiometerDistanceInFeet = tensionControl.GetError();
	*tensionerPotentiometerDistanceInFeet = tensionControl.GetSetpoint();
	//for status routine 
	*state = Reverse;
	if (tensionControl.OnTarget())
			*state = Off;
	else
		if (tensionControl.GetError()> 0)
			*state = Forward;
}
// Sets the tensioner to a value determined by the joystick
// we map the value into a number between cMinVoltage and cMaxVoltage (-0.4 and 2.8 
// where joystick of -1 is max and joystick of 1 is min
// Here are the various mappings involved
//           joystick      voltage   feet
// min       -1            2.8          6
// zero       0            1.4         12
// max       +1            -0.04       25

void Tensioner::SetTensioner( float joystickValue )
{
	float zeropoint = (cMaxVoltage - cMinVoltage)/2;	// half distance between max and min
	float voltageTarget = -joystickValue + 1;     		// value is between 0 and 2 where 0 is max
	voltageTarget = voltageTarget * zeropoint;			// value is scaled to max and min
	voltageTarget = voltageTarget + cMinVoltage;        // value is between max and min

	if (tensionControl.OnTarget())
		tensionControl.Disable();
	
	if (fabs(tensionControl.GetSetpoint() - voltageTarget) > 20)	// 20 volt bounce rejection
	{
		tensionControl.Enable();
		tensionControl.SetSetpoint(voltageTarget);
	}

	
// now calculate distance to Target
	zeropoint = (cMaxDistanceInFeet - cMinDistanceInFeet)/2;
	float distanceTarget = -joystickValue + 1;
	distanceTarget = distanceTarget * zeropoint;
	distanceTarget = distanceTarget + cMinDistanceInFeet; 
//	Wait(0.02);
}

void Tensioner::IncreaseTension()
{
	m_TensionControlMotor.Set(1);
}
void Tensioner::DecreaseTension()
{
	m_TensionControlMotor.Set(-1);
}
void Tensioner::StopMotor()
{
	m_TensionControlMotor.Set(0);
}
void Tensioner::SetContinuousMode(bool bOn)
{
	tensionControl.SetContinuous(bOn);
}
