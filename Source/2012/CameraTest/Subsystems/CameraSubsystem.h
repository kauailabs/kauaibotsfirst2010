#ifndef CAMERA_SUBSYSTEM_H
#define CAMERA_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "../Thread.h"
#include <vector>
#include "Target.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class CameraSubsystem: public Subsystem, public Thread 
{
public:
    
    enum DetectionMode { kDetectTargets, kDetectHoop, kDetectNone };
    
	CameraSubsystem();
	void WriteCurrentImage();
	void SetDetectionMode( DetectionMode newMode );
	CameraSubsystem::DetectionMode GetDetectionMode();

	// Get the targets detected in the last camera processing period.
	// If true, at least one target was detected in the last camera processing period.
	
	bool GetLastDetectedTargets( Target& t1, Target& t2, Target& t3, Target& t4 );

	// Get the hoop detected in the last camera processing period.
	// If true, the hoop was detected in the last camera processing period.
	
	bool GetLastDetectedHoop( Hoop& h);
	
	// Camera Periscope Control
	
	void UpPeriscope();
	void DownPeriscope();
	bool IsPeriscopeUp();
	
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	bool m_bQuit;
	DetectionMode m_detectionMode;
	AxisCamera& camera;
	Solenoid cameraUp;
	Solenoid cameraDown;
	Target m_lastTarget[4];
	Hoop m_lastHoop;
	void GrabAndDetectTargets();
	void GrabAndDetectHoop();
    void UpdateNetworkTable( vector<Target>& targets, int iNumBestTargets );
    void UpdateNetworkTable( bool bFoundHoop, Hoop *pHoop );
	void InitDefaultCommand();
	void ExecLoop ();
	void CalculateDistance( Target& t );
	void CalculateTargetAngle(Target& t);
	void CalculateTargetHeight(Target& t);


};

#endif
