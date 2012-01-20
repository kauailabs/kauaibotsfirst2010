#include "OI.h"

OI::OI() 
{
	stick = new Joystick(JOYSTICK_PORT);
}

Joystick * OI::getJoystick() 
{
	return stick;
}

DriverStation * OI::getDriverStation()
{
	return DriverStation::GetInstance();
}
