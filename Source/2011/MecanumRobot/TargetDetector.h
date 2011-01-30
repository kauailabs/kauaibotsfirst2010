#ifndef _TARGETDETECTOR_H 
#define _TARGETDETECTOR_H 
  
#include "WPILib.h" 
#include "KauaibotsTarget.h"
#include "CustomGyro.h"
  
class TargetDetector 
{ 
public: 
          
	TargetDetector( CustomGyro * pGyro, Servo * pHorizontalServo, Servo *pVerticalServo ); 
	~TargetDetector(); 
          
	void GetDetectedTargets( vector<Target>& detectedTargets, double& gyroAngle, double& targetAngle);
	void SetDetectedTargets( vector<Target>& detectedTargets, double gyroAngle, double targetAngle );
	void Enable( bool bEnable );
	bool IsEnabled();
	CustomGyro *Gyroscope() { return m_pGyro; }
	Servo *HorizontalServo() { return m_pHorizontalServo; }
	Servo *VerticalServo() { return m_pVerticalServo; }
	
protected:
	
	void InitDetectedTargets();
	
	Task m_task; 
	CustomGyro *m_pGyro;
	Servo *m_pHorizontalServo;
	Servo *m_pVerticalServo;
	vector<Target> m_LastDetectedTargets;
	double m_gyroAngle;
	double m_targetAngle;
}; 
  
#endif // _TARGETDETECTOR_H 
