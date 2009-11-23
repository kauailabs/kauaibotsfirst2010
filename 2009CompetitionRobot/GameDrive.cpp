/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "GameDrive.h"

#include "GenericHID.h"
#include "Joystick.h"
#include "Victor.h"
#include "Utility.h"
#include "WPIStatus.h"
#include "SpeedMotor.h"
#include <math.h>


GameDrive::GameDrive()
{
	m_frontLeftMotor = new Jaguar(1);
	m_frontRightMotor = new Jaguar(2);
	m_rearLeftMotor = new Jaguar(3);
	m_rearRightMotor = new Jaguar(4);
	m_frontSteerMotor = new Speedmotor(5);
	m_rearSteerMotor = new Speedmotor(6);
	m_frontPot = new Potentiometer(1);
	m_rearPot = new Potentiometer(2);
	m_PIDFront = new PIDController(1,1,1);
	m_PIDRear = new PIDController(1,1,1);
	m_sensitivity = 0.5;
	m_invertedRear = -1;
	m_PIDFront->SetInput(m_frontPot, m_frontPot->GetMin(), m_frontPot->GetMax());
	m_PIDFront->SetOutput(m_frontSteerMotor);
	m_PIDRear->SetInput(m_rearPot, m_rearPot->GetMin(), m_rearPot->GetMax());
	m_PIDRear->SetOutput(m_rearSteerMotor);
	m_deleteAll = true;
	m_DriveWheelEncoder = new Encoder(kDriveWheelEncoderForward,kDriveWheelEncoderReverse);
	m_FreeWheelEncoder = new Encoder(kFreeWheelEncoderForward,kFreeWheelEncoderReverse);
	m_DriveWheelEncoder->Start();
	m_FreeWheelEncoder->Start();

}


GameDrive::GameDrive(SpeedController *frontLeftMotor, SpeedController *rearLeftMotor,
					SpeedController *frontRightMotor, SpeedController *rearRightMotor,
					UINT32 frontSteerChannel, UINT32 rearSteerChannel, Potentiometer *frontPot,
					Potentiometer *rearPot, float sensitivity)
{
	if (frontLeftMotor == NULL || rearLeftMotor == NULL || frontRightMotor == NULL || rearRightMotor == NULL)
	{
		wpi_fatal(NullParameter);
		m_frontLeftMotor = m_rearLeftMotor = m_frontRightMotor = m_rearRightMotor = NULL;
		return;
	}
	m_frontLeftMotor = frontLeftMotor;
	m_rearLeftMotor = rearLeftMotor;
	m_frontRightMotor = frontRightMotor;
	m_rearRightMotor = rearRightMotor;
	m_frontSteerMotor = new Speedmotor(frontSteerChannel);
	m_rearSteerMotor = new Speedmotor(rearSteerChannel);
	m_PIDFront = new PIDController(1,1,1);
	m_PIDRear = new PIDController(1,1,1);
	m_sensitivity = sensitivity;
	for (INT32 i=0; i < kMaxNumberOfMotors; i++)
	{
		m_invertedMotors[i] = 1;
	}
	m_invertedRear = -1;
	m_PIDFront->SetInput( frontPot, frontPot->GetMin(), frontPot->GetMax());
	m_PIDRear->SetInput( rearPot, rearPot->GetMin(), rearPot->GetMax());
	m_PIDFront->SetOutput( m_frontSteerMotor );
	m_PIDRear->SetOutput( m_rearSteerMotor );
	m_deleteSpeedControllers = false;
	m_deleteSteerControllers = true;
}




/**
 * GameDrive destructor.
 * Deletes all the motor objects regardless of how they were allocated. Don't continue to use
 * any passed-in PWM objects after this class destructor is run.
 **/
GameDrive::~GameDrive(void)
{
	if (m_deleteAll)
	{
		m_deleteSpeedControllers = true;
		m_deleteSteerControllers = true;
		delete m_frontPot;
		delete m_rearPot;
	}
	if (m_deleteSpeedControllers)
	{
		delete m_frontLeftMotor;
		delete m_rearLeftMotor;
		delete m_frontRightMotor;
		delete m_rearRightMotor;
	}
	if (m_deleteSteerControllers)
	{
		delete m_frontSteerMotor;
		delete m_rearSteerMotor;
		delete m_PIDFront;
		delete m_PIDRear;
	}
}


/**
 * Drive the motors at "speed" and "curve".
 * 
 * The speed and curve are -1.0 to +1.0 values where 0.0 represents stopped and
 * not turning. The algorithm for adding in the direction attempts to provide a constant
 * turn radius for differing speeds.
 * 
 * This function sill most likely be used in an autonomous routine.
 * 
 * @param speed The forward component of the speed to send to the motors.
 * @param curve The rate of turn, constant for different forward speeds.
 */
void GameDrive::Drive(float speed, float curve)
{
	float leftSpeed, rightSpeed;

	if (curve < 0)
	{
		float value = log(-curve);
		float ratio = (value - m_sensitivity)/(value + m_sensitivity);
		if (ratio == 0) ratio =.0000000001;
		leftSpeed = speed / ratio;
		rightSpeed = speed;
	}
	else if (curve > 0)
	{
		float value = log(curve);
		float ratio = (value - m_sensitivity)/(value + m_sensitivity);
		if (ratio == 0) ratio =.0000000001;
		leftSpeed = speed;	
		rightSpeed = speed / ratio;
	}
	else
	{
		leftSpeed = speed;
		rightSpeed = speed;
	}
	SetLeftRightMotorSpeeds(leftSpeed, rightSpeed);
}




/**
 * Custom Drive function built for one joystick
 */
void GameDrive::CustomDrive(LogitechGamepad *pad)
{
	// get values from the joystick
	float moveValue = pad->GetLeftY();
	float turnValue = pad->GetRightX();
	moveValue = Limit(moveValue);
	turnValue = Limit(turnValue);

	// local variables to hold the computed PWM values for the motors; default to 0.0 (off)
	float leftMotorSpeed = 0.0;
	float rightMotorSpeed = 0.0;
	float frontTurnSpeed = 0.0;
	float rearTurnSpeed = 0.0;
	
	// square the inputs (while preserving the sign) to increase fine control while permitting full power
	if (moveValue >= 0.0)
	{
		moveValue = (moveValue * moveValue);
	}
	else
	{
		moveValue = -(moveValue * moveValue);
	}
	if (turnValue >= 0.0)
	{
		turnValue = (turnValue * turnValue);
	}
	else
	{
		turnValue = -(turnValue * turnValue);
	}

	// set motor speeds
	leftMotorSpeed = rightMotorSpeed = moveValue;

	// handle turning
	frontTurnSpeed = turnValue/2;
	rearTurnSpeed = turnValue/2;
	if( pad->GetNumberedButton(6) )
	    rearTurnSpeed *= -1;

	// call motor functions
	SetLeftRightMotorSpeeds(leftMotorSpeed, rightMotorSpeed);
	SetWheelAngles(frontTurnSpeed, rearTurnSpeed);
}


/** Set the speed of the right and left motors.
 * This is used once an appropriate drive setup function is called such as
 * TwoWheelDrive(). The motors are set to "leftSpeed" and "rightSpeed"
 * and includes flipping the direction of one side for opposing motors.
 * @param leftSpeed The speed to send to the left side of the robot.
 * @param rightSpeed The speed to send to the right side of the robot.
 */
void GameDrive::SetLeftRightMotorSpeeds(float leftSpeed, float rightSpeed)
{
	wpi_assert(m_rearLeftMotor != NULL && m_rearRightMotor != NULL);
	
	leftSpeed = Limit(leftSpeed);
	rightSpeed = Limit(rightSpeed);

	if (m_frontLeftMotor != NULL)
		m_frontLeftMotor->Set(Limit(leftSpeed) * m_invertedMotors[kFrontLeftMotor]);
	m_rearLeftMotor->Set(Limit(leftSpeed) * m_invertedMotors[kRearLeftMotor]);

	if (m_frontRightMotor != NULL)
		m_frontRightMotor->Set(-Limit(rightSpeed) * m_invertedMotors[kFrontRightMotor]);
	m_rearRightMotor->Set(-Limit(rightSpeed) * m_invertedMotors[kRearRightMotor]);
}


/**
 * Use the steering motors to set the angle of the wheels
 */
void GameDrive::SetWheelAngles( float frontTurn, float rearTurn )
{
	wpi_assert(m_frontSteerMotor != NULL && m_rearSteerMotor != NULL);
	
	frontTurn = Limit(frontTurn);
	rearTurn = Limit(rearTurn);

	m_PIDFront->SetSetpoint(frontTurn);
	m_PIDRear->SetSetpoint(rearTurn);
}


/**
 * Limit motor values to the -1.0 to +1.0 range.
 */
float GameDrive::Limit(float num)
{
	if (num > 1.0)
	{
		return 1.0;
	}
	if (num < -1.0)
	{
		return -1.0;
	}
	return num;
}

/*
 * Invert a motor direction.
 * This is used when a motor should run in the opposite direction as the drive
 * code would normally run it. Motors that are direct drive would be inverted, the
 * Drive code assumes that the motors are geared with one reversal.
 * @param motor The motor index to invert.
 * @param isInverted True if the motor should be inverted when operated.
 */
void GameDrive::SetInvertedMotor(MotorType motor, bool isInverted)
{
	if(motor == 4)
	{m_invertedRear *= -1;}
	else if (motor < 0 || motor > 3)
	{
		wpi_fatal(InvalidMotorIndex);
		return;
	}
	m_invertedMotors[motor] = isInverted ? -1 : 1;
}


