#include "DriveForward.h"
#include <math.h>
#include "JoystickConstants.h"
#include <Timer.h>


DriveForward::DriveForward() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
}

// Called just before this Command runs the first time
void DriveForward::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void DriveForward::Execute() {
	double x=1;
	double y=-1;
	double z=1;
	drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
	if(leftRange > 10){
		drive->DoMecanum(x,y,z);
		drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
	}
	else if (leftRange < 10){
		double x=.5;
		double y=-.5;
		double z=.5;
		drive->DoMecanum(x,y,z);
		drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
	}
}

// Make this return true when this Command no longer needs to run execute()
bool DriveForward::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveForward::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveForward::Interrupted() {
}
