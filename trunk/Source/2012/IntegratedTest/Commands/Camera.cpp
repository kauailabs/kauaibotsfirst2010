#include "Camera.h"

Camera::Camera() {
        // Use requires() here to declare subsystem dependencies
        Requires(camera);
}

// Called just before this Command runs the first time
void Camera::Initialize() {
        bCameraReady = false;
        m_cameraUp = false;
        m_lastButtonState = false;
}

// Called repeatedly when this Command is scheduled to run
void Camera::Execute() {
        // Digital Input 1 provides a way to 'capture' an image
        // which is written to the robot's flash file-system
        // This occurs when input 1 is toggled on then off
        if ( oi->getDriverStation()->GetDigitalIn(4) )
        {
                bCameraReady = true;
        }
        else
        {
                if ( bCameraReady )
                {
                        camera->WriteCurrentImage();
                        bCameraReady = false;
                }
        }
        
        // If Driver Station Input 2 is on, detect targets
        // Else if Driver Station Input 3 is on, detect hoops,
        // Else disable detection
        
        if ( oi->getDriverStation()->GetDigitalIn(2) )
        {
                camera->SetDetectionMode( CameraSubsystem::kDetectTargets );
        }
        else if ( oi->getDriverStation()->GetDigitalIn(3) )
        {
                camera->SetDetectionMode( CameraSubsystem::kDetectHoop );
        }
        else
        {
                camera->SetDetectionMode( CameraSubsystem::kDetectNone );
        }
        
        // If Driver Station Input 4 is on, up periscope
        // else down periscope
        Joystick*pjoystick;
        pjoystick = oi->getShooterJoystick();
        bool currentButton=pjoystick->GetRawButton(4);
        if(currentButton && !m_lastButtonState)
        {
        	m_cameraUp = !m_cameraUp;
       	}
       	m_lastButtonState = currentButton;
       	if(m_cameraUp)
       	{
       		camera->UpPeriscope();
        }
       	else
       	{
       		camera->DownPeriscope();
        }
        SmartDashboard*sd=SmartDashboard::GetInstance();
        sd->PutBoolean("Tilter Down",m_cameraUp);
        // Digital Input 5 on:  Chute angle of -10
        // Digital Input 6 on:  Chute angle of 10
        // else:                Chute angle of 0
        
        /*if ( oi->getDriverStation()->GetDigitalIn(5))
        {
                SmartDashboard::GetInstance()->PutDouble("ChuteAngle",-10.0);           
        }
        else if ( oi->getDriverStation()->GetDigitalIn(6))
        {
                SmartDashboard::GetInstance()->PutDouble("ChuteAngle",10.0);            
        }
        else
        {
                SmartDashboard::GetInstance()->PutDouble("ChuteAngle",0.0);
        }*/
}

// Make this return true when this Command no longer needs to run execute()
bool Camera::IsFinished() {
        return false;
}

// Called once after isFinished returns true
void Camera::End() {
        
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Camera::Interrupted() {
}
