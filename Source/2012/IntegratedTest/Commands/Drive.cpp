#include "Drive.h"
#include <math.h>
#include "JoystickConstants.h"

Drive::Drive() 
{
        Requires(drive); // Exclusive use of the Drive Subsystem is required.
}

void Drive::Initialize() 
{
		m_creepOn = false;
		m_lastButtonState = false;
}

void Drive::Execute() 
{
        // Update PercentVbus vs. Speed Mode setting
        bool bUsePercentVbusMode = oi->getDriverStation()->GetDigitalIn(1);
        if ( drive->GetControlMode() == CANJaguar::kPercentVbus )
        {
                if ( !bUsePercentVbusMode ) drive->SetControlMode( CANJaguar::kSpeed );
        }
        else
        {
                if ( bUsePercentVbusMode ) drive->SetControlMode( CANJaguar::kPercentVbus );
        }
        Joystick *pJoystick = oi->getJoystick();
        bool currentButton = pJoystick->GetRawButton(2);
        if(currentButton && !m_lastButtonState)
                {
                	m_creepOn = !m_creepOn;
                }
                m_lastButtonState = currentButton;
                if(m_creepOn){
                	enum DriveGear { kLowGear, kHighGear };
                    drive->SetDriveGear( DriveSubsystem::kLowGear );
                }
                else{
                  	enum DriveGear { kLowGear, kHighGear };
                    drive->SetDriveGear(DriveSubsystem::kHighGear);
                }
        // Drive with current joystick values.
        double twist = pJoystick->GetTwist();
        if (fabs(twist) < DEADZONE)
                twist = 0;
        twist = twist * -1;
        
        double y = pJoystick->GetY();
        if (fabs(y) < DEADZONE)
                y = 0;
                                                        
        double x = pJoystick->GetX();
        if (fabs(x) < DEADZONE)
                x = 0;
        
        y = InputJoystickAdjust(y, JoystickAdjust, JoystickPow, JoystickMult, JoystickDead);    
        x = InputJoystickAdjust(x, JoystickAdjust2, JoystickPow2, JoystickMult2, JoystickDead2);
        twist = ROTATE_DIRECTION * InputJoystickAdjust(twist, JoystickAdjust3, JoystickPow3, JoystickMult3, JoystickDead3);
        
        drive->DoMecanum(x*-1,y*-1,twist);
        //drive->DoMecanum(y,x,twist);
}

bool Drive::IsFinished() 
{
        return false;
}

void Drive::End() 
{
}

void Drive::Interrupted() 
{
        drive->DoMecanum(0,0,0);        // Stop the drive
}

double Drive::InputJoystickAdjust( double dJoystickIn, double dAdjustment, double dExponent, double dMultiplier , double dDead)
{

        double dJoystickOut = 0.0;
        if ((dJoystickIn > dDead) || (dJoystickIn < (-1*dDead)))
        {
                dJoystickOut = ((dAdjustment * pow(dJoystickIn, dExponent)) + ((1 - dAdjustment) * dJoystickIn)) * dMultiplier; 
        }
        return dJoystickOut;
}
