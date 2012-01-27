#include "TestGobbler.h"

TestGobbler::TestGobbler() 
{
	Requires(Gobbler);
	
}

// Called just before this Command runs the first time
void TestGobbler::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void TestGobbler::Execute() 
{
	bool bGobblerForward = oi->getDriverStation()->GetDigitalIn(1);
	bool bGobblerReverse = oi->getDriverStation()->GetDigitalIn(2);
	if(bGobblerForward)
	{  
		Gobbler->Forward();

	}
	else if(bGobblerReverse)
	{
		Gobbler->Reverse();
	}
	else 
	{
		Gobbler->Stop();
	}
}

// Make this return true when this Command no longer needs to run execute()
bool TestGobbler::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void TestGobbler::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TestGobbler::Interrupted() 
{
	Gobbler->Stop();
}
