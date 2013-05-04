#include "WPILib.h"
#include "MecanumDrive.h"
#include "constants.h"
#include <math.h>

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
		myRobot(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS,CANJaguar::kSpeed),
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
		//SmartDashboard::Log("Entering Autonomous Mode...",Dashboard_Status); 

		myRobot.SetSafetyEnabled(false);
		while ( IsAutonomous()) 
		{
			myRobot.DoMecanum(0,0,0);
			Wait(2.0);		// Wait 2 seconds
		}
		myRobot.SetSafetyEnabled(false);
		
		//SmartDashboard::Log("Exiting Autonomous Mode...",Dashboard_Status);
	}

	/**
	 * Runs the motors under driver control  
	 */
	
	void OperatorControl(void)
	{
		//SmartDashboard *pDashboard = SmartDashboard::GetInstance();
		
		//pDashboard->Log("Entering Teleop Mode...",Dashboard_Status); 
	
		bool bUsePercentVbusMode = DriverStation::GetInstance()->GetDigitalIn(1);
		myRobot.SetMode( bUsePercentVbusMode ? CANJaguar::kPercentVbus : CANJaguar::kSpeed );		
		
		GetWatchdog().SetEnabled(false);
		myRobot.SetSafetyEnabled(false);

		while (IsOperatorControl() && !IsDisabled())
		{
			GetWatchdog().Feed();
			
			bool bNewUsePercentVbusMode = DriverStation::GetInstance()->GetDigitalIn(1);
			if ( bNewUsePercentVbusMode != bUsePercentVbusMode)
			{
				bUsePercentVbusMode = bNewUsePercentVbusMode;
				myRobot.SetMode( bUsePercentVbusMode ? CANJaguar::kPercentVbus : CANJaguar::kSpeed );		
			}
			
			double twist = stick1.GetTwist();
			if (fabs(twist) < DEADZONE)
				twist = 0;
						
			double y = stick1.GetY();
			if (fabs(y) < DEADZONE)
				y = 0;
									
			double x = stick1.GetX();
			if (fabs(x) < DEADZONE)
				x = 0;
			myRobot.DoMecanum(x,y,twist * -1);
			
			//myRobot.DoMecanum(stick1.GetX(),stick1.GetY(),stick1.GetTwist() * -1);
			
			//pDashboard->Log(stick1.GetX(), Dashboard_Joystick1_X);
			//pDashboard->Log(stick1.GetY(), Dashboard_Joystick1_Y);
			//pDashboard->Log(stick1.GetTwist(), Dashboard_Joystick1_Twist);
			/*pDashboard->Log(myRobot.FrontLeftMotor().GetSpeed(), FrontLeftEncoderSpeed);
			pDashboard->Log(myRobot.FrontRightMotor().GetSpeed(), FrontRightEncoderSpeed);
			pDashboard->Log(myRobot.RearLeftMotor().GetSpeed(), RearLeftEncoderSpeed);
			pDashboard->Log(myRobot.RearRightMotor().GetSpeed(), RearRightEncoderSpeed);
			pDashboard->Log(myRobot.FrontLeftMotor().Get(), FrontLeftMotorSetting);			
			pDashboard->Log(myRobot.FrontRightMotor().Get(), FrontRightMotorSetting);			
			pDashboard->Log(myRobot.RearLeftMotor().Get(), RearLeftMotorSetting);			
			pDashboard->Log(myRobot.RearRightMotor().Get(), RearRightMotorSetting);	
			pDashboard->Log(myRobot.FrontLeftMotor().GetFaults(), FrontLeftMotorFaults);
			pDashboard->Log(myRobot.FrontRightMotor().GetFaults(), FrontRightMotorFaults);
			pDashboard->Log(myRobot.RearLeftMotor().GetFaults(), RearLeftMotorFaults);
			pDashboard->Log(myRobot.RearRightMotor().GetFaults(), RearRightMotorFaults);*/
			Wait(0.05); // Wait 50 ms 
		}

		//pDashboard->Log("Exiting Teleop Mode...",Dashboard_Status); 
	}
};

START_ROBOT_CLASS(DriveMuleRobot);


