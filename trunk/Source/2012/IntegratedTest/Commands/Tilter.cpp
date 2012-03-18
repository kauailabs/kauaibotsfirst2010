#include "Tilter.h"

Tilter::Tilter() {
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
        Requires(tilter);
}

// Called just before this Command runs the first time
void Tilter::Initialize() {
		m_tilterDown = false;
		m_lastButtonState = false;
}

// Called repeatedly when this Command is scheduled to run
void Tilter::Execute() {
		Joystick*zjoystick;
		zjoystick = oi->getJoystick();
		bool currentButton=zjoystick->GetRawButton(1);
		if(currentButton && !m_lastButtonState)
		//if ( ( currentButton == true ) && ( m_lastButtonState == false ) )
		{
			m_tilterDown = !m_tilterDown;
		}
		m_lastButtonState = currentButton;
		if (chute->IsUp())
		{
			tilter->Up();
		}
		else
		{
			if(!m_tilterDown)
			{
				tilter->Up();
			}
			else
			{
				tilter->Down();
			}
		}
		SmartDashboard*sd=SmartDashboard::GetInstance();
		sd->PutBoolean("Tilter Down",m_tilterDown);
}

// Make this return true when this Command no longer needs to run execute()
bool Tilter::IsFinished() {
        return false;
}

// Called once after isFinished returns true
void Tilter::End() {
        tilter->Up();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Tilter::Interrupted() {
}
