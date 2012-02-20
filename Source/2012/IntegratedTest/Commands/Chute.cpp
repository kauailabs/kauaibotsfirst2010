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
		zjoystick = oi->getShooterJoystick();
        bool ChuteUpTriggerOn = zjoystick->GetRawButton(1);
        //bool TriggerOn = zjoystick->GetRawButton(5);
        bool SteerLeft = zjoystick->GetX();
        bool SteerRight = zjoystick->GetX();
        
        if (ChuteUpTriggerOn)
        {
                chute->ChuteUp();
                chute->TriggerOn();
        }
        else
        {
                chute->ChuteDown();
                chute->TriggerOff();
        }
        if (SteerLeft > 0)
        {
                chute->SetSteeringAngle(chute->GetMinimumChuteAngle());
        }
        else if (SteerRight < 0)
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
