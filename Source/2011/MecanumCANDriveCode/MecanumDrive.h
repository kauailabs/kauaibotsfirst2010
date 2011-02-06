#ifndef __MecanumDrive_h__
#define __MecanumDrive_h__
/*
 * MecanumDrive.h
 *
 *  Created on: Nov 22, 2009
 *      Author: slibert
 */

#include "WPILib.h"
#include "../CANJaguar/CANJaguar.h"
//#include "CustomGyro.h"
//#include "FilteredAccelerometer.h"

/*
 * Description:  Custom Robot Drive Class to extend the
 * WPI Library RobotDrive class in order to add the
 * ability to operate a mecanum omniwheel drive system,
 * and also to incorporate feedback from quadrature encoders.
 */
class MecanumDrive
{
protected:
	CANJaguar	m_frontLeftMotor;
	CANJaguar  m_frontRightMotor;
	CANJaguar  m_rearLeftMotor;
	CANJaguar  m_rearRightMotor;
	//CustomGyro	m_gyroscope;
	//FilteredAccelerometer m_accelerometerX;
	//FilteredAccelerometer m_accelerometerY;
	
public:
	MecanumDrive( UINT32 frontLeftMotorCANAddress,
			UINT32 frontRightMotorCANAddress,
			UINT32 rearLeftMotorCANAddress,
			UINT32 rearRightMotorCANAddress);
	virtual void DoMecanum( float vX, float vY, float vRot );
	void InitializeSensors();
	void RestartSensors();

	CANJaguar& 	FrontLeftMotor() { return m_frontLeftMotor; }
	CANJaguar& 	FrontRightMotor() { return m_frontRightMotor; }
	CANJaguar& 	RearLeftMotor() { return m_rearLeftMotor; }
	CANJaguar& 	RearRightMotor() { return m_rearRightMotor; }
	
	//CustomGyro&			Gyroscope() { return m_gyroscope; }
	//FilteredAccelerometer& 		AccelerometerX() { return m_accelerometerX; }
	//FilteredAccelerometer&		AccelerometerY() { return m_accelerometerY; }
	
	virtual ~MecanumDrive();
protected:
	void MecanumDriveFwdKinematics( float wheelSpeeds[4], float* pVelocities );
	void MecanumDriveInvKinematics( float velocities[3], float* pWheelSpeeds);
	void DoMecanumInternal( float vX, float vY, float vRot );
	
	double InputJoystickAdjust( double dJoystickIn );
	double InputJoystickAdjust2( double dJoystickIn );
	double InputJoystickAdjust3( double dJoystickIn );
};
#endif
