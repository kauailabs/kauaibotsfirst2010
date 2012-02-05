 #include "Chute.h"

Chute::Chute() 
{
	Requires(chute);
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
}

// Called just before this Command runs the first time
void Chute::Initialize() 
{

	
	
}

// Called repeatedly when this Command is scheduled to run
void Chute::Execute() 
{
	bool bChuteUp = !oi->getDriverStation()->GetDigitalIn(1);
	bool bTriggerOn = !oi->getDriverStation()->GetDigitalIn(2);
	bool bSteerLeft = !oi->getDriverStation()->GetDigitalIn(3);
	bool bSteerRight = !oi->getDriverStation()->GetDigitalIn(4);
	
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
	if (bSteerLeft)
	{
		chute->SetSteeringAngle(STEERING_LOWER_BOUND_DEGREES);
	}
	else if (bSteerRight)
	{
		chute->SetSteeringAngle(STEERING_UPPER_BOUND_DEGREES);
	}
	else
	{
		chute->SetSteeringAngle(STEERING_CENTER_POSITION_DEGREES);
	}
}


// Make this return true when this Command no longer needs to run execute()
bool Chute::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void Chute::End() 
{
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Chute::Interrupted() 
{

}
