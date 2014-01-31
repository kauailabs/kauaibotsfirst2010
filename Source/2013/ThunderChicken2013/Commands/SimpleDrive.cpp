#include "SimpleDrive.h"
#include "WPILib.h"
#include "../OI.h"
#include "Joystick.h"
SimpleDrive::SimpleDrive() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::swerveDriveSystem);
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

// Called just before this Command runs the first time
void SimpleDrive::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void SimpleDrive::Execute() {  
	Joystick* pstick = Robot::oi->getdriver_joystick();
	double twist = pstick->GetTwist();
	double y = pstick->GetY();
	Robot::swerveDriveSystem->DoSimpleDrive(y,twist);
}

// Make this return true when this Command no longer needs to run execute()
bool SimpleDrive::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void SimpleDrive::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SimpleDrive::Interrupted() {

}