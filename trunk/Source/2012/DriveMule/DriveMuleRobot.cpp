#include "WPILib.h"
#include "MecanumDrive.h"
#include "constants.h"

/**
 * This simple robot program operates a mecanum drive, under control
 * of a joystick in teleop mode.  In autonomous mode, the robot does nothing.
 * In both teleop and autonomous mode, the status messages are output to the
 * "SmartDashboard" client.
 */

class DriveMuleRobot : public SimpleRobot
{
	MecanumDrive myRobot;			// robot drive system
	Joystick stick1; 				// Input Joystick

public:

	DriveMuleRobot(void) :
		myRobot(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS),
		stick1(1)	
	{
		// Configure MotorSafety to require motors are updated at least every 100ms.
		myRobot.SetExpiration(0.1);
		SmartDashboard::init();
	}

	/**
	 * During Autonomous code, the DriveMule shuts the motors off.
	 */
	
	void Autonomous(void)
	{
		SmartDashboard::Log("Entering Autonomous Mode...",Dashboard_Status); 

		myRobot.SetSafetyEnabled(false);
		while ( IsAutonomous()) 
		{
			myRobot.DoMecanum(0,0,0);
			Wait(2.0);		// Wait 2 seconds
		}
		myRobot.SetSafetyEnabled(true);
		
		SmartDashboard::Log("Exiting Autonomous Mode...",Dashboard_Status);
	}

	/**
	 * Runs the motors under driver control  
	 */
	
	void OperatorControl(void)
	{
		SmartDashboard::Log("Entering Teleop Mode...",Dashboard_Status); 
		
		GetWatchdog().SetEnabled(false);
		myRobot.SetSafetyEnabled(false);

		while (IsOperatorControl() && !IsDisabled())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetX(),stick1.GetY(),stick1.GetTwist() * -1);
			
			SmartDashboard::Log(stick1.GetX(), Dashboard_Joystick1_X);
			SmartDashboard::Log(stick1.GetY(), Dashboard_Joystick1_Y);
			SmartDashboard::Log(stick1.GetTwist(), Dashboard_Joystick1_Twist);
			SmartDashboard::Log(myRobot.FrontLeftMotor().GetSpeed(), FrontLeftEncoderSpeed);
			SmartDashboard::Log(myRobot.FrontRightMotor().GetSpeed(), FrontRightEncoderSpeed);
			SmartDashboard::Log(myRobot.RearLeftMotor().GetSpeed(), RearLeftEncoderSpeed);
			SmartDashboard::Log(myRobot.RearRightMotor().GetSpeed(), RearRightEncoderSpeed);
			SmartDashboard::Log(myRobot.FrontLeftMotor().GetFaults(), FrontLeftMotorFaults);
			SmartDashboard::Log(myRobot.FrontRightMotor().GetFaults(), FrontRightMotorFaults);
			SmartDashboard::Log(myRobot.RearLeftMotor().GetFaults(), RearLeftMotorFaults);
			SmartDashboard::Log(myRobot.RearRightMotor().GetFaults(), RearRightMotorFaults);
			SmartDashboard::Log(myRobot.FrontLeftMotor().Get(), FrontLeftMotorSetting);			
			SmartDashboard::Log(myRobot.FrontRightMotor().Get(), FrontRightMotorSetting);			
			SmartDashboard::Log(myRobot.RearLeftMotor().Get(), RearLeftMotorSetting);			
			SmartDashboard::Log(myRobot.RearRightMotor().Get(), RearRightMotorSetting);	
			SmartDashboard::Log(myRobot.FrontLeftMotor().GetP(), FrontLeftMotorP);
			SmartDashboard::Log(myRobot.FrontRightMotor().GetP(), FrontRightMotorP);
			SmartDashboard::Log(myRobot.RearLeftMotor().GetP(), RearLeftMotorP);
			SmartDashboard::Log(myRobot.RearRightMotor().GetP(), RearRightMotorP);
			Wait(0.02); // Wait 20 ms 
		}

		SmartDashboard::Log("Exiting Teleop Mode...",Dashboard_Status); 
	}
};

START_ROBOT_CLASS(DriveMuleRobot);

