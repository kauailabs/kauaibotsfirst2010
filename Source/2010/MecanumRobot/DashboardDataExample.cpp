#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "AutoRotationMecanumDrive.h"
#include <math.h>
#include "Kicker.h"
#include "Tensioner.h"

enum WaitType { Time=0, TillOnTarget, TillBallDetected, TillAtZeroDegrees };
struct AutonomousStep
{
	float vX;
	float vY;
	float vRot;
	WaitType wait;
	float waitPeriodInSeconds; 
};

float crab = -.02;
float rcrab = .02;
AutonomousStep FarCourt1[]={
		{   .7,     0+crab,    	0,            Time,    10}, // Make sure we kick last ball 
		{    0,    	0,    	0,            Time,    20}  // Wait for 20 seconds for autonomous mode to end
};
AutonomousStep FarCourt2[]={
		{   .7,     0+crab,    	0,            Time,    1.3}, // first target is 22 inches from start
		{    0,    	0+crab,    	0,    TillOnTarget,     3}, // Rotate to the target (or give up in 3 seconds)			
		{   .7,     0+crab,    	0,            Time,    1.8}, // 36 inches 
		{    0,    	0+crab,    	0,    TillOnTarget,     3}, // Rotate to the target (or give up in 3 seconds)		
		{   .7,     0+crab,    	0,            Time,    1.8}, //36 inches 
		{    0,    	0+crab,    	0,    TillOnTarget,     3}, // Rotate to the target (or give up in 3 seconds)	
		{   .7,     0+crab,    	0,            Time,    1.5}, // Make sure we kick last ball 
		{    0,    	0,    	0,            Time,    20}  // Wait for 20 seconds for autonomous mode to end
};
AutonomousStep MidCourt1[]={
		{   .7,     0+crab,    	  0,    Time,    0.2}, 	
		{   .8,     0+crab,    	  0.5,  Time,    0.2}, //24 inches + rotation
		{   .8,     0+crab,    	  0,    Time,    0.4},			// rotate back	
		{   .8,     0+crab,    	  -0.5,  Time,   0.2}, 
		{   .7,     0+crab,    	  0,    Time,   1.3}, 	
		{   .7,     0+crab,    	  0,TillBallDetected,   2.1}, // Make sure we kick last ball 
		{    0,         0,    		0,   Time,    20.0}  // Wait for 20 seconds for autonomous mode to end
		};

AutonomousStep MidCourt2[]={
		{   .7,0+crab,    	0+rcrab,            Time,    10}, // Make sure we kick last ball 
//		{    0,    .8,    	0,    		Time,    1.5},		// get out of the way	
		{    0,    	0,    	0,            Time,    20}  // Wait for 20 seconds for autonomous mode to end
};

AutonomousStep NearCourt1[]={
		{   .8,     0+crab,    	.8,            Time,    1.3}, // 36 inches 
//		{    .8,  0+crab,    		0,    TillOnTarget,     2}, // Rotate to the target (or give up in 3 seconds)	
		{   .7,   0+crab,    	0,    TillBallDetected,    2.1}, // Make sure we kick last ball 
//		{    0,       .8,    	0,    		Time,    1.5},		// get out of the way		
		{    0,         0,    		0,          Time,    20}  // Wait for 20 seconds for autonomous mode to end
		};

AutonomousStep NearCourt2[]={
		{   .7,     0+crab,    	0,            Time,    5}, // Make sure we kick last ball 
//		{    0,         .8,    	0,    		Time,    1.5},		// get out of the way	
		{    0,    	0,    	0,            Time,    20}  // Wait for 20 seconds for autonomous mode to end
		};

AutonomousStep sandbox[]={
		{   0,   -1,    	0,    Time,    .2},	  
		{   0,   1,    	0,    Time,    .2},	
		{   0,   0,    	0,    TillOnTarget,    5},
		{   0,   -1,    	0,    Time,    .2},		
		{   0,   1,    	0,    Time,    .2},			
		{   0,   0,    	0,    TillAtZeroDegrees,    5}, 		
		{   0,   -1,    	0,    Time,    .2	 }, 
		{   0,   0,    	0,    Time,    .2	 } 
		};

struct AutonomousProgramInfo
{
	AutonomousStep *Program;
	int NumberOfSteps;
};

#define MAKE_PROGRAM_INFO(p) p, sizeof(p) / sizeof(p[0])

// Modify to add/change the program list.  NOTE:  MUST HAVE AT LEAST 1 PROGRAM!!!

AutonomousProgramInfo AutonomousProgramList[] = {
		{ MAKE_PROGRAM_INFO( FarCourt1 ) },
		{ MAKE_PROGRAM_INFO( FarCourt2 ) },		
		{ MAKE_PROGRAM_INFO( MidCourt1 ) },
		{ MAKE_PROGRAM_INFO( MidCourt2 ) },
		{ MAKE_PROGRAM_INFO( NearCourt1 ) },
		{ MAKE_PROGRAM_INFO( NearCourt2 ) },
		{ MAKE_PROGRAM_INFO( sandbox ) },
};

#define NUM_AUTONOMOUS_PROGRAMS (sizeof(AutonomousProgramList)/sizeof(AutonomousProgramList[0]))

/**
 * This is a demo program showing the use of the Dashboard data packing class.
 */ 
class DashboardDataExample : public SimpleRobot
{
	AutoRotationMecanumDrive myRobot; 	// robot drive system
	Joystick stick1; 					// Drive (X,Y,Rotation) controller
	Joystick stick2; 					// Camera Servo controller
	Servo horizontalServo;				// Camera horizontal servo
	Servo verticalServo;				// Camera vertical servo
	DashboardDataFormat dashboardDataFormat;	// Dashboard updater
	Kicker kicker;						// Kicker system
	Tensioner tensioner;				// Tensioner system
	
public:
	DashboardDataExample(void)
		: myRobot(1,2,3,4,1,2,3,4,5,6,7,8,1,2,3,&dashboardDataFormat,&kicker,&horizontalServo,&verticalServo) // these must be initialized in the same order
		, stick1(1)		// as they are declared above.
		, stick2(2)
		, horizontalServo(9)
		, verticalServo(10)
		, kicker(5,9,10)
		, tensioner(6,5)
	{
		GetWatchdog().SetEnabled(false);
		// Create and set up a camera instance. first wait for the camera to start
		// if the robot was just powered on. This gives the camera time to boot.
		Wait(5.0);
		AxisCamera& camera = AxisCamera::GetInstance();
		camera.WriteResolution(AxisCamera::kResolution_320x240);
		camera.WriteBrightness(50);     // TODO:  Tune This..
	//	Wait(20.0);
		GetWatchdog().SetEnabled(true);
		GetWatchdog().SetExpiration(2.0);
        // Set camera servos to their default position
		UpdateCameraServos(0,0);
	}

	/**
	 * Main entry point for autonomous mode code
	 **/
	
	static const int cNumDigitalDriverStationInputs = 8;
	
	void  Autonomous () 
	{
		printf("Entering Autonomous mode.\n");
		DriverStation *ds = DriverStation::GetInstance();
		
		// Get information to help decide which autonomous
		// program to run...
		DriverStation::Alliance alliance = ds->GetAlliance();
		UINT32 location = ds->GetLocation();

		// Determine which program to use.
		int iSelectedAutonomousProgram = 0;
		
		for ( int i = 1; i < cNumDigitalDriverStationInputs + 1; i++)
		{
			if ( ds->GetDigitalIn(i) && (i <= NUM_AUTONOMOUS_PROGRAMS))
			{
				iSelectedAutonomousProgram = i - 1; // Translate to a 0-based index.
				break;
			}
		}
		
		kicker.SetKickerState(Kicker::Loading);		
		
		GetWatchdog().SetEnabled(true);
		UpdateCameraServos(0,0);
		
		AutonomousStep instruction;	
		int step=0;
		int numEntries = AutonomousProgramList[iSelectedAutonomousProgram].NumberOfSteps;
		// Status variable to return results from AutonomousDrive
		AutoRotationMecanumDrive::WaitType success;
		while(IsAutonomous() && (step < numEntries) )
		{
			instruction = AutonomousProgramList[iSelectedAutonomousProgram].Program[step];

			success = myRobot.AutonomousDrive( 
					instruction.vX,
					instruction.vY,
					instruction.vRot,
					(AutoRotationMecanumDrive::WaitType)instruction.wait,
					instruction.waitPeriodInSeconds,
					GetWatchdog());	// do an autonomous step
			dashboardDataFormat.PackAndSend(stick1, myRobot,kicker,tensioner); 

		    step++;
		}
		GetWatchdog().SetEnabled(false);
		// should never get here
		kicker.RequestQuit();
	}

	// Updates Camera servo horizontal and vertical positions,
	// input values are from -1 to 1.
	void UpdateCameraServos( double dHorizServoJoystick, double dVertServoJoystickY )
	{
		horizontalServo.Set((dHorizServoJoystick+1)/2);

		// Correct for non-zero offset in vertical servo.
		
		const double dVertOffset = -.1;
		verticalServo.Set(((dVertServoJoystickY + 1)/2) + dVertOffset);		
	}
	
	/**
	 * Main entry point for Operator Control (teleop) mode
	 * 
	 * Button 10:  When held down, rotates to target
	 * Trigger:    When held down, will cause the kicker to fire immediately
	 * Button 11:  When held down, will cause the kicker to fire when the ball is detected
	 * Button 6:   When held down, robot will move forward, kick when ball is detected, and then stop
	 * Button 7:   When held down, robot will move to position where Button 6 was last pressed and then stop
	 * 
	 **/
	void  OperatorControl ()
	{
		printf("Entering Teleop mode.\n");
		GetWatchdog().SetEnabled(true);

		// Load up the kicker
		kicker.SetKickerState(Kicker::Loading);		
		Timer kickerShutdownTimer;
		
		Timer driveForwardAndKickTimer;
		driveForwardAndKickTimer.Reset();
		
		Timer driveReverseToPreKickPositionTimer;
		driveReverseToPreKickPositionTimer.Reset();
		
		bool bInDriveForwardAndKickMode = false;
		bool bInDriveReverseToPreKickPositionMode = false;
		bool bLastInDriveForwardAndKick = false;
		
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			
			bool bDriveForwardAndKick = stick1.GetRawButton(6);
			bool bDriveReverseToPreKickPosition = stick1.GetRawButton(7);
			bool bAutoDrive = bDriveForwardAndKick || bDriveReverseToPreKickPosition;
			
			if ( !bAutoDrive )
			{
				myRobot.DoMecanum(stick1.GetY(),stick1.GetX() * -1,stick1.GetTwist() * -1);
			}
			
			double dHorizServoJoystick = CameraServoJoystickAdjust(stick2.GetX());			
			double dVertServoJoystickY = CameraServoJoystickAdjust(stick2.GetY());
			UpdateCameraServos(dHorizServoJoystick,dVertServoJoystickY);
			
			if ( stick1.GetTrigger() )
			{
				kicker.RequestFire();
			}

			tensioner.SetTensioner(stick1.GetZ());
			kicker.SetAutoFire(stick1.GetRawButton(11));
			myRobot.SetAutoRotationMode(stick1.GetRawButton(10), true);
				
			//
			// Drive Forward and Kick Mode
			//
			
			if ( !bInDriveForwardAndKickMode && bDriveForwardAndKick && !bLastInDriveForwardAndKick )
			{
				// Entering drive forward and kick mode

				bInDriveForwardAndKickMode = true;
				printf("Entering Drive Forward and Kick Mode.\n");
				driveForwardAndKickTimer.Reset();
				driveForwardAndKickTimer.Start();
				
				// Move forward until ball is detected, then kick the ball and stop
				kicker.SetAutoFire(true);
			}

			if ( bInDriveForwardAndKickMode )
 			{
				if ( !bDriveForwardAndKick || kicker.IsBallDetected() )
				{
					// Exit Drive Forward and Kick Mode
					printf("Exiting Drive Forward and Kick Mode.\n");
					bInDriveForwardAndKickMode = false;
					// Driver has requested that auto-drive forward and kick mode be terminated
					driveForwardAndKickTimer.Stop();
					printf("Drove forward for %f seconds.\n",driveForwardAndKickTimer.Get());
					myRobot.DoMecanum(0,0,0,false);						
				}
				else // Drive forward
				{
					myRobot.DoMecanum(.8,0,0,false);					
				}
			}
			
			//
			// Drive Reverse to Prekick Position Mode
			//
			
			if ( !bInDriveReverseToPreKickPositionMode && bDriveReverseToPreKickPosition &&
					(driveForwardAndKickTimer.Get() > 0 ) )
			{
				// Entering drive reverse and kick mode

				bInDriveReverseToPreKickPositionMode = true;
				printf("Entering Drive Reverse to PreKick Position  Mode.\n");
				driveReverseToPreKickPositionTimer.Reset();
				driveReverseToPreKickPositionTimer.Start();
			}
			if ( bInDriveReverseToPreKickPositionMode )
 			{
				if ( !bDriveReverseToPreKickPosition || driveReverseToPreKickPositionTimer.HasPeriodPassed(driveForwardAndKickTimer.Get()))
				{
					// Exit Drive Reverse to PreKick Position Mode
					printf("Exiting Drive Reverse to PreKick Position Mode.\n");
					driveReverseToPreKickPositionTimer.Stop();
					driveReverseToPreKickPositionTimer.Reset();
					bInDriveReverseToPreKickPositionMode = false;				
					myRobot.DoMecanum(0,0,0,false);
					driveForwardAndKickTimer.Reset();
				}
				else
				{
					myRobot.DoMecanum(-.8,0,0,false);														
				}
 			}
			
			UpdateDashboard();
			
const double cKickerShutdownTimeoutInSeconds = 118.0;
			
			if ( kickerShutdownTimer.HasPeriodPassed(cKickerShutdownTimeoutInSeconds))
			{
				kicker.RequestQuit();				
			}
			
			bLastInDriveForwardAndKick = bDriveForwardAndKick;
			
			Wait(0.02);	// Wait a bit, allowing other tasks to run.
		}	
	}
		
	/**
	 * Send data to the dashboard
	 */
	void UpdateDashboard(void)
	{
		dashboardDataFormat.PackAndSend(stick1, myRobot,kicker,tensioner);
	}

	double CameraServoJoystickAdjust( double dJoystickIn )
	{
		return dJoystickIn;
		
		const double cCameraServoJoystickExponent = 2.0;
		
		double dJoystickOut = 0.0;
		if ( dJoystickIn > 0 )
			dJoystickOut = pow(dJoystickIn, cCameraServoJoystickExponent);
		else
			dJoystickOut = -1 * pow(dJoystickIn, cCameraServoJoystickExponent);

		return dJoystickOut;
	}
	
};

START_ROBOT_CLASS(DashboardDataExample);

