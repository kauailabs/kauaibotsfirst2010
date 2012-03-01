#include "DriveForward.h"
#include <math.h>
#include "JoystickConstants.h"
#include <Timer.h>


DriveForward::DriveForward() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	requires(drive);
}

// Called just before this Command runs the first time
void DriveForward::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void DriveForward::Execute() {
	Timer::Timer();  
	void Timer::Reset();  
	void Timer::Start();
	double Timer::Get(hello);
	while(hello != 5){
		x = 1;
		y = -1;
		z = 1;
		drive->DoMecanum(x,y,z);
		double Timer::Get(hello);
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
