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
			accelerometerChannelY ), m_turnController(.015,.001,0,&m_gyroscope,this)
{
	m_pDashboardDataFormat = pDashboardDataFormat;
	m_pKicker = pKicker;
	m_pHorizontalServo = pHorizontalServo;
	m_pVerticalServo = pVerticalServo;
	
	// Initialize Auto-rotation 
	SetAutoRotationMode(false);
	m_turnController.SetInputRange(-360.0, 360.0);
	m_turnController.SetOutputRange(-1, 1);     // TODO:  Review this
	m_turnController.SetTolerance(.25 / 720.0 * 100);
	m_bAutoRotateTargetSet = false;
}

void AutoRotationMecanumDrive::SetAutoRotationMode( bool bEnable )
{
	if ( bEnable != m_bAutoRotationMode )
	{
		m_bAutoRotationMode = bEnable;
		SafeSetAutoRotateAmount(0);	// Always reset rotation amount	
		m_bAutoRotateTargetSet = false;
		bEnable ? m_turnController.Enable() : m_turnController.Disable();
	}
}

bool AutoRotationMecanumDrive::GetAutoRotationMode()
{
	return m_bAutoRotationMode;
}

void AutoRotationMecanumDrive::DoMecanum( float vX, float vY, float vRot, bool bScaleInputs )
{
	if ( bScaleInputs )
	{
		vX = InputJoystickAdjust(vX);
		vY = InputJoystickAdjust(vY);
		vRot = InputJoystickAdjust2(vRot *.8);	
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
			m_turnController.Disable();
		}
		printf("AutoRotate Value:  %f\n",(double)vRot);
	}
	
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

const float cRampUpTimeInSeconds = 0.1;

AutoRotationMecanumDrive::WaitType AutoRotationMecanumDrive::AutonomousDrive( float vX, float vY, float vRot, WaitType wait, float waitPeriodInSeconds, Watchdog& watchdog  )
{
	bool bDone = false;
	WaitType returnVal = Time;
	
	DriverStation *ds = DriverStation::GetInstance();
	
	// Save State
	bool bLastAutoRotateMode = GetAutoRotationMode();

	// Enable AutoRotation if TillOnTarget, otherwise disable it.
	SetAutoRotationMode( ( wait == TillOnTarget ) );

	double currTime = GetTime();
	double quitTime = currTime + waitPeriodInSeconds;
	
	double rampUpTime = currTime + cRampUpTimeInSeconds;

	while ( ds->IsAutonomous() && !bDone && (currTime < quitTime) )
	{
		float modifiedVX = vX;
		float modifiedVY = vY;
		float modifiedVRot = vRot;
		
		float rampUpTimeRemaining = rampUpTime - currTime;
		if ( rampUpTimeRemaining > 0 )
		{
			float rampPercentage = rampUpTimeRemaining / rampUpTime;

			modifiedVX *= rampPercentage;
			modifiedVY *= rampPercentage;
			modifiedVRot *= rampPercentage;
		}
		
		watchdog.Feed();
		DoMecanum( modifiedVX, modifiedVY, modifiedVRot, false );

		if ( (wait == TillOnTarget) && m_turnController.OnTarget() )
		{
			returnVal = wait;
			bDone = true;
		}
		else if ( ( wait == TillBallDetected ) && m_pKicker->IsBallDetected() )
		{
			returnVal = wait;
			bDone = true;
		}
		Wait(0.02); 			// TODO:  Tune this.
		currTime = GetTime();
	}
		
	// Restore state
	SetAutoRotationMode( bLastAutoRotateMode );
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
	AxisCamera& camera = AxisCamera::GetInstance();
	if ( camera.IsFreshImage() ) 
	{
		// get the gyro heading that goes with this image
		double gyroAngle = Gyroscope().PIDGet();
		
		// get the camera image
		ColorImage *image = camera.GetImage();
		
		int imageHeight = image->GetHeight();
		int imageWidth = image->GetWidth();

		// find FRC targets in the image
		vector<Target> targets = Target::FindCircularTargets(image);
		delete image;  // Be sure to delete the image!
		
		if (targets.size() == 0 || targets[0].m_score < MINIMUM_SCORE)
		{
			// no targets found. Make sure the first one in the list is 0,0
			// since the dashboard program annotates the first target in green
			// and the others in magenta. With no qualified targets, they'll all
			// be magenta.
			Target nullTarget;
			nullTarget.m_majorRadius = 0.0;
			nullTarget.m_minorRadius = 0.0;
			nullTarget.m_score = 0.0;
			if (targets.size() == 0)
				targets.push_back(nullTarget);
			else
				targets.insert(targets.begin(), nullTarget);
		
			m_pDashboardDataFormat->sendVisionData(0.0, gyroAngle, 0.0, 0.0, targets);
		}
		else // Targets detected
		{
			KauaibotsTarget smartTargeter(targets[0],imageHeight,imageWidth,m_pHorizontalServo,m_pVerticalServo);
			//printf("Distance (feet):  %f\n",smartTargeter.GetDistanceToTargetFeet2());				//printf("Distance (inches):  %f\n",smartTargeter.GetDistanceToTargetInches());

			// set the new PID heading setpoint to the first target in the list
			double horizontalAngle = smartTargeter.GetRobotHorizontalAngle();
			double setPoint = gyroAngle + horizontalAngle;
			printf("Target Angle (degrees):  %f    SetPoint:  %f  Gyro:  %f  OnTarget:  %i  Error:  %f\n,",horizontalAngle, setPoint,gyroAngle, m_turnController.OnTarget(), (double)m_turnController.GetError());

			if ( !m_bAutoRotateTargetSet )
			{
				m_bAutoRotateTargetSet = true;
				m_turnController.SetSetpoint(setPoint);
			}
				
			// send dashboard data for target tracking
			m_pDashboardDataFormat->sendVisionData(0.0, gyroAngle, setPoint, targets[0].m_xPos / targets[0].m_xMax, targets);
		}
	}		
}

