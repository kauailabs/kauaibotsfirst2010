  #include "GobblerSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/TestGobbler.h"

GobblerSubsystem::GobblerSubsystem() : Subsystem("GobblerSubsystem")
{
	firstMotor=new Victor(FIRST_GOBBLER_MOTOR_PWM_CHANNEL);
	secondMotor=new Victor(SECOND_GOBBLER_MOTOR_PWM_CHANNEL);
}
 
GobblerSubsystem::~GobblerSubsystem()
{
	delete firstMotor;
	delete secondMotor;
	firstMotor=0; 
	secondMotor=0;
}
void GobblerSubsystem::InitDefaultCommand() 
{
	// Set the default command for a subsystem here.
	SetDefaultCommand(new TestGobbler());
}

void GobblerSubsystem::Forward()
{
	firstMotor->Set(1);
	secondMotor->Set(1);
}
void GobblerSubsystem::Stop()
{
	firstMotor->Set(0);
	secondMotor->Set(0);
}

void GobblerSubsystem::Reverse()
{
	firstMotor->Set(-1);
	secondMotor->Set(-1);
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

