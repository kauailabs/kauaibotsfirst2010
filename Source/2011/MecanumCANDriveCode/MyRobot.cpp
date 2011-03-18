#include "WPILib.h"
#include "MecanumDrive.h"
#include "sloganarm.h"
/**
 * This is a demo program showing the use of the SimpleRobot base class.
 * It uses CAN to control the Jaguars instead of PWM.
 */ 

#define LEFT_REAR_CAN_ADDRESS 4
#define RIGHT_REAR_CAN_ADDRESS 5
#define RIGHT_FRONT_CAN_ADDRESS 3
#define LEFT_FRONT_CAN_ADDRESS 2
#define ELEVATOR_1_CAN_ADDRESS 7
#define ELEVATOR_2_CAN_ADDRESS 9
#define SHOULDER_CAN_ADDRESS 8
#define HAND_CAN_ADDRESS 6

class CANRobotDemo : public SimpleRobot
{
	MecanumDrive myRobot; // robot drive system
    sloganarm myArm; 
	Joystick stick1; // only joystick

public:
	CANRobotDemo(void):
		myRobot(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS),
	    myArm(SHOULDER_CAN_ADDRESS,HAND_CAN_ADDRESS,ELEVATOR_1_CAN_ADDRESS,ELEVATOR_2_CAN_ADDRESS),
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
			myArm.DoShoulder(stick1.GetRawButton(4),stick1.GetRawButton(2));
			myArm.DoHand(stick1.GetRawButton(5),stick1.GetRawButton(3));			
			myArm.DoElevator1(stick1.GetRawAxis(3));			
			myArm.DoElevator2(stick1.GetRawAxis(3));			
			
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

