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
        bool bChuteUp = zjoystick->GetRawButton(4);
        bool bTriggerOn = zjoystick->GetRawButton(5);
        bool bSteerLeft = zjoystick->GetRawButton(2);
        bool bSteerRight = zjoystick->GetRawButton(3);
        
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
                chute->SetSteeringAngle(chute->GetMinimumChuteAngle());
        }
        else if (bSteerRight)
        {
                chute->SetSteeringAngle(chute->GetMaximumChuteAngle());
        }
        else
        {
                chute->SetSteeringAngle(STEERING_CENTER_POSITION_DEGREES);
        }
        chute->UpdateStatus();
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
