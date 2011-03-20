#include "sloganarm.h"

sloganarm::sloganarm(UINT32 shoulderCANAddress,	UINT32 handCANAddress, UINT32 elev1CANAddress, UINT32 elev2CANAddress)
		:m_shoulder( shoulderCANAddress, CANJaguar::kVoltage),
		 m_hand( handCANAddress, CANJaguar::kVoltage),
		 m_elev1( elev1CANAddress, CANJaguar::kVoltage),
		 m_elev2( elev2CANAddress, CANJaguar::kVoltage)
		
{
		m_shoulder.ConfigMaxOutputVoltage(12.0);
		m_shoulder.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		m_elev1.ConfigMaxOutputVoltage(12.0);
		m_elev1.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		m_elev2.ConfigMaxOutputVoltage(12.0);
		m_elev2.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		m_hand.ConfigMaxOutputVoltage(12.0);
		m_hand.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		m_shoulder.EnableControl();
		m_hand.EnableControl();
		m_elev1.EnableControl();
		m_elev2.EnableControl();

}

void sloganarm::DoShoulder(bool b4, bool b2)
{
	if (b4 != 0)
	{
		m_shoulder.Set(12.0);
	}
	if (b2 != 0)
	{
		m_shoulder.Set(-12.0);
	}
	if ((b2 == 0) && (b4 == 0))
	{
		m_shoulder.Set(0.0);
	}
}

void sloganarm::DoHand(bool b5, bool b3)
{
	if (b5 != 0)
	{
		m_hand.Set(12.0);
	}
	if (b3 != 0)
	{
		m_hand.Set(-12.0);
	}
	if ((b5 == 0) && (b3 == 0))
	{
		m_hand.Set(0.0);
	}

}

void sloganarm::DoElevator1(float vZ)
{
	if (vZ > JoystickDead4)
	{
		m_elev1.Set(vZ * -24.0);
	}
	if (vZ < (-1 * JoystickDead4))
	{
		m_elev1.Set(vZ * -24.0);
	}
	if ((vZ <= JoystickDead4) && (vZ >= (-1 * JoystickDead4)))
	{
		m_elev1.Set(0.0);
	}
}

void sloganarm::DoElevator2(float vZ)
{
	if (vZ > JoystickDead4)
	{
		m_elev2.Set(vZ * -24.0);
	}
	if (vZ < (-1 * JoystickDead4))
	{
		m_elev2.Set(vZ * -24.0);
	}
	if ((vZ <= JoystickDead4) && (vZ >= (-1 * JoystickDead4)))
	{
		m_elev2.Set(0.0);
	}
}

sloganarm::~sloganarm()
{	
}
