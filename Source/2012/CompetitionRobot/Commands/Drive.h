#ifndef DRIVE_H
#define DRIVE_H

#include "../CommandBase.h"

class Drive : public CommandBase 
{
public:
	Drive();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
protected:
	double InputJoystickAdjust( double dJoystickIn, double dAdjustment, double dExponent, double dMultiplier , double dDead);
};
#endif
