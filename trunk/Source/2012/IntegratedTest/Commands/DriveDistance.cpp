#include "DriveDistance.h"

const double safetyRange = 6;

DriveDistance::DriveDistance(bool x,double distanceInches,double RPMs,bool ifRange) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
	
	m_ifRange 			= ifRange;
	m_distanceInches 	= distanceInches;
	m_x 				= x;
	m_RPMs 				= RPMs;

	// Calculate the amount of time to drive
	double seconds 		= drive->GetSecondsToTravelLinearDistance( m_x, m_distanceInches, m_RPMs );
	SetTimeout(seconds);	
}

// Called just before this Command runs the first time
void DriveDistance::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void DriveDistance::Execute() {
	
	if ( !IsTimedOut() )
	{
		 double maxRPMs = drive->GetSpeedModeMaxRPMs();
		 double driveSpeed = m_RPMs/maxRPMs;
		 if (m_distanceInches < 0){
			 driveSpeed*=-1;
		 }
		 if (m_ifRange){
			 double frontRange;
			 double rightRange;
			 double rearRange;
			 double leftRange;

			 drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
			 double rangeICareAbout;
			 
			 if ( m_x )
			 {
				 if ( driveSpeed < 0 )  rangeICareAbout = leftRange;
				 else					rangeICareAbout = rightRange;	
			 }
			 else
			 {
				 if ( driveSpeed < 0 )  rangeICareAbout = rearRange;
				 else					rangeICareAbout = frontRange;	
			 }
			 
			 if (rangeICareAbout <= safetyRange){
				 double ratio = (rangeICareAbout/safetyRange);
				 driveSpeed *= ratio;
			 }
		 }
		 if(m_x){
			 drive->DoMecanum(driveSpeed,0,0);
		 }
		 else{
			 drive->DoMecanum(0,driveSpeed,0);
		 }
	}	
}

// Make this return true when this Command no longer needs to run execute()
bool DriveDistance::IsFinished() {
	return IsTimedOut();
}

// Called once after isFinished returns true
void DriveDistance::End() {
	drive->DoMecanum(0,0,0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveDistance::Interrupted() {
	drive->DoMecanum(0,0,0);
}
