#include "DriveSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/Drive.h"

#define DRIVE_FRONT_EDGEFINDER_CHANNEL 4
#define DRIVE_RIGHT_EDGEFINDER_CHANNEL 5
#define DRIVE_REAR_EDGEFINDER_CHANNEL 6
#define DRIVE_LEFT_EDGEFINDER_CHANNEL 7


DriveSubsystem::DriveSubsystem() : 
	PIDSubsystem("DriveSubsystem",0.024,0.0012,0.00),
	drive(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS,CANJaguar::kSpeed),
	yaw(DRIVE_YAW_GYRO_CHANNEL),
	pitch(DRIVE_PITCH_GRYO_CHANNEL),
	roll(DRIVE_ROLL_GYRO_CHANNEL),
	frontRanger(FRONT_RANGEFINDER_CHANNEL),
	rightRanger(RIGHT_RANGEFINDER_CHANNEL),
	rearRanger(REAR_RANGEFINDER_CHANNEL),
	leftRanger(LEFT_RANGEFINDER_CHANNEL),
	frontLeftEdgeFinder(DRIVE_FRONT_LEFT_EDGEFINDER_CHANNEL),
	frontRightEdgeFinder(DRIVE_FRONT_RIGHT_EDGEFINDER_CHANNEL),
	rearLeftEdgeFinder(DRIVE_REAR_LEFT_EDGEFINDER_CHANNEL),
	rearRightEdgeFinder(DRIVE_REAR_RIGHT_EDGEFINDER_CHANNEL)
{	
	InitializeSensors();
}
    
void DriveSubsystem::InitializeSensors()
{
    // Gyroscopes
	//         
	// Gyro is initialized, including calibration, in constructor
	//         
	// Initialize sensivity of the AD22305 Gyroscope         
	yaw.SetSensitivity(.007);  	
	pitch.SetSensitivity(.007);  	
	roll.SetSensitivity(.007);  	
}

void DriveSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new Drive());
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

void DriveSubsystem::DoMecanum( float vX, float vY, float vRot )
{
	drive.DoMecanum( vX, vY, vRot );
}
CANJaguar::ControlMode DriveSubsystem::getMode()
{
	return drive.GetMode();
}

void DriveSubsystem::setMode( CANJaguar::ControlMode newMode )
{
	drive.SetMode( newMode );
}

void DriveSubsystem::GetEulerAngles( double& yawAngle, double& pitchAngle, double& rollAngle)
{
	yawAngle = yaw.GetAngle();
	pitchAngle = pitch.GetAngle();
	rollAngle = roll.GetAngle();
}

void DriveSubsystem::GetRangesInches( double& frontRange, double& rightRange, double& rearRange, double& leftRange )
{
	frontRange =	frontRanger.GetRangeInches();
	rightRange =	rightRanger.GetRangeInches();
	rearRange  =	rearRanger.GetRangeInches();
	leftRange  =	leftRanger.GetRangeInches();
}

void DriveSubsystem::GetEdges( bool &frontLeft, bool& frontRight, bool& rearRight, bool& rearLeft)
{
	frontLeft	= (frontLeftEdgeFinder.Get() != 0);
	frontRight	= (frontRightEdgeFinder.Get() != 0);
	rearLeft	= (rearLeftEdgeFinder.Get() != 0);
	rearRight	= (rearRightEdgeFinder.Get() != 0);
}

double DriveSubsystem::ReturnPIDInput()
{
	return yaw.GetAngle();
}

void DriveSubsystem::UsePIDOutput(double)
{
	// Do nothing for now.
	// However, when in auto-rotation mode, update
	// the next rotation amount with this value.
}


