#include "DriveSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/DriveWithJoystick.h"

DriveSubsystem::DriveSubsystem() : 
	Subsystem("DriveSubsystem"),
	drive(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS,CANJaguar::kSpeed)
{	
}
    
void DriveSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new DriveWithJoystick());
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
