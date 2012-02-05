#include "Hopper.h"

Hopper::Hopper() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(hopper);
}

// Called just before this Command runs the first time
void Hopper::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void Hopper::Execute() {
	if (oi->getDriverStation()->GetDigitalIn(1))
	{
		hopper->On();
	}
	else 
	{
		hopper->Off();
	}
}

// Make this return true when this Command no longer needs to run execute()
bool Hopper::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void Hopper::End() {
	hopper->Off();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Hopper::Interrupted() {
}
