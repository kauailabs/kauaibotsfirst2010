#include "Drive.h"
#include <math.h>
#include "JoystickConstants.h"

////////////////////////////////////////////////////////////////////
// Auto-balance Mode
//
// - has three states:  Advancing, Correcting, EBrake
// - has an "advance" direction, determined when entering Auto-balance mode
// - begins in the Advancing state
// - while Advancing drives in the advance direction at a standard velocity
// - transitions to Correcting when the current angle drops below a % of the max pitch
// - while Correcting drives at a velocity = (currPitch / maxPitch ) * standard velocity
// - transitions to EBrake when the current angle stabilizes near 0 for awhile
// - while Ebraking alternates between positive and negative, very low drive values
// - always drives in "low gear"
//
// Balance indications
// 
// (a)   Auto-balance Mode off:  up arrow, down arrow or off to indicate current angle
// (b    Auto-balance Mode on:   stop-sign, to indicate driver commands are ignored
//
// Balance buttons
//
// (a)   Drive Joystick Button 4:  Enter Advance State
// (b)   Drive Joystick Button 8:  Enter EBrake State (if button 4 not already pushed)
////////////////////////////////////////////////////////////////////

// Standard velocity driven in auto-balance mode
const float autoAdvanceStandardVelocity = .3;

// Auto-correction begins when current pitch angle is < this ratio of the max pitch angle
const float autoCorrectBeginMaxAngleRatio = .9;

// Balance occurs when the pitch angle is lower than this
const float autoCorrectCompletePitchMinDegrees = 1.0;

// Balance occurs when exit pitch angle is lower than threshold this many iterations
// Note that this is multiplied by the iteration period (~20 milliseconds)
const int autoCorrectCompleteIterations = 5;

// E-brake velocity
const float eBrakeVelocity = .01;

// Off-balance indicator light angle threshold
const float offBalanceIndicatorThresholdAngle = 5.0;

Drive::Drive() 
{
        Requires(drive); // Exclusive use of the Drive Subsystem is required.
        Requires(trafficcop);
}

void Drive::Initialize() 
{
		// Creep-mode state
		m_creepOn = false;
		m_lastButtonState = false;
		
		// Auto-balance state
		m_autoBalanceState = kDisabled;
		m_maxPitch = 0;
		m_lastY = 0;
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
	
	// Configure normal (high-gear) vs. creep (low-gear) mode
	
	Joystick *pJoystick = oi->getJoystick();
	bool currentButton = pJoystick->GetRawButton(2);
	if( currentButton && !m_lastButtonState )
	{
		m_creepOn = !m_creepOn;
	}
	m_lastButtonState = currentButton;
	if(m_creepOn){
		drive->SetDriveGear( DriveSubsystem::kLowGear );
	}
	else{
		 drive->SetDriveGear(DriveSubsystem::kHighGear);
	}
	
	// Drive with current joystick values.
	
	// Invert Joystick axes and apply deadzones
	
	double twist = pJoystick->GetTwist();
	if (fabs(twist) < DEADZONE)
			twist = 0;
	twist = twist * -1;
	
	double y = pJoystick->GetY();
	if (fabs(y) < DEADZONE)
			y = 0;
	y = y * -1;
													
	double x = pJoystick->GetX();
	if (fabs(x) < DEADZONE)
			x = 0;
	x = x * -1;
	
	// Adjust joystick response curves
	
	y 		= InputJoystickAdjust(y, JoystickAdjust, JoystickPow, JoystickMult, JoystickDead);    
	x 		= InputJoystickAdjust(x, JoystickAdjust2, JoystickPow2, JoystickMult2, JoystickDead2);
	twist 	= ROTATE_DIRECTION * InputJoystickAdjust(twist, JoystickAdjust3, JoystickPow3, JoystickMult3, JoystickDead3);

	// Auto-balance
	
	double pitch, roll, yaw;
	drive->GetEulerAnglesDegrees( pitch, roll, yaw );

	bool autoBalanceMode 	= pJoystick->GetRawButton(9);
	bool ebrakeMode 		= pJoystick->GetRawButton(4);
	if ( autoBalanceMode || ebrakeMode )
	{
		drive->SetDriveGear( DriveSubsystem::kLowGear );
		if ( !m_autoBalanceModeActive )
		{
			if ( ebrakeMode )
			{
				m_autoBalanceState = kEBraking;
			}
			else
			{
				// Just transitioned into auto-balance mode
				m_maxPitch = 0;
				m_autoBalanceState = kAdvancing;
	
				// Determine the direction to travel in "advance" mode
				m_autoBalanceDirection = ((pitch > 0) || (m_lastY > 0)) ? 1 : -1;
			}
		}
		
		x 		= 0;
		twist	= 0;
		
		if ( m_autoBalanceState == kAdvancing )
		{
			// Drive forward at standard velocity until pitch angle
			// falls to a % of the maximum pitch, at which point 
			// auto-correcting occurs.
			
			float correctingThreshold = fabs(m_maxPitch * autoCorrectBeginMaxAngleRatio);
			if ( ( ( m_autoBalanceDirection > 0 ) && ( pitch > correctingThreshold ) ) ||
				 ( ( m_autoBalanceDirection < 0 ) && ( pitch < correctingThreshold ) ) )
			{
				// Continue in same direction at standard velocity
				y = autoAdvanceStandardVelocity * m_autoBalanceDirection;					       			
				
				// update maximum pitch angle, needed for correctingThreshold
				if ( m_autoBalanceDirection == 1 ) // Forward
				{
					if ( pitch > m_maxPitch ) m_maxPitch = pitch; 
				}
				else
				{
					if ( pitch < m_maxPitch ) m_maxPitch = pitch;
				}
			}
			else // pitch angle < correctingThreshold - begin to correct
			{
				m_autoCorrectCompleteCount = 0;
				m_autoBalanceState = kAutoCorrecting;
			}
		}		
		
		if ( m_autoBalanceState == kAutoCorrecting )
		{
			if ( fabs(pitch) < autoCorrectCompletePitchMinDegrees )
			{
				// Robot appears balanced.  If so, transition to EBrake state
				m_autoCorrectCompleteCount++;
				if ( m_autoCorrectCompleteCount >= autoCorrectCompleteIterations )
				{
					m_autoBalanceState = kEBraking;
				}
			}
			else // Not yet balanced; drive proportional to pitch
			{
				m_autoCorrectCompleteCount = 0;
				y = (autoAdvanceStandardVelocity) * (pitch / m_maxPitch);					
				y -= .2;
			}
		}

		if ( m_autoBalanceState == kEBraking )
		{
			y = eBrakeVelocity * ((m_lastY > 0 ) ? -1 : 1);					       			
		}	
	}
	else
	{
		m_autoBalanceState = kDisabled;
	}
	
	m_autoBalanceModeActive = autoBalanceMode;
	
	// Update the traffic cop
	
	
	bool showWaitIndicator = pJoystick->GetRawButton(6);
	if ( ( m_autoBalanceState == kDisabled ) && !showWaitIndicator )
	{
		if ( pitch < (offBalanceIndicatorThresholdAngle * -1) )
		{
			trafficcop->Back();
		}
		else if ( pitch > offBalanceIndicatorThresholdAngle )
		{
			trafficcop->Forward();
		}
		else
		{
			trafficcop->Off();
		}    	
	}
	else
	{
		trafficcop->Stop();
	}

	// Finally, drive
	
	drive->DoMecanum( x, y, twist );
	m_lastY = y;
}

bool Drive::IsFinished() 
{
        return false;
}

void Drive::End() 
{
    drive->DoMecanum(0,0,0);        // Stop the drive
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
