#ifndef __MecanumDrive_h__
#define __MecanumDrive_h__
/*
 * MecanumDrive.h
 *
 *  Created on: Nov 22, 2009
 *      Author: slibert
 *  Modified on: March 19,2011
 * 		Re-Author: smassaro
 */

#include "WPILib.h"
#include "constants.h"

class MecanumDrive : public MotorSafety
{
protected:
	
	CANJaguar			m_frontLeftMotor;
	CANJaguar  			m_frontRightMotor;
	CANJaguar 			m_rearLeftMotor;
	CANJaguar  			m_rearRightMotor;
	MotorSafetyHelper *	m_safetyHelper;
	
public:
	MecanumDrive( UINT32 frontLeftMotorCANAddress,
			UINT32 frontRightMotorCANAddress,
			UINT32 rearLeftMotorCANAddress,
			UINT32 rearRightMotorCANAddress
			);
	virtual void DoMecanum( float vX, float vY, float vRot );

	CANJaguar& 	FrontLeftMotor() { return m_frontLeftMotor; }
	CANJaguar& 	FrontRightMotor() { return m_frontRightMotor; }
	CANJaguar& 	RearLeftMotor() { return m_rearLeftMotor; }
	CANJaguar& 	RearRightMotor() { return m_rearRightMotor; }
	
	virtual ~MecanumDrive();

	// Motor Safety Implementation
	
	void SetExpiration(float timeout);
	float GetExpiration();
	bool IsAlive();
	void StopMotor();
	void SetSafetyEnabled(bool enabled);
	bool IsSafetyEnabled();

protected:
	void MecanumDriveFwdKinematics( float wheelSpeeds[4], float* pVelocities );
	void MecanumDriveInvKinematics( float velocities[3], float* pWheelSpeeds);
	
	double MecanumDrive::InputJoystickAdjust( double dJoystickIn, double dAdjustment, double dExponent, double dMultiplier , double dDead);
};
#endif
