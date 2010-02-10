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
	KauaibotsTarget(Target t, ColorImage *image, Servo *horizCameraServo, Servo *vertCameraServo);
	double GetRobotHorizontalAngle();
	double GetRobotVerticalAngle();
	double GetDistanceToTargetInches();  // Method using YPos
	double GetDistanceToTargetInches2(); // Method using Major Radius
	double GetDistanceToGoalInches();    // Method using YPos
protected:
	double GetHorizServoAngle();
	double GetVertServoAngle();
};

#endif // _KAUAIBOTSTARGET_H
