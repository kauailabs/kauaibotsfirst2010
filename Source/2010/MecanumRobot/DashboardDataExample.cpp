#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "AutoRotationMecanumDrive.h"
#include <math.h>
#include "Kicker.h"
#include "Tensioner.h"

enum WaitType { Time, TillOnTarget, TillBallDetected };
struct AutonomousStep
{
	float vX;
	float vY;
	float vRot;
	WaitType wait;
	float waitPeriodInSeconds; 
};
AutonomousStep AutonomousProgram[]={
		// 
		{    0,      0,    0,            Time,     3}, // Wait for 3 seconds before doing anything		
		{   -3,      0,    0,            Time,    .5}, // Move Backward for .5 second.
		{    0,      0,   .3,            Time,    .5}, // Rotate a bit, for .5 second.
		{    0,      0,    0,    TillOnTarget,     3}, // Rotate to the target (or give up in 3 seconds)
		{   .3,      0,    0,TillBallDetected,    .5}, // Drive forward until ball is kicked (or give up in 2 seconds)
		{   .3,      0,    0,            Time,    .5}, // Move Forward for .5 second.
		{    0,      0,    0,    TillOnTarget,     3}, // Rotate to the target (or give up in 3 seconds)		
		{   .3,      0,    0,            Time,    .5}, // Move Forward for .5 second.
		{   .3,      0,    0,            Time,     2},  // Drive forward until ball is kicked (or give up in 2 seconds)
		{    0,      0,    0,    TillOnTarget,     3}, // Rotate to the target (or give up in 3 seconds)		
		{   .3,      0,    0,TillBallDetected,    .5}, // Drive forward until ball is kicked (or give up in 2 seconds)
		{    0,      0,    0,            Time,    20}  // Wait for 20 seconds for autonomous mode to end
};


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
		camera.WriteBrightness(0);     // TODO:  Tune This...
		GetWatchdog().SetEnabled(true);
		GetWatchdog().SetExpiration(1.0);
        // Set camera servos to their default position
		UpdateCameraServos(0,0);
	}

	/**
	 * Main entry point for autonomous mode code
	 **/
	void  Autonomous () 
	{
		DriverStation *ds = DriverStation::GetInstance();
		
		// Get information to help decide which autonomous
		// program to run...
		DriverStation::Alliance alliance = ds->GetAlliance();
		UINT32 location = ds->GetLocation();
		float analogInput = ds->GetAnalogIn(0);
		
		kicker.SetKickerState(Kicker::Loading);		
		
		GetWatchdog().SetEnabled(true);
		UpdateCameraServos(0,0);
		
		bool bRunOnce = false;
		AutonomousStep instruction;	
		int step;
		// Status variable to return results from AutonomousDrive
		AutoRotationMecanumDrive::WaitType success;
		while(IsAutonomous() && !bRunOnce )
		{
			instruction = AutonomousProgram[step];
			GetWatchdog().Feed();  // TODO:  Review this.  Add to AutonomousDrive()?

			success = myRobot.AutonomousDrive( 
					instruction.vX,
					instruction.vY,
					instruction.vRot,
					(AutoRotationMecanumDrive::WaitType)instruction.wait,
					instruction.waitPeriodInSeconds);	// do an autonomous step
			dashboardDataFormat.PackAndSend(stick1, myRobot,kicker,tensioner); 
/*			
			// Autonomous functions go here...
			//
			// Example:
			//
			// Status variable to return results from AutonomousDrive
			AutoRotationMecanumDrive::WaitType success;
			// Move Right for .5 second.
			//success = myRobot.AutonomousDrive( 0, .3, 0,AutoRotationMecanumDrive::Time,.5);
			// Move Left for .5 second.

			GetWatchdog().Feed();  // TODO:  Review this.  Add to AutonomousDrive()?
			// Move Backward for .5 second.
			success = myRobot.AutonomousDrive( -.3, 0, 0,AutoRotationMecanumDrive::Time,.5);
			GetWatchdog().Feed();  // TODO:  Review this.  Add to AutonomousDrive()
			// Rotate a bit, for .5 second.
			success = myRobot.AutonomousDrive( 0, 0, .3,AutoRotationMecanumDrive::Time,.5);
			GetWatchdog().Feed();  // TODO:  Review this.  Add to AutonomousDrive()
			// Rotate to the target (or give up in 3 seconds)
			success = myRobot.AutonomousDrive(0,0,0,AutoRotationMecanumDrive::TillOnTarget,3);
			GetWatchdog().Feed();  // TODO:  Review this.  Add to AutonomousDrive()
			// Drive forward until ball is kicked (or give up in 2 seconds)
			success = myRobot.AutonomousDrive(.3,0,0,AutoRotationMecanumDrive::TillBallDetected,2);
			GetWatchdog().Feed();  // TODO:  Review this.  Add to AutonomousDrive()
			
			dashboardDataFormat.PackAndSend(stick1, myRobot,kicker,tensioner);
	*/	
		    bRunOnce = true;
		}
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
	 **/
	void  OperatorControl ()
	{
		GetWatchdog().SetEnabled(true);

		// Load up the kicker
		kicker.SetKickerState(Kicker::Loading);		
		Timer kickerShutdownTimer;
		kickerShutdownTimer.Start();
		
		while (IsOperatorControl())
		{
			GetWatchdog().Feed();
			myRobot.DoMecanum(stick1.GetY() * -1,stick1.GetX(),stick1.GetTwist() * -1);
			
			double dHorizServoJoystick = CameraServoJoystickAdjust(stick2.GetX());			
			double dVertServoJoystickY = CameraServoJoystickAdjust(stick2.GetY());
			UpdateCameraServos(dHorizServoJoystick,dVertServoJoystickY);
			
			if ( stick1.GetTrigger() )
			{
				kicker.RequestFire();
			}

			tensioner.SetTensioner(stick1.GetZ());
			kicker.SetAutoFire(stick1.GetRawButton(11));
			myRobot.SetAutoRotationMode(stick1.GetRawButton(10));
				
			UpdateDashboard();
			
const double cKickerShutdownTimeoutInSeconds = 118.0;			
			
			if ( kickerShutdownTimer.HasPeriodPassed(cKickerShutdownTimeoutInSeconds))
			{
				kicker.RequestQuit();				
			}
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

