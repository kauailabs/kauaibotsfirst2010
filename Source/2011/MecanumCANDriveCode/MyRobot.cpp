#include "WPILib.h"
#include "MecanumDrive.h"
/**
 * This is a demo program showing the use of the SimpleRobot base class.
 * It uses CAN to control the Jaguars instead of PWM.
 */ 

#define LEFT_REAR_CAN_ADDRESS 2
#define RIGHT_REAR_CAN_ADDRES 3
#define RIGHT_FRONT_CAN_ADDRESS 4
#define LEFT_FRONT_CAN_ADDRESS 5

class CANRobotDemo : public SimpleRobot
{
	MecanumDrive myRobot; // robot drive system
	Joystick stick1; // only joystick

public:
	CANRobotDemo(void):
		myRobot(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRES),
		stick1(1)
	{
		GetWatchdog().SetExpiration(100);
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		GetWatchdog().SetEnabled(false);
		//myRobot.Drive(0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		//myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		printf("In OperatorControl\n");
		GetWatchdog().SetEnabled(true);
		while (IsOperatorControl() && !IsDisabled())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetX(),stick1.GetY(),stick1.GetTwist() * -1); // drive with arcade style (use right stick)
			// Print out some information about the Left motor.
			printf ("Left=> ");
			printf ("Bus: %5.2f V ", myRobot.RearLeftMotor().GetBusVoltage());
			printf ("Out: %6.2f V ", myRobot.RearLeftMotor().GetOutputVoltage());
			printf ("Cur: %4.1f A ", myRobot.RearLeftMotor().GetOutputCurrent());
			printf ("Temp: %5.1f Deg C ", myRobot.RearLeftMotor().GetTemperature());
			printf ("LimSw: %s%s ", myRobot.RearLeftMotor().GetForwardLimitOK() ? "F":"-",
					myRobot.RearLeftMotor().GetReverseLimitOK() ? "R":"-");
			printf ("PwrCyc: %d ", myRobot.RearLeftMotor().GetPowerCycled() ? 1:0);
			printf ("\n");

			Wait(0.02);
		}
	}
};

START_ROBOT_CLASS(CANRobotDemo);

