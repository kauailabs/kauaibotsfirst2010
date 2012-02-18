#include "Gobbler.h"

Gobbler::Gobbler() {
	Requires(gobbler);
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
}

// Called just before this Command runs the first time
void Gobbler::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void Gobbler::Execute() {
	Joystick*pjoystick;
	pjoystick = oi->getJoystick();
	bool forwardgobbler=pjoystick->GetRawButton(10);
	if(forwardgobbler)
	{
		gobbler->Forward();
	}
	else
	{
		gobbler->Stop();
	}
	bool reversegobbler=pjoystick->GetRawButton(10);
	if(reversegobbler)
	{
		gobbler->Reverse();
	}
	else
	{
		gobbler->Stop();
	}
	//bool Joystick::GetButton(ButtonType button);
	//
}

// Make this return true when this Command no longer needs to run execute()
bool Gobbler::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void Gobbler::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Gobbler::Interrupted() {
	gobbler->Stop();
}
