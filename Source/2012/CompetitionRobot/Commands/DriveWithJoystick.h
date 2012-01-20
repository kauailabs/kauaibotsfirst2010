#ifndef DRIVE_WITH_JOYSTICK_H
#define DRIVE_WITH_JOYSTICK_H

#include "../CommandBase.h"

class DriveWithJoystick : public CommandBase 
{
public:
	DriveWithJoystick();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
};
#endif
