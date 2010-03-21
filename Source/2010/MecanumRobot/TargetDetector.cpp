#include "TargetDetector.h"

#define MINIMUM_SCORE 0.01


// Create Mutex for Target Data
static SEM_ID cDetectorStateSemaphore = semBCreate (SEM_Q_PRIORITY, SEM_FULL);

/**
 * Internal task.
 * 
 * Task which checks the compressor pressure switch and operates the relay as necessary
 * depending on the pressure.
 * 
 * Do not call this function directly.
 */
static void detectorTask(TargetDetector *detector)
{
	Wait(5); // Wait a few seconds to ensure system initialization is complete.
	
	AxisCamera& camera = AxisCamera::GetInstance();

	while (1)
	{	
		if ( camera.IsFreshImage() ) 
		{
			//printf("Got a fresh image.\n");
			// get the gyro heading that goes with this image
			double gyroAngle = detector->Gyroscope()->PIDGet();
			double targetAngle = 0;
			
			// get the camera image
			ColorImage *image = camera.GetImage();
			
			Wait(0.005);	// Yield for a short while
			
			int imageHeight = image->GetHeight();
			int imageWidth = image->GetWidth();

			// find FRC targets in the image
			vector<Target> targets = Target::FindCircularTargets(image);
			delete image;  // Be sure to delete the image!
			
			if (targets.size() == 0 || targets[0].m_score < MINIMUM_SCORE)
			{
				// no targets found. Make sure the first one in the list is 0,0
				// since the dashboard program annotates the first target in green
				// and the others in magenta. With no qualified targets, they'll all
				// be magenta.
				Target nullTarget;
				nullTarget.m_majorRadius = 0.0;
				nullTarget.m_minorRadius = 0.0;
				nullTarget.m_score = 0.0;
				if (targets.size() == 0) 
					targets.push_back(nullTarget);
				else
					targets.insert(targets.begin(), nullTarget);
			}
			else
			{
				KauaibotsTarget smartTargeter(targets[0],imageHeight,imageWidth,detector->HorizontalServo(),detector->VerticalServo());									

				targetAngle = smartTargeter.GetRobotHorizontalAngle();
				//printf("Found target at angle %f.\n", targetAngle);
			}
			
			detector->SetDetectedTargets(targets, gyroAngle, targetAngle);
		}		
		
		Wait(0.02);	// Yield for awhile....
	}
}

TargetDetector::TargetDetector( CustomGyro * pGyro,
		Servo *pHorizontalServo,
		Servo *pVerticalServo ) :
			m_task ("TargetDetector", (FUNCPTR)detectorTask, Task::kDefaultPriority+2)
{
	m_pGyro = pGyro;
	m_pHorizontalServo = pHorizontalServo;
	m_pVerticalServo = pVerticalServo;
	
	InitDetectedTargets();	
	
	if (!m_task.Start((INT32)this))
	{
		wpi_fatal(CompressorTaskError);
	}
}

TargetDetector::~TargetDetector()
{
	// Shut down the task
	m_task.Stop();
}

void TargetDetector::GetDetectedTargets( vector<Target>& detectedTargets, double& gyroAngle, double& targetAngle )
{
	{
		Synchronized sync(cDetectorStateSemaphore);	
		detectedTargets = m_LastDetectedTargets;
		gyroAngle = m_gyroAngle;
		targetAngle = m_targetAngle;
	}
}

void TargetDetector::SetDetectedTargets( vector<Target>& detectedTargets, double gyroAngle, double targetAngle)
{
	{
		Synchronized sync(cDetectorStateSemaphore);		
		
		m_LastDetectedTargets = detectedTargets;
		m_gyroAngle = gyroAngle;
		m_targetAngle = targetAngle;
	}	
}

void TargetDetector::InitDetectedTargets()
{
	vector<Target> defaultTargets;
	
	Target nullTarget;
	nullTarget.m_majorRadius = 0.0;
	nullTarget.m_minorRadius = 0.0;
	nullTarget.m_score = 0.0;
	defaultTargets.push_back(nullTarget);	
	
	SetDetectedTargets( defaultTargets, 0, 0 );
}

void TargetDetector::Enable( bool bEnable )
{
	if ( !IsEnabled() && bEnable )
	{
		InitDetectedTargets();
	}
	bEnable ? m_task.Suspend() : m_task.Resume();
}

bool TargetDetector::IsEnabled()
{	
	return ( !m_task.IsSuspended() && m_task.IsReady() );
}
