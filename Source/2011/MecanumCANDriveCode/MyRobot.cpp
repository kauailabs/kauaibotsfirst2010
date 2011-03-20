#include "WPILib.h"
#include "MecanumDrive.h"
#include "sloganarm.h"
#include "pneumatics.h"
#include "constants.h"


class CANRobotDemo : public SimpleRobot
{
	MecanumDrive myRobot; //Mecanum Drive
    sloganarm myArm; //Arm,Elevator and Claw
	pneumatics airsystem; //Air System
	Joystick stick1; //Joystick

public:
	CANRobotDemo(void):
		myRobot(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS),
	    myArm(SHOULDER_CAN_ADDRESS,HAND_CAN_ADDRESS,ELEVATOR_1_CAN_ADDRESS,ELEVATOR_2_CAN_ADDRESS),
	    airsystem( LOCK_1_CHANNEL, LOCK_2_CHANNEL, FIRE_1_CHANNEL, FIRE_2_CHANNEL,
	    			PRESSURE_SWITCH, COMPRESSOR1),
		stick1(1)
	{
		GetWatchdog().SetExpiration(100);
	}


	void Autonomous(void)
	{
		GetWatchdog().SetEnabled(false);
		// Autonomous Code Here

	}


	void OperatorControl(void)
	{
		GetWatchdog().SetEnabled(true);
		while (IsOperatorControl() && !IsDisabled())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetX(),stick1.GetY(),stick1.GetTwist() * -1); 
			myArm.DoShoulder(stick1.GetRawButton(SHOULDER_BUTTON_1),stick1.GetRawButton(SHOULDER_BUTTON_2));
			myArm.DoHand(stick1.GetRawButton(HAND_BUTTON_1),stick1.GetRawButton(HAND_BUTTON_2));			
			myArm.DoElevator1(stick1.GetRawAxis(3));			
			myArm.DoElevator2(stick1.GetRawAxis(3));	
			airsystem.Deployment(stick1.GetRawButton(DEPLOY_BUTTON));
			airsystem.UnDeployment(stick1.GetRawButton(UNDEPLOY_BUTTON));

			Wait(0.02); // Do we need this???
		}
	}
};

START_ROBOT_CLASS(CANRobotDemo);

