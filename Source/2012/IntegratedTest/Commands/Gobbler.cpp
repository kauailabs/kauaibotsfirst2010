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
	Joystick*yjoystick;
	pjoystick = oi->getJoystick();
	yjoystick = oi->getShooterJoystick();
	bool forwardgobbler=pjoystick->GetRawButton(3);
	bool forwardshootgobbler=yjoystick->GetRawButton(3);
	bool reverseshootgobbler=yjoystick->GetRawButton(2);
	if(forwardgobbler or forwardshootgobbler)
	{
		gobbler->Forward();
	}
	else if(reverseshootgobbler)
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
