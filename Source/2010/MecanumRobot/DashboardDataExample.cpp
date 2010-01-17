#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "MecanumDrive.h"

/**
 * This is a demo program showing the use of the Dashboard data packing class.
 */ 
class DashboardDataExample : public SimpleRobot
{
	MecanumDrive myRobot; // robot drive system
	Joystick stick1; // Linear velocity control joystick
	Joystick stick2; // Rotational velocity control joystick
	DashboardDataFormat dashboardDataFormat;

public:
	DashboardDataExample(void)
		: myRobot(1,2,3,4,1,2,3,4,5,6,7,8,1,2,3) // these must be initialized in the same order
		, stick1(1)		// as they are declared above.
		, stick2(2)
	{
		GetWatchdog().SetExpiration(0.1);
	}

	/**
	 * Main entry point for autonomous mode code
	 **/
	void  Autonomous () 
	{
		GetWatchdog().SetEnabled(true);
		Dashboard &dashboard = m_ds->GetLowPriorityDashboardPacker();
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
		Dashboard &dashboard = m_ds->GetLowPriorityDashboardPacker();
		INT32 i=0;
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetX(),stick1.GetY(),stick2.GetX());
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
		dashboardDataFormat.PackAndSend(stick1, stick2, myRobot);
	}
};

START_ROBOT_CLASS(DashboardDataExample);

