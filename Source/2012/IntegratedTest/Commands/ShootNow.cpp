#include "ShootNow.h"

ShootNow::ShootNow(bool bShoot, float timeRun) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(gobbler);
	Requires(chute);
	m_bShoot = bShoot;
	m_timeRun = timeRun;
}

// Called just before this Command runs the first time
void ShootNow::Initialize() {
	SetTimeout(m_timeRun);
}

// Called repeatedly when this Command is scheduled to run
void ShootNow::Execute() {
	if ( m_bShoot )
	{
		gobbler->Forward();
		chute->ChuteUp();
		chute->TriggerOn();
	}
	else
	{
		gobbler->Stop();
		chute->ChuteDown();
		chute->TriggerOff();		
	}
}

// Make this return true when this Command no longer needs to run execute()
bool ShootNow::IsFinished() {
	return IsTimedOut();
	
}

// Called once after isFinished returns true
void ShootNow::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ShootNow::Interrupted() {
}
