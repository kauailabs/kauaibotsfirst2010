#include "WPILib.h"
#include "Subsystems\SwerveSteer.h"
#include "Robotmap.h"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	//RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	SwerveSteer steer1;
public:
	RobotDemo(void):
		//myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		steer1(FRONT_SIDECAR, FRONT_LEFT_STEER_DATA, FRONT_LEFT_STEER_CHIPSELECT,FRONT_LEFT_STEER_CLOCK, FRONT_LEFT_STEER_MOTOR )
	{
		//myRobot.SetExpiration(0.1);
		SmartDashboard::init();
		SmartDashboard::PutData(&steer1);
		steer1.Enable();
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	void Autonomous(void)
	{
		//myRobot.SetSafetyEnabled(false);
		//myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 				//    for 2 seconds
		//myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	void OperatorControl(void)
	{
		//myRobot.SetSafetyEnabled(false);
		while (IsOperatorControl())
		{
			//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			double twist= stick.GetTwist();
			steer1.SetSetpoint(twist);
			
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

