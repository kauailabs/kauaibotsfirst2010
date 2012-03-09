#include "WaitTime.h"

WaitTime::WaitTime(float runTime) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	runTime= m_runTime;
}

// Called just before this Command runs the first time
void WaitTime::Initialize() {
	SetTimeout(m_runTime);
}

// Called repeatedly when this Command is scheduled to run
void WaitTime::Execute() {
	
}

// Make this return true when this Command no longer needs to run execute()
bool WaitTime::IsFinished() {
	return IsTimedOut();
}

// Called once after isFinished returns true
void WaitTime::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void WaitTime::Interrupted() {
}
