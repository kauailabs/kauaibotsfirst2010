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

enum KickDistance { Near, Mid, Far };

float crab = 0; //0.2; // SL, changed to 0 for competition robot -.02;
float rcrab = 0; //-0.2; // SL, changed to 0 for competition robot.02;

//////
//
// Autonomous Mode Programs
//
//////

// TODO:  Check direction of side-to-side motion
// TODO:  Check duration of side-to-side motion (don't cross into other side of court)
// TODO:  Validate the robot "drift" over time, correct as appropriate
// TODO:  Validate the shooting accuracy, correct as appropriate

AutonomousStep NearCourt1[]={
	{   .7,     0,    	0,	TillBallDetected,	4}, 	// Forward - at least 36 inches, until we kick 
	{    0,	   .8,    	0,	Time,    			1},		// get out of the way		
	{    0,		0,    	0,	Time,    			20} 	// Wait for 20 seconds for autonomous mode to end
};

AutonomousStep MidCourt1[]={
	{   .7,		0,    	0,	TillBallDetected,	2.5}, 	// Forward - until we kick
	{    0,     0,    	0,	Time,    			1},		// wait a bit	
	{   .7,		0,    	0,	TillBallDetected,	2.5}, 	// Forward - until we kick 
	{    0,    .8,    	0,	Time,    			1.3},	// get out of the way	
	{    0,    	0,    	0,	Time,    			20}  	// Wait for 20 seconds for autonomous mode to end
};

AutonomousStep FarCourt1[]={
	{    0,     0,    	0,	Time,				1},		// wait a bit	
	{   .7,     0,    	0,	TillBallDetected,	2}, 	// Forward - until ball is kicked
	{    0,     0,    	0,	Time,				1},		// wait a bit	
	{   .7,     0,    	0,	TillBallDetected,	2.5}, 	// Forward - until ball is kicked 
	{    0,     0,    	0,	Time,				1},		// wait a bit	
	{   .7,     0,    	0,	TillBallDetected,	2.5}, 	// Forward - until ball is kicked
	{  -.7,    .13,    	0,	Time,				5}  	// Move back near the center line
};

struct AutonomousProgramInfo
{
	AutonomousStep *Program;
	int NumberOfSteps;
	KickDistance Tensioner;
};

#define MAKE_PROGRAM_INFO(p) p, sizeof(p) / sizeof(p[0])

// Modify to add/change the program list.  NOTE:  MUST HAVE AT LEAST 1 PROGRAM!!!

AutonomousProgramInfo AutonomousProgramList[] = {
		{ MAKE_PROGRAM_INFO( NearCourt1 ), 	Near },
		{ MAKE_PROGRAM_INFO( MidCourt1 ), 	Mid },
		{ MAKE_PROGRAM_INFO( FarCourt1 ), 	Far },
};

#define NUM_AUTONOMOUS_PROGRAMS (sizeof(AutonomousProgramList)/sizeof(AutonomousProgramList[0]))

// Camera Servo settings (tunable)

const double c_dAzStraight	= 0;
const double c_dAzLeft 		= .75;
const double c_dAzRight 	= -.75;

const double c_dElLevel 	= 0;
const double c_dElDown 		= -.55;
const double c_dElMidDown 	= -.2;

const double c_dVertOffset = -.1;
const double c_dHorizOffset = 0;

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
		
		GetWatchdog().SetEnabled(true);
		GetWatchdog().SetExpiration(3.0);
        // Set camera servos to their default position
		UpdateCameraServos(c_dAzStraight, c_dElDown);
	}

	/**
	 * Main entry point for autonomous mode code
	 **/
	
	static const int cNumDigitalDriverStationInputs = 8;
	
	void  Autonomous () 
	{
		printf("Entering Autonomous mode.\n");
		DriverStation *ds = DriverStation::GetInstance();

		kicker.SetAutoFire(true);		
		
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
		
		// Ensure the kicker mechanism is loaded before running autonomous program
		if ( kicker.GetKickerState() != Kicker::Loaded )
		{
			kicker.SetKickerState(Kicker::Loading);		
			Wait(3.5);
		}
			
		GetWatchdog().SetEnabled(true);
		//UpdateCameraServos(0,0);

		/*
		KickDistance tension = AutonomousProgramList[iSelectedAutonomousProgram].Tensioner;
		switch(tension)
		{
		case Near:
			tensioner.SetTensioner(.8);		// TODO:  Check this
			break;
		case Mid:
			tensioner.SetTensioner(0);
			break;
		case Far:
			tensioner.SetTensioner(-.8);	// TODO:  Check this
			break;
		}
		*/
		
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
		//kicker.RequestQuit();
	}

	// Updates Camera servo horizontal and vertical positions,
	// input values are from -1 to 1.
	void UpdateCameraServos( double dHorizServoJoystick, double dVertServoJoystickY )
	{
		// Correct for non-zero offset in horizontal/vertical servos		
		horizontalServo.Set(((dHorizServoJoystick+1)/2) + c_dHorizOffset);		
		verticalServo.Set(((dVertServoJoystickY + 1)/2) + c_dVertOffset);		
	}
	
	/**
	 * Main entry point for Operator Control (teleop) mode
	 * 
	 * Button 10:  When held down, rotates to target
	 * Trigger:    When held down, will cause the kicker to fire immediately
	 * Button 11:  When held down, will cause the kicker to NOT auto-fire when the ball is detected
	 * Button 6:   When held down, robot will move forward, kick when ball is detected, and then stop
	 * Button 7:   When held down, robot will move to position where Button 6 was last pressed and then stop
	 * 
	 **/
	void  OperatorControl ()
	{
		printf("Entering Teleop mode.\n");
		GetWatchdog().SetEnabled(true);

		// Ensure the kicker is loaded
		if ( kicker.GetKickerState() != Kicker::Loaded)
		{
			kicker.SetKickerState(Kicker::Loading);		
		}
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
			
			// Update Camera Look Angles (Azimuth, Elevation)
			
			bool bLookLeft = stick1.GetRawButton(4) | stick1.GetRawButton(2);
			bool bLookRight = stick1.GetRawButton(5) | stick1.GetRawButton(3);
			bool bLookUp = stick1.GetRawButton(9);			
			double dLook = c_dAzStraight;
			double dTilt = bLookUp ? c_dElLevel : c_dElDown;
			if ( bLookLeft )
			{
				dLook = c_dAzLeft;
				dTilt = c_dElMidDown;
			}
			else if ( bLookRight )
			{
				dLook = c_dAzRight;
				dTilt = c_dElMidDown;
			}
			if ( bLookLeft && bLookRight )
			{
				dLook = c_dAzStraight;
			}

			double dHorizServoJoystick = CameraServoJoystickAdjust(dLook);			
			double dVertServoJoystickY = CameraServoJoystickAdjust(dTilt);
			UpdateCameraServos(dHorizServoJoystick,dVertServoJoystickY);
			
			// Update Drive system with latest Joystick inputs
			
			bool bDriveForwardAndKick = stick1.GetRawButton(6);
			bool bDriveReverseToPreKickPosition = stick1.GetRawButton(7);
			bool bAutoDrive = bDriveForwardAndKick || bDriveReverseToPreKickPosition;
			
			if ( !bAutoDrive )
			{
				myRobot.DoMecanum(stick1.GetY(),stick1.GetX() * -1,stick1.GetTwist() * -1);
			}
			
			if ( stick1.GetTrigger() )
			{
				kicker.RequestFire();
			}

			// If "adjust tensioner" button is held down, update tensioner position.
			if ( stick1.GetRawButton(9))
			{
				tensioner.SetTensioner(stick1.GetZ());				
			}
			
			kicker.SetAutoFire(!stick1.GetRawButton(11));
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
				//kicker.SetAutoFire(true);
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
			
			Wait(0.01);	// Wait a bit, allowing other tasks to run.
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

