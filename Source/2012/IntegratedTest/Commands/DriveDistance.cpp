#include "DriveDistance.h"

const double safetyRange = 6;

DriveDistance::DriveDistance(bool x,double distanceInches,double RPMs,bool ifRange) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(drive);
	double seconds = drive->GetSecondsToTravelLinearDistance(x,distanceInches,RPMs);
	m_ifRange = ifRange;
	m_distanceInches = distanceInches;
	m_cycles = int(seconds / 0.02);
	m_currentCycle = 0;
	m_x = x;
	m_RPMs = RPMs;
}

// Called just before this Command runs the first time
void DriveDistance::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void DriveDistance::Execute() {
	double frontRange;
	double rightRange;
	double rearRange;
	double leftRange;
	m_currentCycle++;
	if(m_currentCycle <= m_cycles){
		 double maxRPMs = drive->GetSpeedModeMaxRPMs();
		 double driveSpeed = m_RPMs/maxRPMs;
		 if (m_distanceInches < 0){
			 driveSpeed*=-1;
		 }
		 if (m_ifRange){
			 drive->GetRangesInches(frontRange,rightRange,rearRange,leftRange);
			 double rangeICareAbout;
			 if(driveSpeed<0 and m_x){
				 	 rangeICareAbout = leftRange;
			 }
			 else if(driveSpeed<=0 and !m_x){
			 	 rangeICareAbout = rightRange;
			 }
			 else if(driveSpeed>0 and m_x){
			 	 rangeICareAbout = rearRange;
			 }
			 else if(driveSpeed>=0 and !m_x){
			 	 rangeICareAbout = frontRange;
			 }
			 if (rangeICareAbout <= safetyRange){
				 double ratio = double(1)/(rangeICareAbout/safetyRange);
				 driveSpeed*=ratio;
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
	bool done = m_currentCycle > m_cycles;
	return done;
}

// Called once after isFinished returns true
void DriveDistance::End() {
	drive->DoMecanum(0,0,0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveDistance::Interrupted() {
}
