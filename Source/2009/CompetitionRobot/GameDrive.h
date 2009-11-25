/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef GameDrive_H_
#define GameDrive_H_

#include <stdlib.h>
#include <VxWorks.h>
#include  "potentiometer.h"
#include "logitechgamepad.h"
#include <WPILib.h>
#include "speedmotor.h"

class SpeedController;
class GenericHID;

/**
 * Utility class for handling Robot drive based on a definition of the motor configuration.
 * The robot drive class handles basic driving for a robot. Currently, 2 and 4 motor standard
 * drive trains are supported. In the future other drive types like swerve and meccanum might
 * be implemented. Motor channel numbers are passed supplied on creation of the class. Those are
 * used for either the Drive function (intended for hand created drive code, such as autonomous)
 * or with the Tank/Arcade functions intended to be used for Operator Control driving.
 */
class GameDrive
{
public:
	typedef enum
	{
		kFrontLeftMotor = 0,
		kFrontRightMotor = 1,
		kRearLeftMotor = 2,
		kRearRightMotor = 3,
		kRearSteerMotor = 4
	} MotorType;

	typedef enum
	{
		kDriveWheelEncoderForward = 1,
		kDriveWheelEncoderReverse = 2,
		kFreeWheelEncoderForward =3,
		kFreeWheelEncoderReverse=4,
	} EncoderPort;
	
	GameDrive();
	GameDrive(SpeedController *frontLeftMotor, SpeedController *rearLeftMotor, SpeedController *frontRightMotor,
					SpeedController *rearRightMotor, UINT32 frontSteerChannel, UINT32 rearSteerChannel,
					Potentiometer *frontPot, Potentiometer *rearPot, float sensitivity = 0.5);
	~GameDrive(void);

	void Drive(float speed, float curve);
	void CustomDrive(LogitechGamepad *pad);
	void SetLeftRightMotorSpeeds(float leftSpeed, float rightSpeed);
	void SetWheelAngles(float frontTurn, float rearTurn);
	void SetInvertedMotor(MotorType motor, bool isInverted);
	Encoder *DriveWheelEncoder() { return m_DriveWheelEncoder; }
	Encoder *FreeWheelEncoder() { return m_FreeWheelEncoder; }
	AnalogChannel *FrontSteeringPot() { return m_frontPot; }
	AnalogChannel *RearSteeringPot() { return m_rearPot; }
	
private:
	float Limit(float num);

	static const INT32 kMaxNumberOfMotors = 4;

	INT32 m_invertedMotors[kMaxNumberOfMotors];
	INT32 m_invertedRear;
	float m_sensitivity;
	bool m_deleteSpeedControllers;
	bool m_deleteSteerControllers;
	SpeedController *m_frontLeftMotor;
	SpeedController *m_frontRightMotor;
	SpeedController *m_rearLeftMotor;
	SpeedController *m_rearRightMotor;
	Speedmotor 		*m_frontSteerMotor;
	Speedmotor		*m_rearSteerMotor;
	Potentiometer	*m_frontPot;
	Potentiometer	*m_rearPot;
	PIDController   *m_PIDFront;
	PIDController	*m_PIDRear;
	Encoder			*m_DriveWheelEncoder;
	Encoder 		*m_FreeWheelEncoder;
	bool			m_deleteAll;
};

#endif

