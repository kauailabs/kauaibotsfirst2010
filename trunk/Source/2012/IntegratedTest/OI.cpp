#include "OI.h"

OI::OI() 
{
        stick = new Joystick(JOYSTICK_PORT);
        stick2 = new Joystick(JOYSTICK_PORT_2);
}

Joystick * OI::getJoystick() 
{
        return stick;
}

Joystick * OI::getShooterJoystick() 
{
        return stick2;
}

DriverStation * OI::getDriverStation()
{
        return DriverStation::GetInstance();
}
