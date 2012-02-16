#include "Tilter.h"

Tilter::Tilter() {
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
        Requires(tilter);
}

// Called just before this Command runs the first time
void Tilter::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void Tilter::Execute() {
		Joystick*zjoystick;
		zjoystick = oi->getJoystick();
		bool tilterdown=zjoystick->GetRawButton(6);
		if(tilterdown)
		{
			tilter->Down();
		}
		else
		{
			tilter->Up();
		}
}

// Make this return true when this Command no longer needs to run execute()
bool Tilter::IsFinished() {
        return false;
}

// Called once after isFinished returns true
void Tilter::End() {
        tilter->Up();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Tilter::Interrupted() {
}
