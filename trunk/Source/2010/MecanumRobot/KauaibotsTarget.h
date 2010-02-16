#ifndef _KAUAIBOTSTARGET_H
#define _KAUAIBOTSTARGET_H

#include "WPILib.h"
#include "Target.h"

class KauaibotsTarget : Target
{
protected:
	int m_nHeight;
	int m_nWidth;
	double m_nHorizPixelPitchMM;
	double m_nVertPixelPitchMM;
	Servo *m_horizCameraServo;
	Servo *m_vertCameraServo;
public:
	KauaibotsTarget(Target t, int imageHeight, int imageWidth, Servo *horizCameraServo, Servo *vertCameraServo);
	double GetRobotHorizontalAngle();	 // Horiz. Degrees from Robot Center to Target Center
	double GetRobotVerticalAngle();      // Vert. Degrees from Robot Center to Target Center
	double GetDistanceToTargetInches();  // Distance from Camera to Target Center (using vert angle)
	double GetDistanceToTargetInches2(); // Distance from Camera to Target Center (using target radius)
	double GetDistanceToGoalInches();    // Distance from Robot to Goal (using vert angle)
protected:
	double GetHorizServoAngle();		 // Servo Horiz. Angle offset (0 = no offset)
	double GetVertServoAngle();			 // Servo Vert. Angle offset (0= not offset)
};

#endif // _KAUAIBOTSTARGET_H
