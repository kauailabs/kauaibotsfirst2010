#ifndef Chute_SUBSYSTEM_H
#define Chute_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class ChuteSubsystem: public Subsystem {
private:
	Servo* steering;
	Solenoid* liftingUp;
	Solenoid* liftingDown;
	Solenoid* triggerRightUp;
	Solenoid* triggerRightDown;
	Solenoid* triggerLeftUp;
	Solenoid* triggerLeftDown;
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	ChuteSubsystem();
	void InitDefaultCommand();
	
	void ChuteUp();
	void ChuteDown();
	void TriggerOn();
	void TriggerOff();
};

#endif
