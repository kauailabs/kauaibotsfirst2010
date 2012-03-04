#include "DriveForward.h"
#include <math.h>
#include "JoystickConstants.h"
#include <Timer.h>


DriveForward::DriveForward() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
	double timeoutSeconds;
	SetTimeout(timeoutSeconds);
}

// Called just before this Command runs the first time
void DriveForward::Initialize() {

}

// Called repeatedly when this Command is scheduled to run
void DriveForward::Execute() {
	double x;
	double y;
	double rot;
	double frontRange;
	double rightRange;
	double rearRange;
	double leftRange;
	drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
	if(leftRange > 10){
		x=1;
		y=1;
		rot=1;
		drive->DoMecanum(x,y,rot);
		drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
	}
	else{
		double x=.5;
		double y=-.5;
		double rot=.5;
		drive->DoMecanum(x,y,rot);
		drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
	}
}
// Make this return true when this Command no longer needs to run execute()
bool DriveForward::IsFinished() {
	double frontRange;
	double rightRange;
	double rearRange;
	double leftRange;
	drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
	if (leftRange < 6){
		return true;
	}
	return IsTimedOut();
}

// Called once after isFinished returns true
void DriveForward::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveForward::Interrupted() {
}
