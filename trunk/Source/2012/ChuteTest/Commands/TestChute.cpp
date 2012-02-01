 #include "TestChute.h"

TestChute::TestChute() 
{
	Requires(chute);
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
}

// Called just before this Command runs the first time
void TestChute::Initialize() 
{

	
	
}

// Called repeatedly when this Command is scheduled to run
void TestChute::Execute() 
{
	bool bChuteUp = !oi->getDriverStation()->GetDigitalIn(1);
	bool bTriggerOn = !oi->getDriverStation()->GetDigitalIn(2);
	if (bChuteUp)
	{
		chute->ChuteUp();
	}
	else
	{
		chute->ChuteDown();
	}
	if (bTriggerOn)
	{
		chute->TriggerOn();
	}
	else
	{
		chute->TriggerOff();
	}
	
}


// Make this return true when this Command no longer needs to run execute()
bool TestChute::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void TestChute::End() 
{
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TestChute::Interrupted() 
{

}
