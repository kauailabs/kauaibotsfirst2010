/*
 * AutoRotationMecanumDrive.cpp
 *
 *  Created on: March 4, 2010
 *      Author: slibert
 */

#include "AutoRotationMecanumDrive.h"
#include "Vision/AxisCamera.h"
#include "Vision/HSLImage.h"
#include "Vision/ColorImage.h"
#include "Vision/ImageBase.h"
#include <math.h>
#include "KauaibotsTarget.h"

#define MINIMUM_SCORE 0.01

static SEM_ID cAutoRotateSemaphore = semBCreate (SEM_Q_PRIORITY, SEM_FULL);

AutoRotationMecanumDrive::AutoRotationMecanumDrive( UINT32 frontLeftMotorChannel,
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
	Servo *pVerticalServo)
	: MecanumDrive(
			frontLeftMotorChannel,
			frontRightMotorChannel,
			rearLeftMotorChannel,
			rearRightMotorChannel,
			frontLeftEncoderChannelA,
			frontLeftEncoderChannelB,
			frontRightEncoderChannelA,
			frontRightEncoderChannelB,
			rearLeftEncoderChannelA,
			rearLeftEncoderChannelB,
			rearRightEncoderChannelA,
			rearRightEncoderChannelB,
			gyroChannel,
			accelerometerChannelX,
//			accelerometerChannelY ), m_turnController(.015,.001,0.001,&m_gyroscope,this)
//			accelerometerChannelY ), m_turnController(.025,.001,0.1,&m_gyroscope,this)
//			accelerometerChannelY ), m_turnController(.025,0,0,&m_gyroscope,this)
			accelerometerChannelY ), m_turnController(.025,.005,0.2,&m_gyroscope,this), m_TargetDetector(&m_gyroscope, pHorizontalServo, pVerticalServo)
{
	m_pDashboardDataFormat = pDashboardDataFormat;
	m_pKicker = pKicker;
	
	// Initialize Auto-rotation 
	SetAutoRotationMode(false,true);
	m_turnController.SetInputRange(-360.0, 360.0);
	m_turnController.SetOutputRange(-.5, .5);     // TODO:  Review this
	m_turnController.SetTolerance(.25 / 720.0 * 100);
	m_bAutoRotateTargetSet = false;
	m_bAutoRotateToTarget = true;
}

void AutoRotationMecanumDrive::SetAutoRotationMode( bool bEnable, bool bRotateToTarget )
{
	if ( bEnable != m_bAutoRotationMode )
	{
		m_bAutoRotationMode = bEnable;
		m_bAutoRotateToTarget = bRotateToTarget;
		SafeSetAutoRotateAmount(0);	// Always reset rotation amount	
		m_bAutoRotateTargetSet = false;
		bEnable ? m_turnController.Enable() : m_turnController.Disable();
	}
}

bool AutoRotationMecanumDrive::GetAutoRotationMode( bool &bRotateToTarget )
{
	bRotateToTarget = m_bAutoRotateToTarget;
	return m_bAutoRotationMode;
}

const double cMinMotorRotationalOutput = .25;

void AutoRotationMecanumDrive::DoMecanum( float vX, float vY, float vRot, bool bScaleInputs )
{
	if ( bScaleInputs )
	{
		vX = InputJoystickAdjust(vX);
		vY = InputJoystickAdjust(vY);
		vRot = InputJoystickAdjust2(vRot *.9);	
	}
		
	// Update the Vision Dashboard.
	// Note that this will force the angle to the target
	// to be computed, which is used for auto-rotation.
	// NOTE:  Do this after the update to the motors.
	
	UpdateDashboard();

	// If in auto-rotation mode, get the rotation value from the PID controller
	// based upon the delta between the target position and the current gyro 
	// position.
	
	if ( m_bAutoRotationMode )
	{
		vRot = SafeGetAutoRotateAmount();
		vRot *= -1;	// Invert value before sending to the output.
		if ( m_turnController.OnTarget() )
		{
			//m_turnController.Disable();
		}
		/*
		// If requested rotation value is within the dead zone, adjust it. 
		if ( vRot < 0 && (vRot > (cMinMotorRotationalOutput * -1)) )
		{
			vRot = cMinMotorRotationalOutput * -1;
		}
		else if ( vRot > 0 && (vRot < cMinMotorRotationalOutput) )
		{
			vRot = cMinMotorRotationalOutput;
		}
		*/
		//printf("AutoRotate Value:  %f\n",(double)vRot);
	
	}
	
	//printf("Rotation value:  %f\n",(double)vRot);
	
	MecanumDrive::DoMecanumInternal( vX, vY, vRot );
}

// Performs an autonomous-mode command.  Allows control of the
// linear and rotation velocity (explicit values).  Supports
// Autorotation (if wait == TillOnTarget).
//
// This method executes until the "wait" period has expired.  The
// wait period may be one of the following:
//
// Time:  The driving commands will be continually sent to the
//        drive system until the time period has expired.
// TillOnTarget:  The driving commands will be continually sent
//        to the drive system until either the robot has rotated 
//        to the target, or the time period has expired.
// TillBallDetected:  The driving commands will be continually sent
//        to the drive system until the "ball detected" signal
//        has been received, or the time period has expired.
//
// NOTE:  When this method returns, the drive system will be stopped.
//
// Parameters:
//
// vX:  X Axis Velocity (-1 to 1)
// vY:  Y Axis Velocity (-1 to 1)
// vRot: Rotational Velocity (-1 to 1) [Ignored if wait = TillOnTarget]
// WaitType:  Specifies how long the drive commands will continue.
// waitPeriodInSeconds:  Timeout Period.  If WaitType is not 
//   equal to "Time", this is the maximum time that the "Till..."
//   event will be waited for.
//
//
// Return Value:  returns the reason for the wait.
////////////////////////////////////////////////////////////////////

const double cRampUpTimeInSeconds = 0.5;

AutoRotationMecanumDrive::WaitType AutoRotationMecanumDrive::AutonomousDrive( float vX, float vY, float vRot, WaitType wait, float waitPeriodInSeconds, Watchdog& watchdog  )
{
	bool bDone = false;
	WaitType returnVal = Time;
	
	DriverStation *ds = DriverStation::GetInstance();
	
	// Save State
	bool bLastRotateToTarget = true;
	bool bLastAutoRotateMode = GetAutoRotationMode( bLastRotateToTarget );

	// Enable AutoRotation if TillOnTarget, otherwise disable it.
	SetAutoRotationMode( ( wait == TillOnTarget ) || ( wait == TillAtZeroDegrees ), (wait == TillOnTarget) );

	double currTime = GetTime();
	double quitTime = currTime + waitPeriodInSeconds;
	
	double rampUpTime = currTime + cRampUpTimeInSeconds;

	while ( ds->IsAutonomous() && !bDone && (currTime < quitTime) )
	{
		float modifiedVX = vX;
		float modifiedVY = vY;
		float modifiedVRot = vRot;
		
		double rampUpTimeRemaining = rampUpTime - currTime;
		if ( rampUpTimeRemaining > 0 )
		{
			double rampPercentage = 1 - (rampUpTimeRemaining / cRampUpTimeInSeconds);

			printf("Ramping up to %f percent.\n", rampPercentage);
			
			modifiedVX *= rampPercentage;
			modifiedVY *= rampPercentage;
			modifiedVRot *= rampPercentage;
		}
		
		watchdog.Feed();
		DoMecanum( modifiedVX, modifiedVY, modifiedVRot, false );

		if ( ((wait == TillOnTarget) || (wait == TillAtZeroDegrees )) && m_turnController.OnTarget() )
		{
			returnVal = wait;
			bDone = true;
		}
		else if ( ( wait == TillBallDetected ) && m_pKicker->IsBallDetected() )
		{
			returnVal = wait;
			bDone = true;
		}
		Wait(0.01); 			// TODO:  Tune this.
		currTime = GetTime();
	}
		
	// Restore state
	SetAutoRotationMode( bLastAutoRotateMode, bLastRotateToTarget );
	return returnVal;
}

/*
 * Default destructor
 */
AutoRotationMecanumDrive::~AutoRotationMecanumDrive()
{	
	m_pDashboardDataFormat = 0;
	m_pKicker = 0;
}

void AutoRotationMecanumDrive::PIDWrite(float output)
{
//	if (output > 0 && output < .1)
//		output = .1;
//	if (output < 0 && output > -.1)
//		output = -.1;
	SafeSetAutoRotateAmount( output );
}

float AutoRotationMecanumDrive::SafeGetAutoRotateAmount()
{
	Synchronized sync(cAutoRotateSemaphore);
	float autoRotateAmount = m_pendingAutoRotateAmount;
	return autoRotateAmount;
}

void AutoRotationMecanumDrive::SafeSetAutoRotateAmount( float rot )
{
	Synchronized sync(cAutoRotateSemaphore);
	m_pendingAutoRotateAmount = rot;
}

/**
 * Send data to the dashboard
 */
void AutoRotationMecanumDrive::UpdateDashboard()
{
	double gyroAngle;
	double targetAngle;	
	vector<Target> targets;
	
	m_TargetDetector.GetDetectedTargets( targets, gyroAngle, targetAngle);

	if (targets.size() == 0 || targets[0].m_score < MINIMUM_SCORE)
	{
		// No targets found
		m_pDashboardDataFormat->sendVisionData(0.0, gyroAngle, 0.0, 0.0, targets);		
	}
	else
	{
		double setPoint = gyroAngle + targetAngle;		
		//printf("Target Angle (degrees):  %f   OnTarget:  %i  Error:  %f\n",targetAngle, m_turnController.OnTarget(), (double)m_turnController.GetError());
		
		// If auto-rotation was requested, 
		if ( !m_bAutoRotateTargetSet )
		{
			m_bAutoRotateTargetSet = true;
			if ( m_bAutoRotateToTarget )
			{
				m_turnController.SetSetpoint(setPoint);
			}
			else // Rotate to zero degrees
			{
				m_turnController.SetSetpoint(0);					
			}
		}
			
		// send dashboard data for target tracking
		m_pDashboardDataFormat->sendVisionData(0.0, gyroAngle, setPoint, targets[0].m_xPos / targets[0].m_xMax, targets);		
	}		
}

