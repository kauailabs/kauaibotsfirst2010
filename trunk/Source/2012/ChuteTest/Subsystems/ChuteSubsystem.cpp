#include "ChuteSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/TestChute.h"

ChuteSubsystem::ChuteSubsystem() : Subsystem("ChuteSubsystem") 
{
	steering=new Servo(STEERING_SERVO_CHANNEL);
	liftingUp=new Solenoid(1, LIFTING_UP_SOLENOID_CHANNEL);
	liftingDown=new Solenoid(1, LIFTING_DOWN_SOLENOID_CHANNEL);
	triggerRightUp=new Solenoid(1, TRIGGER_RIGHT_UP_SOLENOID_CHANNEL);
	triggerRightDown-new Solenoid(1, TRIGGER_RIGHT_DOWN_SOLENOID_CHANNEL);
	triggerLeftDown=new Solenoid(1, TRIGGER_LEFT_DOWN_SOLENOID_CHANNEL);
	triggerLeftUp=new Solenoid(1, TRIGGER_LEFT_UP_SOLENOID_CHANNEL);
}

void ChuteSubsystem::InitDefaultCommand() 
{
	SetDefaultCommand(new TestChute());
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void ChuteSubsystem::ChuteUp()
{
	liftingUp->Set(true);
	liftingDown->Set(false);
}
void ChuteSubsystem::ChuteDown()
{
	liftingUp->Set(false);
	liftingDown->Set(true);

}
void ChuteSubsystem::TriggerOn()
{

	triggerLeftUp->Set(false);
	triggerLeftDown->Set(true);
	triggerRightUp->Set(false);
	triggerRightDown->Set(true);
}

void ChuteSubsystem::TriggerOff()
{
	triggerLeftUp->Set(true);
	triggerLeftDown->Set(false);
	triggerRightUp->Set(true);
	triggerRightDown->Set(false);
}
	







// Put methods for controlling this subsystem
// here. Call these from Commands.

