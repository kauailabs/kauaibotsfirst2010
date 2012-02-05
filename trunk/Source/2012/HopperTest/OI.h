#ifndef OI_H
#define OI_H

#include "WPILib.h"

class OI {
private:
	static const int JOYSTICK_PORT=1;
	Joystick *stick;
public:
	OI();
	Joystick *getJoystick();
	DriverStation *getDriverStation();
};

#endif
