#include "TestTilter.h"

TestTilter::TestTilter() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(tilter);
}

// Called just before this Command runs the first time
void TestTilter::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void TestTilter::Execute() {
	if (oi->getDriverStation()->GetDigitalIn(1))
	{
		tilter->Down();
	}
	else 
	{
		tilter->Up();
	}
}

// Make this return true when this Command no longer needs to run execute()
bool TestTilter::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void TestTilter::End() {
	tilter->Up();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TestTilter::Interrupted() {
}
