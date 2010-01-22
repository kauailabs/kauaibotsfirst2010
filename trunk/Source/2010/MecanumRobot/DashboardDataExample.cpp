#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "MecanumDrive.h"

/**
 * This is a demo program showing the use of the Dashboard data packing class.
 */ 
class DashboardDataExample : public SimpleRobot
{
	MecanumDrive myRobot; // robot drive system
	Joystick stick1; // X-box (X,Y,Rotation) controller
	DashboardDataFormat dashboardDataFormat;

public:
	DashboardDataExample(void)
		: myRobot(1,2,3,4,1,2,3,4,5,6,7,8,1,2,3) // these must be initialized in the same order
		, stick1(1)		// as they are declared above.
	{
		GetWatchdog().SetExpiration(0.5);
	}

	/**
	 * Main entry point for autonomous mode code
	 **/
	void  Autonomous () 
	{
		GetWatchdog().SetEnabled(true);
		Dashboard &dashboard = m_ds->GetHighPriorityDashboardPacker();
		INT32 i=0;
		while(IsAutonomous())
		{
			GetWatchdog().Feed();
			
			// Autonomous functions go here...
			
			dashboard.Printf("It's been %f seconds, according to the FPGA.\n", GetClock());
			dashboard.Printf("Iterations: %d\n", ++i);
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
		Dashboard &dashboard = m_ds->GetHighPriorityDashboardPacker();
		INT32 i=0;
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetX(),stick1.GetTwist(),stick1.GetY());
			dashboard.Printf("It's been %f seconds, according to the FPGA.\n", GetClock());
			dashboard.Printf("Iterations: %d\n", ++i);
			UpdateDashboard();
			Wait(0.02);
		}	
	}
		
	/**
	 * Send data to the dashboard
	 * Just sending a few values to show the data changing.
	 * These values could be read from hardware.
	 */
	void UpdateDashboard(void)
	{
		dashboardDataFormat.PackAndSend(stick1, myRobot);
	}
};

START_ROBOT_CLASS(DashboardDataExample);

