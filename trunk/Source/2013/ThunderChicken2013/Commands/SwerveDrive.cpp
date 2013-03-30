#include "SwerveDrive.h"
#include "WPILib.h"
#include "../OI.h"
#include "Joystick.h"
#include <math.h>
#include "JoystickConstants.h"
SwerveDrive::SwerveDrive() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
	Requires(Robot::swerveDriveSystem);
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
}

// Called just before this Command runs the first time
void SwerveDrive::Initialize() {
	Robot::swerveDriveSystem->EnablePIDControl(true);
}

// Called repeatedly when this Command is scheduled to run
void SwerveDrive::Execute() {  
	Joystick* pstick = Robot::oi->getdriver_joystick();
	double rotate = pstick->GetTwist();
	double y = pstick->GetY();
	double x = pstick->GetX();
	
	y = -y;		// First, invert the Y axis of the joystick (by default, joystick "forward" is negative)
	
	// Adjust response of all joystick components
	//
	// To visualize the response, use the grapher at http://www.mathsisfun.com/data/function-grapher.php
	//
	// The equation to use is Multiplier*(Adjustment*JoyIn^Power + JoyIn(1-Adjustment))
	//
	// where the X axis input is the joystick value (range is from -1 to 1)
	//
	// Here's an example URL:  
	//
	// http://www.mathsisfun.com/graph/function-grapher.php?func1=1*(.4*x^3+x(1-.4))&xmin=-1&xmax=1&ymin=-1&ymax=1
	
	x = AdjustJoystickResponse( x, JoystickStrafe_Adjust, JoystickStrafe_Power, JoystickStrafe_Multiplier, JoystickStrafe_Deadband );
	y = AdjustJoystickResponse( y, JoystickForward_Adjust, JoystickForward_Power, JoystickForward_Multiplier, JoystickForward_Deadband );
	rotate = AdjustJoystickResponse( rotate, JoystickRotate_Adjust, JoystickRotate_Power, JoystickRotate_Multiplier, JoystickRotate_Deadband );
	
	if ( pstick->GetRawButton(9))
	{
		Robot::swerveDriveSystem->ZeroFieldOrientedDriveYaw();
	}
	
	if ( pstick->GetRawButton(11))
	{
		if ( !Robot::swerveDriveSystem->IsFieldOrientedDriveEnabled())
		{
			Robot::swerveDriveSystem->EnableFieldOrientedDrive(true);
		}
	}
	else if ( pstick->GetRawButton(10))
	{
		if ( Robot::swerveDriveSystem->IsFieldOrientedDriveEnabled())
		{
			Robot::swerveDriveSystem->EnableFieldOrientedDrive(false);
		}
	}
	
	Robot::swerveDriveSystem->DoSwerve(x,y,rotate);
}

// Make this return true when this Command no longer needs to run execute()
bool SwerveDrive::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void SwerveDrive::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SwerveDrive::Interrupted() {

}

double SwerveDrive::AdjustJoystickResponse( double dJoystickIn, double dAdjustment, double dExponent, double dMultiplier , double dDead) 
{         
	double dJoystickOut = 0.0;         
	if ((dJoystickIn > dDead) || (dJoystickIn < (-1*dDead)))         
	{                 
		dJoystickOut = ((dAdjustment * pow(dJoystickIn, dExponent)) + ((1 - dAdjustment) * dJoystickIn)) * dMultiplier;          
	}         
	return dJoystickOut; 
} 