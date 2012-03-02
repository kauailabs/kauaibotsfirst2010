#include "BridgeTilterDown.h"

BridgeTilterDown::BridgeTilterDown() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(tilter);
}

// Called just before this Command runs the first time
void BridgeTilterDown::Initialize() {

}
// Called repeatedly when this Command is scheduled to run
void BridgeTilterDown::Execute() {  
			tilter->Down();
}

// Make this return true when this Command no longer needs to run execute()
bool BridgeTilterDown::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void BridgeTilterDown::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void BridgeTilterDown::Interrupted() {
}
