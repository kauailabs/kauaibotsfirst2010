#include "CameraUp.h"

CameraUp::CameraUp(bool bUp) {
        // Use requires() here to declare subsystem dependencies
        Requires(camera);
        m_up = bUp;
}

// Called just before this Command runs the first time
void CameraUp::Initialize() {
}

// Called repeatedly when this Command is scheduled to run
void CameraUp::Execute() {
       	if(m_up)
       	{
       		camera->UpPeriscope();
        }
       	else
       	{
       		camera->DownPeriscope();
        }
}

// Make this return true when this Command no longer needs to run execute()
bool CameraUp::IsFinished() {
        return true;
}

// Called once after isFinished returns true
void CameraUp::End() {
   	if(m_up)
   	{
   		camera->UpPeriscope();
    }
   	else
   	{
   		camera->DownPeriscope();
    }
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CameraUp::Interrupted() {
}
