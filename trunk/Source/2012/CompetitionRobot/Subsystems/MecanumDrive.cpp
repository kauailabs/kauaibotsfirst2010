/*
 * MecanumDrive.cpp
 *
 *  Created on: Nov 22, 2009
 *      Author: slibert
 *  Modified on: March 18, 2011
 * 		Re-Author: smassaro
 */

#include "MecanumDrive.h"
#include <math.h>


MecanumDrive::MecanumDrive( UINT32 frontLeftMotorCANAddress,
	UINT32 frontRightMotorCANAddress,
	UINT32 rearLeftMotorCANAddress,
	UINT32 rearRightMotorCANAddress,
	CANJaguar::ControlMode controlMode,
	int maxSpeedModeRPMs )
	: m_frontLeftMotor( frontLeftMotorCANAddress, CANJaguar::kPercentVbus)
	, m_frontRightMotor( frontRightMotorCANAddress, CANJaguar::kPercentVbus)
	, m_rearLeftMotor( rearLeftMotorCANAddress, CANJaguar::kPercentVbus)
	, m_rearRightMotor( rearRightMotorCANAddress, CANJaguar::kPercentVbus)
	, m_currControlMode( controlMode )
{
	m_frontLeftMotor.GetPowerCycled();
	m_frontRightMotor.GetPowerCycled();
	m_rearLeftMotor.GetPowerCycled();
	m_rearRightMotor.GetPowerCycled();
	
	m_maxSpeedModeRPMs = maxSpeedModeRPMs;
	
	SetMode( controlMode );
	
	// Init Motor Safety
	m_safetyHelper = new MotorSafetyHelper(this);
	m_safetyHelper->SetSafetyEnabled(false);	
}

void MecanumDrive::SetMode( CANJaguar::ControlMode controlMode )
{
	m_currControlMode = controlMode;

	if ( m_currControlMode == CANJaguar::kSpeed )
	{
		m_maxOutputSpeed = m_maxSpeedModeRPMs;
	}
	else // kPercentVbus
	{
		m_maxOutputSpeed = 1;
	}
	
	InitMotor(m_frontLeftMotor);
	InitMotor(m_frontRightMotor);
	InitMotor(m_rearLeftMotor);
	InitMotor(m_rearRightMotor);	
}

CANJaguar::ControlMode MecanumDrive::GetMode()
{
	return m_currControlMode;
}

void MecanumDrive::InitMotor( CANJaguar& motor )
{
	motor.ChangeControlMode( m_currControlMode );
	if ( m_currControlMode == CANJaguar::kSpeed )
	{
		motor.ConfigEncoderCodesPerRev(360);
		motor.ConfigMaxOutputVoltage(12.0);
		motor.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
		motor.SetPID(.7,.004,0);
		motor.SetSpeedReference(CANJaguar::kSpeedRef_QuadEncoder);
	}
	motor.EnableControl();
}

void MecanumDrive::CheckForRestartedMotor( CANJaguar& motor, const char * strDescription )
{
	if ( m_currControlMode != CANJaguar::kSpeed )   // kSpeed is the default
	{
		if ( motor.GetPowerCycled() )
		{
			Wait(0.10); // Wait 100 ms 
			InitMotor( motor );
			char error[256];
			sprintf(error, "\n\n>>>>%s %s", strDescription, "Jaguar Power Cycled - re-initializing");
			printf(error);
			setErrorData(error, strlen(error), 100);		
		}
	}
}

void MecanumDrive::MecanumDriveFwdKinematics( float wheelSpeeds[4], float* pVelocities )
{
	for ( int i = 0; i < 3; i++ )
	{
		pVelocities[i] = 0;
		for ( int wheel = 0; wheel < 4; wheel ++ )
		{
			pVelocities[i] += wheelSpeeds[wheel] * (1 / invMatrix[wheel][i]);
		}
		pVelocities[i] *= ((float)1/4);
	}
}

void MecanumDrive::MecanumDriveInvKinematics( float velocities[3], float* pWheelSpeeds)
{
	for ( int wheel = 0; wheel < 4; wheel ++ )
	{
		pWheelSpeeds[wheel] = 0;
		for ( int i = 0; i < 3; i++ )
		{
			pWheelSpeeds[wheel] += velocities[i] * invMatrix[wheel][i];
		}
	}
}

void MecanumDrive::DoMecanum( float vX, float vY, float vRot )
{
	//vRot = ROTATE_DIRECTION * vRot;
	
	float excessRatio = (float)1.0 / ( fabs(vX) + fabs(vY) + fabs(vRot) ); 
	if ( excessRatio < 1.0 )
	{
		vX		*= excessRatio;
		vY		*= excessRatio;
		vRot	*= excessRatio;
	}

	vRot *= (1/cRotK);

	float wheelSpeeds[4];
	float velocities[3] = { vX, vY, vRot };

	MecanumDriveInvKinematics( velocities, &wheelSpeeds[0] );
	
	UINT8 syncGroup = 0x80;

	CheckForRestartedMotor( m_frontLeftMotor, "Front Left" );
	CheckForRestartedMotor( m_frontRightMotor, "Front Right" );
	CheckForRestartedMotor( m_rearLeftMotor, "Rear Left" );
	CheckForRestartedMotor( m_rearRightMotor, "Rear Right" );
	
	m_frontLeftMotor.Set(m_maxOutputSpeed * wheelSpeeds[0] * -1 * DRIVE_DIRECTION, syncGroup );
	m_frontRightMotor.Set(m_maxOutputSpeed * wheelSpeeds[1] * DRIVE_DIRECTION, syncGroup);
	m_rearLeftMotor.Set(m_maxOutputSpeed * wheelSpeeds[2] * -1 * DRIVE_DIRECTION, syncGroup);  
	m_rearRightMotor.Set(m_maxOutputSpeed * wheelSpeeds[3] * DRIVE_DIRECTION, syncGroup);

	CANJaguar::UpdateSyncGroup(syncGroup);	
	
	m_safetyHelper->Feed();	
}

MecanumDrive::~MecanumDrive()
{	
	delete m_safetyHelper;
}

// MotorSafety Implementation : the following implement the MotorSafety interface.
// This interface was introduced in 2011 and is designed to replace the older
// "UserWatchdog"; the purpose of the interface is to ensure that the motors
// are disabled if the software stops attempting to control the motors

void MecanumDrive::SetExpiration(float timeout)
{
	m_safetyHelper->SetExpiration(timeout);
}

float MecanumDrive::GetExpiration()
{
	return m_safetyHelper->GetExpiration();
}

bool MecanumDrive::IsAlive()
{
	return m_safetyHelper->IsAlive();
}

bool MecanumDrive::IsSafetyEnabled()
{
	return m_safetyHelper->IsSafetyEnabled();
}

void MecanumDrive::SetSafetyEnabled(bool enabled)
{
	m_safetyHelper->SetSafetyEnabled(enabled);
}

void MecanumDrive::GetDescription(char *desc)
{
	sprintf(desc, "MecanumDrive Class");
}

void MecanumDrive::StopMotor()
{
	m_frontLeftMotor.Disable();
	m_frontRightMotor.Disable();
	m_rearLeftMotor.Disable();
	m_rearRightMotor.Disable();
}

