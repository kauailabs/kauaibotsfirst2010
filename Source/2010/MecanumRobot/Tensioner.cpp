#include "Tensioner.h"

const float cMinVoltage = 1.63; // Fully Tightened
const float cMaxVoltage = 4.97; // Fully Loosened

const float cMinDistanceInFeet = 6;
const float cMaxDistanceInFeet = 35;

// voltageRange = cMaxVoltage = cMinVoltage
// distanceRange = cMaxDistanceInFeet - cMinDistanceInFeet

Tensioner::Tensioner( 
		UINT32 iTensionMotorPort, UINT32 iAnalogPotentiometerPort) :
			m_TensionControlMotor(iTensionMotorPort),
			m_TensionPotentiometer(iAnalogPotentiometerPort)			
{
	SetTensionerMotorState(Off);
}

void Tensioner::GetStatus( float *tensionerPotentiometerVoltage, 
					bool *tensionerPotentiometerDistanceInFeet,
					TensionerMotorState *state)
{
	*tensionerPotentiometerVoltage =  m_TensionPotentiometer.GetAverageVoltage();
	*tensionerPotentiometerDistanceInFeet = 0;
	if ( m_TensionControlMotor.Get() == 0)
	{
		*state = Off;
	}
	else if ( m_TensionControlMotor.Get() > 0)
	{
		*state = Forward;
	}
	else
	{
		*state = Reverse;
	}

}

void Tensioner::SetTensionerMotorState( TensionerMotorState state )
{
	switch ( state )
	{
	case Forward:
		m_TensionControlMotor.Set(1);
		break;
	case Reverse:
		m_TensionControlMotor.Set(-1);
		break;
	case Off:
		m_TensionControlMotor.Set(0);
	default:
		break;
	}
}
