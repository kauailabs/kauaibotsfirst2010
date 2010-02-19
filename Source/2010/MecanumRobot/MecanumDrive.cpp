/*
 * MecanumDrive.cpp
 *
 *  Created on: Nov 22, 2009
 *      Author: slibert
 */

#include "MecanumDrive.h"
#include <math.h>

//////////////////////////////////////////////////////////
// Robot-specific measurements (in units of inches)
//
// These may change if the robot design changes
//////////////////////////////////////////////////////////

const float cWidth			= 22.25;					// Distance btwn left/right wheels
const float cLength			= 26.0;					// Distance btwn front/back wheels
const float wheelDiameter	= 6.0;					// Per AndyMark Specs
const float wheelRadius		= wheelDiameter / 2;

//////////////////////////////////////////////////////////
// Proportional translation vs. Rotation
//
// For the same motor speed, the distance of translation and distance of rotation
// are not the same, due to the proportions of the wheel radius, and the 
// distance between front/back and left/right wheels.
//////////////////////////////////////////////////////////

const float cRotK = ((cWidth + cLength)/2) / wheelRadius;		// Rotational Coefficient

//////////////////////////////////////////////////////////
// Forward and Inverse Kinematics Matrices
//
// As document on p. 523 of Omnidirectional Mobile Robot - Design and Implementation
// (Ioan Doroftei, Victor Grosu and Veaceslav Spinu).
//
// A nominal value of 1 is used for translational motion.  
//
// NOTE:  These matrices assume the roller direction of each wheel points to the
// center of the robot.
//
// The AndyMark documentation indicates an alternative configuration, where each wheel's
// Axis points to the center of the robot.  This alternative configuration modifies the
// Matrices below by swapping the front and rear wheels.
//////////////////////////////////////////////////////////

const int WHEEL_FRONTLEFT	= 0;
const int WHEEL_FRONTRIGHT	= 1;
const int WHEEL_REARLEFT	= 2;
const int WHEEL_REARRIGHT	= 3;

const int VX	= 0;
const int VY	= 1;
const int VROT	= 2;

#define ROLLERS_ROTATE_TO_ROBOT_CENTER 1

const float invMatrix[4][3] = 
{ 
#ifdef ROLLERS_ROTATE_TO_ROBOT_CENTER
	{  -1, 1,  cRotK },
	{   1, 1, -cRotK },
	{   1, 1,  cRotK },
	{  -1, 1, -cRotK },
#else
	{   1, 1,  cRotK },
	{  -1, 1, -cRotK },
	{  -1, 1,  cRotK },
	{   1, 1, -cRotK },
#endif
};

MecanumDrive::MecanumDrive( UINT32 frontLeftMotorChannel,
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
	UINT32 accelerometerChannelY )
	: m_frontLeftMotor( frontLeftMotorChannel)
	, m_frontRightMotor( frontRightMotorChannel)
	, m_rearLeftMotor( rearLeftMotorChannel)
	, m_rearRightMotor( rearRightMotorChannel)
	, m_frontLeftEncoder(4,frontLeftEncoderChannelA,4,frontLeftEncoderChannelB)
	, m_frontRightEncoder(4,frontRightEncoderChannelA,4,frontRightEncoderChannelB)
	, m_rearLeftEncoder(4,rearLeftEncoderChannelA,4,rearLeftEncoderChannelB)
	, m_rearRightEncoder(4,rearRightEncoderChannelA,4,rearRightEncoderChannelB)
	, m_gyroscope(1,gyroChannel)
	, m_accelerometerX(1,accelerometerChannelX)
	, m_accelerometerY(1,accelerometerChannelY)
{
	InitializeSensors();
	RestartSensors();
}

void MecanumDrive::InitializeSensors()
{
	// Encoders
	m_frontLeftEncoder.SetMinRate(0.5);
	m_frontRightEncoder.SetMinRate(0.5);
	m_rearLeftEncoder.SetMinRate(0.5);
	m_rearRightEncoder.SetMinRate(0.5);
	
	// Gyroscope
	//
	// Gyro is initialized, including calibration, in constructor
	//
	// Initialize sensivity of the AD22305 Gyroscope
	m_gyroscope.SetSensitivity(.007);
	// Accelerometers
	//
	// Accelerometers are initialized, including calibration, in constructor
	//
	// Intialize Zero voltage and sensitivity of the ADXL335
	// Accelerometer chip based upon chip spec sheet.
	m_accelerometerX.SetZero(1.5);
	m_accelerometerX.SetSensitivity(.3);
	m_accelerometerY.SetZero(1.5);
	m_accelerometerY.SetSensitivity(.3);
}

void MecanumDrive::RestartSensors()
{
	m_frontLeftEncoder.Reset();
	m_frontRightEncoder.Reset();
	m_rearLeftEncoder.Reset();
	m_rearRightEncoder.Reset();
	
	m_frontLeftEncoder.Start();
	m_frontRightEncoder.Start();
	m_rearLeftEncoder.Start();
	m_rearRightEncoder.Start();
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

const double cDriveJoystickExponent = 2.0;

double MecanumDrive::InputJoystickAdjust( double dJoystickIn )
{
	double dJoystickOut = 0.0;
	if ( dJoystickIn > 0 )
		dJoystickOut = pow(dJoystickIn, cDriveJoystickExponent);
	else
		dJoystickOut = -1 * pow(dJoystickIn, cDriveJoystickExponent);

	return dJoystickOut;
}

const double cDriveJoystickExponent2 = 3.0;

double MecanumDrive::InputJoystickAdjust2( double dJoystickIn )
{
	double dJoystickOut = 0.0;
	if ( dJoystickIn > 0 )
		dJoystickOut = pow(dJoystickIn, cDriveJoystickExponent2);
	else
		dJoystickOut = -1 * pow(dJoystickIn, cDriveJoystickExponent2);

	return dJoystickOut;
}



// MecanumDrive()
// Each input is expected to range from -1 to 1
// Outputs are the four wheel speeds
// Each output is expected to range from -1 to 1
void MecanumDrive::DoMecanum( float vX, float vY, float vRot )
{
	//char *strDescription = "DoMecanum()";
	//printf("%s (%f,%f,%f)\n\n", strDescription, vX, vY, vRot );

	// Introduce exponential ramp on joystick input.
	
	vX = InputJoystickAdjust(vX);
	vY = InputJoystickAdjust(vY);
	vRot = InputJoystickAdjust2(vRot);
			
	///////
	// Translational/Rotational Input Velocity Scaling:
	//
	// Four input mix cases (1/0-0, 0/1-0, 0/0-1 and .5/.5,1) all should end up driving the
	// motors at their maximum speed.
	//
	// However, total output of each motor cannot exceed 1.
	// Thus, the inputs are scaled to ensure the sum does not exceed 1.0.

	float excessRatio = (float)1.0 / ( fabs(vX) + fabs(vY) + fabs(vRot) ); 
	if ( excessRatio < 1.0 )
	{
		vX		*= excessRatio;
		vY		*= excessRatio;
		vRot	*= excessRatio;

		//printf("Input Velocities scaled to %f, %f, %f\n", vX, vY, vRot);
	}

	///////
	// Rotational Output Velocity Scaling
	//
	// Outputs should not exceed the minimum/maximum output mother speed ranges.
	// Therefore, scale the rotational input value so that the maximum rotational
	// output velocity is equivalent to the maximum translational output velocity (1.0).
	//
	// This scaling is derived from the fact that the distance traveled for rotational motion 
	// is less than the distance traveled for translational motion.
	//
	// NOTE:  Rotational and Translational distances would be equivalent if the wheel base  
	// was equal to the wheel radius.

	vRot *= (1/cRotK);
	//printf("Output Velocities scaled to %f, %f, %f\n", vX, vY, vRot);

	float wheelSpeeds[4];
	float velocities[3] = { vX, vY, vRot };

	MecanumDriveInvKinematics( velocities, &wheelSpeeds[0] );

	//printf("FL, FR:  %f, %f\n\n", wheelSpeeds[0], wheelSpeeds[1]);
	//printf("RL, RR:  %f, %f\n\n", wheelSpeeds[2], wheelSpeeds[3]);

	m_frontLeftMotor.Set(wheelSpeeds[0] * -1 ); // Inverted polarity of left-side wheels
	m_frontRightMotor.Set(wheelSpeeds[1]);
	m_rearLeftMotor.Set(wheelSpeeds[2] * -1 );  // Inverted polarity of left-side wheels
	m_rearRightMotor.Set(wheelSpeeds[3]);
		
	// For testing purposes, transform the resulting wheel speeds through the matching
	// forward kinematics algorithm.  The resulting derived velocities should match
	// the scaled input velocities.

	//float derivedVelocities[3];

	//MecanumDriveFwdKinematics( wheelSpeeds, &derivedVelocities[0] );

	//printf("vX:  %f, vY:  %f, vRot:  %f\n\n", derivedVelocities[VX], derivedVelocities[VY], derivedVelocities[VROT]*cRotK );
}

/*
 * Default destructor
 */
MecanumDrive::~MecanumDrive()
{	
}
