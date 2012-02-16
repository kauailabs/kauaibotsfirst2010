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
		Joystick*zjoystick;
		zjoystick = oi->getJoystick();
		bool ChuteUp=zjoystick->GetRawButton(4);
		bool TriggerOn=zjoystick->GetRawButton(5);
		bool SteerLeft=zjoystick->GetRawButton(2);
		bool SteerRight=zjoystick->GetRawButton(3);
        if (ChuteUp)
        {
                chute->ChuteUp();
        }
        else
        {
                chute->ChuteDown();
        }
        if (TriggerOn)
        {
                chute->TriggerOn();
        }
        else
        {
                chute->TriggerOff();
        }
        if (SteerLeft)
        {
                chute->SetSteeringAngle(STEERING_LOWER_BOUND_DEGREES);
        }
        else if (SteerRight)
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
