#include "OI.h"

OI::OI() 
{
	stick = new Joystick(JOYSTICK_PORT);
	// Process operator interface input here.
}

Joystick* OI::getJoystick(){
	
	return stick;
}

DriverStation *OI::getDriverStation()
{
	return DriverStation::GetInstance();
}
