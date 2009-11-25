// MecanumDriveTester.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <math.h>

//////////////////////////////////////////////////////////
// Robot-specific measurements (in units of inches)
//
// These may change if the robot design changes
//////////////////////////////////////////////////////////

const float cWidth			= 20.5;					// Distance btwn left/right wheels
const float cLength			= 28.5;					// Distance btwn front/back wheels
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

void MecanumDriveFwdKinematics( float wheelSpeeds[4], float* pVelocities )
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

void MecanumDriveInvKinematics( float velocities[3], float* pWheelSpeeds)
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

// MecanumDrive()
// Each input is expected to range from -1 to 1
// Outputs are the four wheel speeds
// Each output is expected to range from -1 to 1

void MecanumDrive( char * strDescription, float vX, float vY, float vRot )
{
	printf("%s (%f,%f,%f)\n\n", strDescription, vX, vY, vRot );

	///////
	// Translational/Rotational Input Velocity Scaling:
	//
	// Four input mix cases (1/0-0, 0/1-0, 0/0-1 and .5/.5,1) all should end up driving the
	// motors at their maximum speed.
	//
	// However, total output of each motor cannot exceed 1.
	// Thus, the inputs are scaled to ensure the sum does not exceed 1.0.

	float excessRatio = (float)1.0 / ( abs(vX) + abs(vY) + abs(vRot) ); 
	if ( excessRatio < 1.0 )
	{
		vX		*= excessRatio;
		vY		*= excessRatio;
		vRot	*= excessRatio;

		printf("Input Velocities scaled to %f, %f, %f\n", vX, vY, vRot);
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
	printf("Output Velocities scaled to %f, %f, %f\n", vX, vY, vRot);

	float wheelSpeeds[4];
	float velocities[3] = { vX, vY, vRot };

	MecanumDriveInvKinematics( velocities, &wheelSpeeds[0] );

	printf("FL, FR:  %f, %f\n\n", wheelSpeeds[0], wheelSpeeds[1]);
	printf("RL, RR:  %f, %f\n\n", wheelSpeeds[2], wheelSpeeds[3]);

	// For testing purposes, transform the resulting wheel speeds through the matching
	// forward kinematics algorithm.  The resulting derived velocities should match
	// the scaled input velocities.

	float derivedVelocities[3];

	MecanumDriveFwdKinematics( wheelSpeeds, &derivedVelocities[0] );

	printf("vX:  %f, vY:  %f, vRot:  %f\n\n", derivedVelocities[VX], derivedVelocities[VY], derivedVelocities[VROT]*cRotK );
}

int _tmain(int argc, _TCHAR* argv[])
{
	// The Andy Mark Test...
	MecanumDrive("Full Forward", 0,1,0);
	MecanumDrive("Full Backward", 0,-1,0);
	MecanumDrive("Right Shift", 1,0,0);
	MecanumDrive("Left Shift", -1,0,0);
	MecanumDrive("CW", 0,0,1);
	MecanumDrive("CCW", 0,0,-1);

	// Additional tests found in the Doroftei, et al paper
	MecanumDrive("Forward-right", .5,.5,0);
	MecanumDrive("Forward-left", -.5,.5,0);
	MecanumDrive("Backward-right", .5,-.5,0);
	MecanumDrive("Backward-left", -.5,-.5,0);
	MecanumDrive("Full Foward, Right Full Spin",0,1,.5);
	MecanumDrive("Lateral Arc",-.5,0,.5);

	// Some more cases that test "mixes" and partial speeds
	MecanumDrive("Half Forward", 0,0.5,0);
	MecanumDrive("Half Backward", 0,-0.5,0);
	MecanumDrive("Half Forward, Left Slow Spin",0,.5,-0.5);
	MecanumDrive("Half Left", -0.5, 0, 0);
	MecanumDrive("9 O'clock full, half spin",.5,.5,.5);
	MecanumDrive("9 O'clock full, full spin",.5,.5,1);
}

