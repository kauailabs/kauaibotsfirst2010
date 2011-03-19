#ifndef __constants_h__
#define __constants_h__

//////////////////////////////////////////////////////////
// Robot-specific measurements (in units of inches)
//////////////////////////////////////////////////////////

const float cWidth			= 24.0;					// Distance btwn left/right wheels
const float cLength			= 28.0;					// Distance btwn front/back wheels
const float wheelDiameter	= 8.0;					// Per AndyMark Specs
const float wheelRadius		= wheelDiameter / 2;

/////////////////////////////////////////////////////////////////////////////////////
// Movement Modifiers
/////////////////////////////////////////////////////////////////////////////////////
#define ROTATE_DIRECTION -1
#define DRIVE_DIRECTION -1
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
//Physical locations of connectors on the robot
/////////////////////////////////////////////////////////////////////////////////////
#define LEFT_REAR_CAN_ADDRESS 4
#define RIGHT_REAR_CAN_ADDRESS 5
#define RIGHT_FRONT_CAN_ADDRESS 3
#define LEFT_FRONT_CAN_ADDRESS 2
#define ELEVATOR_1_CAN_ADDRESS 7
#define ELEVATOR_2_CAN_ADDRESS 9
#define SHOULDER_CAN_ADDRESS 8
#define HAND_CAN_ADDRESS 6
#define LOCK_1_CHANNEL 3
#define LOCK_2_CHANNEL 4
#define FIRE_1_CHANNEL 1
#define FIRE_2_CHANNEL 2
#define PRESSURE_SWITCH 14
#define COMPRESSOR1 1
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
//Joystick Assignments
/////////////////////////////////////////////////////////////////////////////////////
#define SHOULDER_BUTTON_1 4
#define SHOULDER_BUTTON_2 2
#define HAND_BUTTON_1 5
#define HAND_BUTTON_2 3
#define DEPLOY_BUTTON 9
#define UNDEPLOY_BUTTON 6
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Joystick Modifiers
// Result(when outside of the dead zone) = Mult(Adjust*Original^Pow + (1-Adjust)Original)
/////////////////////////////////////////////////////////////////////////////////////
// Forward Backwards
const double JoystickAdjust = 0; 
const double JoystickPow = 3; 
const double JoystickMult = .6;
const double JoystickDead = 0; 
// Side to Side
const double JoystickAdjust2 = 0; 
const double JoystickPow2 = 3; 
const double JoystickMult2 = .4; 
const double JoystickDead2 = 0; 
// Rotation
const double JoystickAdjust3 = 0;
const double JoystickPow3 = 3;
const double JoystickMult3 = .1;
const double JoystickDead3 = 0;
// Elevator Dead Zone
const double JoystickDead4 = 0.20; 
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Mecanum Constants
/////////////////////////////////////////////////////////////////////////////////////
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
// The AndyMark documentation indicates an alternative configuration, where each wheel's
// Axis points to the center of the robot.  This alternative configuration modifies the
// Matrices below by swapping the front and rear wheels.
//////////////////////////////////////////////////////////

const int WHEEL_FRONTLEFT	= 2;
const int WHEEL_FRONTRIGHT	= 3;
const int WHEEL_REARLEFT	= 4;
const int WHEEL_REARRIGHT	= 5;

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
/////////////////////////////////////////////////////////////////////////////////////


#endif

