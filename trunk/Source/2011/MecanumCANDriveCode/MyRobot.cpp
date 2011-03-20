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
	DigitalInput left;
	DigitalInput center;
	DigitalInput right;
	Joystick stick1; //Joystick

public:
	CANRobotDemo(void):
		myRobot(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS),
	    myArm(SHOULDER_CAN_ADDRESS,HAND_CAN_ADDRESS,ELEVATOR_1_CAN_ADDRESS,ELEVATOR_2_CAN_ADDRESS),
	    airsystem( LOCK_1_CHANNEL, LOCK_2_CHANNEL, FIRE_1_CHANNEL, FIRE_2_CHANNEL,
	    			PRESSURE_SWITCH, COMPRESSOR1),
	    left(4,3),
	    center(4,1),
	    right(4,2),
	    stick1(1)
	{
		GetWatchdog().SetExpiration(100);
	}


	void Autonomous(void)
	{
		GetWatchdog().SetEnabled(false);
		//double defaultSteeringGain = 0.65;	// default value for steering gain

		int binaryValue;
		int previousValue = 0;
		//double steeringGain;

		// the power profiles for the straight and forked robot path. They are
		// different to let the robot drive more slowly as the robot approaches
		// the fork on the forked line case.
		double forkProfile[] = {0.70, 0.70, 0.55, 0.60, 0.60, 0.50, 0.40, 0.0};
		double straightProfile[] = {0.70, 0.70, 0.60, 0.60, 0.35, 0.35, 0.35, 0.0};

		double *powerProfile;  // the selected power profile
		
		// set the straight vs forked path variables as read from the DS digital
		// inputs or the I/O Setup panel on the driver station.
		bool straightLine = 1;
		powerProfile = (straightLine) ? straightProfile : forkProfile;
		//double stopTime = (straightLine) ? 2.0 : 4.0;
		bool goLeft = false;//!ds->GetDigitalIn(2) && !straightLine;
		printf("StraightLine: %d\n", straightLine);
		printf("GoingLeft: %d\n", goLeft);

		bool atCross = false;	// true when robot has reached end

		// set up timer for 8 second max driving time and use the timer to
		// pick values from the power profile arrays
		Timer *timer = new Timer();
		timer->Start();
		timer->Reset();
		
		Timer *speedtimer = new Timer();
		speedtimer->Start();
		speedtimer->Reset();
		
		int oldTimeInSeconds = -1;
		double time;
		double speed, turn;
		double yspeed;
		// loop until either we hit the "T" at the end or 8 seconds has
		// elapsed. The time to the end should be less than 7 seconds
		// for either path.
		while (((time = timer->Get()) <30.0) && !atCross && IsAutonomous()) {
			int timeInSeconds = (int) time;
			
			if (speedtimer->Get() < 1.8) yspeed = .40; 
				else yspeed = .20;
			
			//int leftValue = left.Get();// ? 1 : 0;	// read the line tracking sensors
			//int middleValue = center.Get();// ? 1 : 0;
			//int rightValue = right.Get();// ? 1 : 0;

		    // compute the single value from the 3 sensors. Notice that the bits
		    // for the outside sensors are flipped depending on left or right
		    // fork. Also the sign of the steering direction is different for left/right.
			turn = 0;								// default to no turn


			if (left.Get()) {
				turn = 1.0;
				yspeed = -.05;
			};
			if (right.Get()) {
				turn  = -1.0;
				yspeed = -.05;
			};
			
			speed = powerProfile[timeInSeconds];	// speed value for this time

			// useful debugging output for tuning your power profile and steering gain
			if(binaryValue != previousValue)
				printf("Time: %2.2f sensor: %d speed: %1.2f turn: %1.2f atCross: %d\n", time, binaryValue, speed, turn, atCross);
			// move the robot forward
			myRobot.DoMecanum(0,yspeed, turn);
			if (binaryValue != 0) previousValue = binaryValue;
			oldTimeInSeconds = timeInSeconds;
			Wait(0.01);
		}
		// stop driving when finished
		myRobot.DoMecanum(0.0, 0.0,0.0);

	}


	void OperatorControl(void)
	{
		GetWatchdog().SetEnabled(true);
		while (IsOperatorControl() && !IsDisabled())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetX(),stick1.GetY(),stick1.GetTwist() * -1); 
			myArm.DoShoulder(stick1.GetRawButton(SHOULDER_BUTTON_1),stick1.GetRawButton(SHOULDER_BUTTON_2));
			//myArm.DoHand(stick1.GetRawButton(HAND_BUTTON_1),stick1.GetRawButton(HAND_BUTTON_2));			
			myArm.DoHand2(stick1.GetRawButton(HAND_BUTTON_3));	
			myArm.DoElevator1(stick1.GetRawAxis(3));			
			myArm.DoElevator2(stick1.GetRawAxis(3));	
			airsystem.Deployment(stick1.GetRawButton(DEPLOY_BUTTON));
			airsystem.UnDeployment(stick1.GetRawButton(UNDEPLOY_BUTTON));

			Wait(0.02); // Do we need this???
		}
	}
};

START_ROBOT_CLASS(CANRobotDemo);

