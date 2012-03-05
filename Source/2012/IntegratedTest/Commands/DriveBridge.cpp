#include "DriveBridge.h"
#include <math.h>
#include "JoystickConstants.h"

DriveBridge::DriveBridge() 
{
}

void DriveBridge::Initialize() 
{
	// TODO: Get existing control mode and drive gear, and save them.
	// Force Speed Mode to be used
	drive->SetControlMode( CANJaguar::kSpeed );
	drive->SetDriveGear( DriveSubsystem::kLowGear );
	drive->ResetGyros();
	m_bAutoBalanceMode = false;
	// NOTE:  Be sure to restore the old "drive gear" when done.
}

void DriveBridge::Execute() 
{
	if ( true )
	{
		double yawAngle, pitchAngle;
		double accelX, accelY, accelZ;
		
		double offBalancePitchGyroAngleThresholdDegrees = 10; //??
		double onBalancePitchGyroAngleThresholdDegrees  = 5;  //??
		double onBalancePitchAccelAngleTresholdDegrees = 5; // ??
		
		drive->GetEulerAnglesDegrees(yawAngle, pitchAngle);
		drive->GetAccelerationGs( accelX, accelY, accelZ );
		
		// Determine whether assisted balance mode is in effect;
		if ( !m_bAutoBalanceMode && ( pitchAngle > offBalancePitchGyroAngleThresholdDegrees ) )
		{
			m_bAutoBalanceMode = true;
		}
		else if ( m_bAutoBalanceMode && ( pitchAngle <= (-onBalancePitchGyroAngleThresholdDegrees)))
		{
			double pitchAccelAngleDegrees = (accelX * 90);
			if ( pitchAccelAngleDegrees <= onBalancePitchAccelAngleTresholdDegrees )
			{
				m_bAutoBalanceMode = false;
			}
		}
		if ( m_bAutoBalanceMode )
		{
			float driveRate = sin(pitchAngle) * -1;
			drive->DoMecanum( 0, driveRate, 0 );
		}
		else
		{
			Drive::Execute();
		}
	}
	else // e-brake mode
	{
		// Read the current wheel "velocity ratios"
		float x, y, rot;
		drive->GetWheelVelocitiesRatio( x, y, rot );
		drive->DoMecanum( 0, y*-1, 0 );
	}
 }

bool DriveBridge::IsFinished() 
{
        return false;
}

void DriveBridge::End() 
{
	// TODO: restore the saved control mode and drive gear.

}

void DriveBridge::Interrupted() 
{
        drive->DoMecanum(0,0,0);        // Stop the drive
}

