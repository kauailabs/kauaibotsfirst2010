#include "ShootNow.h"

ShootNow::ShootNow() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(gobbler);
	Requires(chute);
}

// Called just before this Command runs the first time
void ShootNow::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void ShootNow::Execute() {
	chute->ChuteUp();
	chute->TriggerOn();
	gobbler->Forward();
	
}

// Make this return true when this Command no longer needs to run execute()
bool ShootNow::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void ShootNow::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ShootNow::Interrupted() {
}
