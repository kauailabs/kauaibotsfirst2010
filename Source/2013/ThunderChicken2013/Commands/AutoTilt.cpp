// RobotBuilder Version: 0.0.2
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.


#include "AutoTilt.h"
#include "../Robotmap.h"
#include "../Robot.h"
#include "Timer.h"

AutoTilt::AutoTilt(double angle) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(Robot::magazine);
	tilt=angle;
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

// Called just before this Command runs the first time
void AutoTilt::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void AutoTilt::Execute() {
	Robot::tilter->UsePIDOutput(tilt);
}

// Make this return true when this Command no longer needs to run execute()
bool AutoTilt::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void AutoTilt::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void AutoTilt::Interrupted() {

}
