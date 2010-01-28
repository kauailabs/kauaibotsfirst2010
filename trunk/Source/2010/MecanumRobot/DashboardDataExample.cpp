#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "MecanumDrive.h"
#include "Vision/AxisCamera2010.h"
#include "Vision/HSLImage.h"

#define MINIMUM_SCORE 0.01

/**
 * This is a demo program showing the use of the Dashboard data packing class.
 */ 
class DashboardDataExample : public SimpleRobot
{
	MecanumDrive myRobot; // robot drive system
	Joystick stick1; // X-box (X,Y,Rotation) controller
	Joystick stick2; // Camera Servo controller
	Servo horizontalServo;
	Servo verticalServo;
	DashboardDataFormat dashboardDataFormat;
	
public:
	DashboardDataExample(void)
		: myRobot(1,2,3,4,1,2,3,4,5,6,7,8,1,2,3) // these must be initialized in the same order
		, stick1(1)		// as they are declared above.
		, stick2(2)
		, horizontalServo(9)
		, verticalServo(10)
	{
		GetWatchdog().SetEnabled(false);
		// Create and set up a camera instance. first wait for the camera to start
		// if the robot was just powered on. This gives the camera time to boot.
		Wait(10.0);
		//printf("Getting camera instance\n");
		AxisCamera& camera = AxisCamera::getInstance();
		//printf("Setting camera parameters\n");
		camera.writeResolution(k320x240);
		camera.writeBrightness(0);
		GetWatchdog().SetEnabled(true);
		GetWatchdog().SetExpiration(0.5);
        // Set camera servos to their default position
		horizontalServo.Set(.5);
        verticalServo.Set(.5);
	}

	/**
	 * Main entry point for autonomous mode code
	 **/
	void  Autonomous () 
	{
		GetWatchdog().SetEnabled(true);
		//Dashboard &dashboard = m_ds->GetHighPriorityDashboardPacker();
		//INT32 i=0;
		while(IsAutonomous())
		{
			GetWatchdog().Feed();
			
			// Autonomous functions go here...
			
			//dashboard.Printf("It's been %f seconds, according to the FPGA.\n", GetClock());
			//dashboard.Printf("Iterations: %d\n", ++i);
			UpdateDashboard();
			Wait(0.02);
		}
	}

	/**
	 * Main entry point for Operator Control (teleop) mode
	 **/
	void  OperatorControl ()
	{
		GetWatchdog().SetEnabled(true);
		//Dashboard &dashboard = m_ds->GetHighPriorityDashboardPacker();
		//INT32 i=0;
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetX(),stick1.GetTwist(),stick1.GetY());
			//dashboard.Printf("It's been %f seconds, according to the FPGA.\n", GetClock());
			//dashboard.Printf("Iterations: %d\n", ++i);
			horizontalServo.Set((stick2.GetX()+1)/2);
			
			// Scale the vertical servo for tilt range
			// of -20 degrees to 90 degrees
			
			double dVertServoJoystickY = stick2.GetY();
			if ( dVertServoJoystickY < -.21) dVertServoJoystickY = -.21;
			verticalServo.Set((dVertServoJoystickY * -.82 ) + .82);
			
			UpdateDashboard();
			Wait(0.02);
		}	
	}
		
	/**
	 * Send data to the dashboard
	 */
	void UpdateDashboard(void)
	{
		dashboardDataFormat.PackAndSend(stick1, myRobot);

		AxisCamera& camera = AxisCamera::getInstance();
		if ( camera.freshImage() ) 
		{
			// get the gyro heading that goes with this image
			double gyroAngle = myRobot.Gyroscope().PIDGet();
			
			// get the camera image
			ColorImage *image = camera.GetImage();

			// find FRC targets in the image
			vector<Target> targets = Target::FindCircularTargets(image);
			delete image;  // Be sure to delete the image!
			
			if (targets.size() == 0 || targets[0].m_score < MINIMUM_SCORE)
			{
				// no targets found. Make sure the first one in the list is 0,0
				// since the dashboard program annotates the first target in green
				// and the others in magenta. With no qualified targets, they'll all
				// be magenta.
				Target nullTarget;
				nullTarget.m_majorRadius = 0.0;
				nullTarget.m_minorRadius = 0.0;
				nullTarget.m_score = 0.0;
				if (targets.size() == 0)
					targets.push_back(nullTarget);
				else
					targets.insert(targets.begin(), nullTarget);
			
				dashboardDataFormat.sendVisionData(0.0, gyroAngle, 0.0, 0.0, targets);
				/*if (targets.size() == 0)
					printf("No target found\n\n");
				else
					printf("No valid targets found, best score: %f\n", targets[0].m_score);
					*/
			}
			else // Targets detected
			{
				// set the new PID heading setpoint to the first target in the list
				double horizontalAngle = targets[0].GetHorizontalAngle();
				double setPoint = gyroAngle + horizontalAngle;

				//turnController.SetSetpoint(setPoint);
				
				// send dashboard data for target tracking
				dashboardDataFormat.sendVisionData(0.0, gyroAngle, 0.0, targets[0].m_xPos / targets[0].m_xMax, targets);

				//	targets[0].Print();
			}
		}		
	}
};

START_ROBOT_CLASS(DashboardDataExample);

