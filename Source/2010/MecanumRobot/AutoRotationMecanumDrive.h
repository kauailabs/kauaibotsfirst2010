#ifndef __AutoRotationMecanumDrive_h__
#define __AutoRotationMecanumDrive_h__
/*
 * AutoRotationMecanumDrive.h
 *
 *  Created on: March 4, 2010
 *      Author: slibert
 */

#include "MecanumDrive.h"
#include "DashboardDataFormat.h"
#include "Kicker.h"
#include "TargetDetector.h"

/*
 * Description:  Custom Robot Drive Class to extend the
 * MecanumDrive class in order to add the
 * ability to auto-orient to the currently-detected target, or the
 * last detected target based upon current gyro position,
 * and also to support Autonomous Mode features.
 */
class AutoRotationMecanumDrive : public MecanumDrive, PIDOutput
{
protected:

	DashboardDataFormat *m_pDashboardDataFormat;
	Kicker *m_pKicker;
	bool m_bAutoRotationMode;
	bool m_bAutoRotateToTarget; // false = rotate to zero degrees
	bool m_bAutoRotateTargetSet;
	PIDController m_turnController;
	Servo *m_pHorizontalServo;
	Servo *m_pVerticalServo;
	float m_pendingAutoRotateAmount;
	TargetDetector m_TargetDetector;
	
public:
	AutoRotationMecanumDrive( UINT32 frontLeftMotorChannel,
			UINT32 frontRightMotorChannel,
			UINT32 rearLeftMotorChannel,
			UINT32 rearRightMotorChannel,
			UINT32 frontLeftEncoderChannelA,
			UINT32 frontLeftEncoderChannelB,
			UINT32 frontRightEncoderChannelA,
			UINT32 frontRightEncoderChannelB,
			UINT32 rearLeftEncoderChannelA,
			UINT32 rearLeftEncoderChannelB,
			UINT32 rearRightEncoderChannelA,
			UINT32 rearRightEncoderChannelB,
			UINT32 gyroChannel,
			UINT32 accelerometerChannelX,
			UINT32 accelerometerChannelY,
			DashboardDataFormat *pDashboardDataFormat,
			Kicker *pKicker,
			Servo *pHorizontalServo,
			Servo *pVerticalServo);
	
	virtual ~AutoRotationMecanumDrive();

	void SetAutoRotationMode( bool bEnable, bool bRotateToTarget /* false = rotate to zero degrees */ );
	bool GetAutoRotationMode( bool& bRotateToTarget );

	double GetAutoRotationError( bool& bOnTarget );	
	
	virtual void DoMecanum( float vX, float vY, float vRot, bool bScaleInputs = true );	
	
	enum WaitType { Time, TillOnTarget, TillBallDetected, TillAtZeroDegrees };
	
	// Performs an autonomous-mode command.
	//
	// vX:  Value to set motor to (-1 to 1)
	// vY:  Value to set motor to (-1 to 1)
	// vRotDegrees: Amount to rotate in degrees (-180 to 180)
	// WaitType:  How long to run the autonomous command
	// waitPeriodInSeconds:  Timeout Period.  If WaitType is not 
	//   equal to "Time", this is the maximum time that the "Till..."
	//   event will be waited for.
	//
	// This method returns the reason for the wait.
	
	WaitType AutonomousDrive( float vX, float vY, float vRot, WaitType wait, float waitPeriodInSeconds, Watchdog& watchdog );

protected:
	
	void UpdateDashboard();	

	// Thread-safe accesors for auto-rotation amount
	float SafeGetAutoRotateAmount();
	void SafeSetAutoRotateAmount( float rot );
			
	// PIDOutput Overrides
	void PIDWrite(float output);

};
#endif // __AutoRotationMecanumDrive_h__
