#include "Drive.h"
#include <math.h>

Drive::Drive() 
{
	Requires(drive); // Exclusive use of the Drive Subsystem is required.
}

void Drive::Initialize() 
{
}

void Drive::Execute() 
{
	// Update PercentVbus vs. Speed Mode setting
	bool bUsePercentVbusMode = oi->getDriverStation()->GetDigitalIn(1);
	if ( drive->getMode() == CANJaguar::kPercentVbus )
	{
		if ( !bUsePercentVbusMode ) drive->setMode( CANJaguar::kSpeed );
	}
	else
	{
		if ( bUsePercentVbusMode ) drive->setMode( CANJaguar::kPercentVbus );
	}

	// Drive with current joystick values.
	Joystick *pJoystick = oi->getJoystick();
	double twist = pJoystick->GetTwist();
	if (fabs(twist) < DEADZONE)
		twist = 0;
				
	double y = pJoystick->GetY();
	if (fabs(y) < DEADZONE)
		y = 0;
							
	double x = pJoystick->GetX();
	if (fabs(x) < DEADZONE)
		x = 0;
	
	drive->DoMecanum(x,y,twist * -1);
}

bool Drive::IsFinished() 
{
	return false;
}

void Drive::End() 
{
}

void Drive::Interrupted() 
{
	drive->DoMecanum(0,0,0);	// Stop the drive
}
