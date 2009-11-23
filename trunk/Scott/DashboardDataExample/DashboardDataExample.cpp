#include "WPILib.h"
#include "DashboardDataFormat.h"

/**
 * This is a demo program showing the use of the Dashboard data packing class.
 */ 
class DashboardDataExample : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	DashboardDataFormat dashboardDataFormat;

public:
	DashboardDataExample(void)
		: myRobot(1, 2)	// these must be initialized in the same order
		, stick(1)		// as they are declared above.
	{
		GetWatchdog().SetExpiration(0.1);
	}

	/**
	 * Main entry point for autonomous mode code
	 **/
	void  Autonomous () 
	{
		GetWatchdog().SetEnabled(true);
		Dashboard &dashboard = m_ds->GetDashboardPacker();
		INT32 i=0;
		while(IsAutonomous())
		{
			GetWatchdog().Feed();
			//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
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
		Dashboard &dashboard = m_ds->GetDashboardPacker();
		INT32 i=0;
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			//myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
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
		static float num = 0.0;
		dashboardDataFormat.m_AnalogChannels[0][0] = num;
		dashboardDataFormat.m_AnalogChannels[0][1] = 5.0 - num;
		dashboardDataFormat.m_DIOChannels[0]++;
		dashboardDataFormat.m_DIOChannelsOutputEnable[0]--;
		num += 0.01;
		if (num > 5.0) num = 0.0;
		dashboardDataFormat.PackAndSend();
	}
};

START_ROBOT_CLASS(DashboardDataExample);

