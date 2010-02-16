#include "KauaibotsTarget.h"
#include <math.h>

const double cCameraAngleOfViewDegrees = 47.0;		// TODO:  Measure this
const double cCameraLensHeightInches = 24.5;		// TODO:  Measure this
const double cTargetCenterHeightInches = 56.5;
const double cTargetOuterDiameterInches = 33.0;
const double cCameraFocalLengthMM = 4.0;
const double cCameraServoMinRange = 0;
const double cCameraServoMaxRange = 1;
const double cHorizCameraServoCenterValue = 0.5;	// TODO:  Measure this
const double cVertCameraServoCenterValue = 0.5;		// TODO:  Measure this
const double cHorizCameraServoRangeDegrees = 90;	// TODO:  Measure this
const double cVertCameraServoRangeDegrees = 80;		// TODO:  Measure this

double DegreesToRadians( double dDegrees )
{
	return (3.1415926 * dDegrees / 180);
}

double RadiansToDegrees( double dRadians )
{
	return (dRadians * 180.0 / 3.1415926);	
}

KauaibotsTarget::KauaibotsTarget(Target t, int imageHeight, int imageWidth, Servo* horizCameraServo, Servo* vertCameraServo)
{
    m_majorRadius = t.m_majorRadius;
    m_minorRadius = t.m_minorRadius;
    m_rawScore = t.m_rawScore;
    m_xPos = t.m_xPos;
    m_yPos = t.m_yPos;
    m_score = t.m_score;
    m_rotation = t.m_rotation;
    m_xMax = t.m_xMax;
    m_bothFound = t.m_bothFound;
	m_nWidth = imageWidth;
	m_nHeight = imageHeight;

	double dSensorSizeMM = tan(DegreesToRadians(cCameraAngleOfViewDegrees / 2)) * cCameraFocalLengthMM * 2;	

	m_nHorizPixelPitchMM = dSensorSizeMM / m_nWidth;
	m_nVertPixelPitchMM = dSensorSizeMM / m_nHeight;

	m_horizCameraServo = horizCameraServo;
	m_vertCameraServo = vertCameraServo;
}

double KauaibotsTarget::GetRobotHorizontalAngle()
{
	double xPixelPos = ((m_xPos*m_nHeight)+m_nWidth)/2;
	
	double horizAngleRadians = 
		atan((((xPixelPos)-m_nWidth/2)*m_nHorizPixelPitchMM)/cCameraFocalLengthMM);
	
	return RadiansToDegrees( horizAngleRadians ) - GetHorizServoAngle();
}

double KauaibotsTarget::GetRobotVerticalAngle()
{
	double yPixelPos = ((m_yPos*m_nWidth)+m_nWidth)/2;

	double vertAngleRadians = 
		atan((((yPixelPos)-m_nHeight/2)*m_nVertPixelPitchMM)/cCameraFocalLengthMM);
	
	return RadiansToDegrees( vertAngleRadians ) - GetVertServoAngle();
}

double KauaibotsTarget::GetHorizServoAngle()
{
	double dCurrServoSetting = m_horizCameraServo->Get();
	double dServoAngle = (dCurrServoSetting / (cCameraServoMaxRange-cCameraServoMinRange) * cHorizCameraServoRangeDegrees) - (cHorizCameraServoCenterValue *cHorizCameraServoRangeDegrees);	
	return dServoAngle;
}

double KauaibotsTarget::GetVertServoAngle()
{
	double dCurrServoSetting = m_vertCameraServo->Get();
	double dServoAngle = (dCurrServoSetting / (cCameraServoMaxRange-cCameraServoMinRange) * cVertCameraServoRangeDegrees) - (cVertCameraServoCenterValue *cVertCameraServoRangeDegrees);	
	return dServoAngle;
}

// Method using yPos
double KauaibotsTarget::GetDistanceToTargetInches()
{
	double dDistanceInches = ( (cTargetCenterHeightInches - cCameraLensHeightInches) /
		 sin( DegreesToRadians ( fabs ( GetRobotVerticalAngle())))) / 12;
	
	return dDistanceInches;
}

// Method using yPos
double KauaibotsTarget::GetDistanceToGoalInches()
{
	double dDistanceInches = ( (cTargetCenterHeightInches - cCameraLensHeightInches) /
		 tan( DegreesToRadians ( fabs ( GetRobotVerticalAngle())))) / 12;
	
	return dDistanceInches;
}

// Method using Major Radius
double KauaibotsTarget::GetDistanceToTargetInches2()
{
	double dDistance = ((1/((m_nVertPixelPitchMM*(m_majorRadius*2))/cCameraFocalLengthMM))*cTargetOuterDiameterInches)/12;
	
	return dDistance;
}

