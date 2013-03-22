#ifndef SWERVEDRIVE_H
#define SWERVEDRIVE_H


#include "Commands/Subsystem.h"
#include "../Robot.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class SwerveDrive: public Command {
public:
	SwerveDrive();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
protected:         
	double AdjustJoystickResponse( double dJoystickIn, double dAdjustment, double dExponent, double dMultiplier , double dDead);  
};

#endif
