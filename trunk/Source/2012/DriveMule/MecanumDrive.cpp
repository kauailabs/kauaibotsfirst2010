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
	UINT32 rearRightMotorCANAddress)
	: m_frontLeftMotor( frontLeftMotorCANAddress, CANJaguar::kSpeed)
	, m_frontRightMotor( frontRightMotorCANAddress, CANJaguar::kSpeed)
	, m_rearLeftMotor( rearLeftMotorCANAddress, CANJaguar::kSpeed)
	, m_rearRightMotor( rearRightMotorCANAddress, CANJaguar::kSpeed)
{
	m_frontLeftMotor.ConfigEncoderCodesPerRev(360);
	m_frontLeftMotor.ConfigMaxOutputVoltage(12.0);
	m_frontLeftMotor.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	m_frontRightMotor.ConfigEncoderCodesPerRev(360);
	m_frontRightMotor.ConfigMaxOutputVoltage(12.0);
	m_frontRightMotor.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	m_rearLeftMotor.ConfigEncoderCodesPerRev(360);
	m_rearLeftMotor.ConfigMaxOutputVoltage(12.0);
	m_rearLeftMotor.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	m_rearRightMotor.ConfigEncoderCodesPerRev(360);
	m_rearRightMotor.ConfigMaxOutputVoltage(12.0);
	m_rearRightMotor.ConfigNeutralMode(CANJaguar::kNeutralMode_Brake);
	m_frontLeftMotor.SetPID(1,0,0);
	m_frontRightMotor.SetPID(1,0,0);
	m_rearLeftMotor.SetPID(1,0,0);
	m_rearRightMotor.SetPID(1,0,0);
	m_frontLeftMotor.EnableControl();
	m_frontRightMotor.EnableControl();
	m_rearLeftMotor.EnableControl();
	m_rearRightMotor.EnableControl();

	// Init Motor Safety
	m_safetyHelper = new MotorSafetyHelper(this);
	m_safetyHelper->SetSafetyEnabled(true);
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

double MecanumDrive::InputJoystickAdjust( double dJoystickIn, double dAdjustment, double dExponent, double dMultiplier , double dDead)
{

	double dJoystickOut = 0.0;
	if ((dJoystickIn > dDead) || (dJoystickIn < (-1*dDead)))
	{
		dJoystickOut = ((dAdjustment * pow(dJoystickIn, dExponent)) + ((1 - dAdjustment) * dJoystickIn)) * dMultiplier;	
	}
	return dJoystickOut;
}

void MecanumDrive::DoMecanum( float vX, float vY, float vRot )
{

	vY = InputJoystickAdjust(vY, JoystickAdjust, JoystickPow, JoystickMult, JoystickDead);	
	vX = InputJoystickAdjust(vX, JoystickAdjust2, JoystickPow2, JoystickMult2, JoystickDead2);
	vRot = ROTATE_DIRECTION * InputJoystickAdjust(vRot, JoystickAdjust3, JoystickPow3, JoystickMult3, JoystickDead3);
	
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

	m_frontLeftMotor.Set(600 * wheelSpeeds[0] * -1 * DRIVE_DIRECTION, syncGroup );
	m_frontRightMotor.Set(600 * wheelSpeeds[1] * DRIVE_DIRECTION, syncGroup);
	m_rearLeftMotor.Set(600 * wheelSpeeds[2] * -1 * DRIVE_DIRECTION, syncGroup);  
	m_rearRightMotor.Set(600 * wheelSpeeds[3] * DRIVE_DIRECTION, syncGroup);

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

void MecanumDrive::StopMotor()
{
	m_frontLeftMotor.Disable();
	m_frontRightMotor.Disable();
	m_rearLeftMotor.Disable();
	m_rearRightMotor.Disable();
}

