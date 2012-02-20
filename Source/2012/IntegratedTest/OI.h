#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI 
{
private:
        static const int JOYSTICK_PORT = 1;
        static const int JOYSTICK_PORT_2 = 2;
        Joystick *stick;
        Joystick *stick2;
public:
        OI();
        Joystick *getJoystick();
        Joystick *getShooterJoystick();
        DriverStation *getDriverStation();

};

#endif
