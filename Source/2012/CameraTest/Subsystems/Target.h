#ifndef TARGET_H
#define TARGET_H

// Camera Metrics
static const int cCameraImagePixelHeight = 240;
static const int cCameraImagePixelWidth = 320;
static double cCameraHorizontalFieldOfViewDegrees = 47;
static double cCameraVerticalFieldOfViewDegrees = cCameraHorizontalFieldOfViewDegrees * (cCameraImagePixelHeight/cCameraImagePixelWidth);

// Absolute Measurements from Arena Specifications

static const int cLowHoopHeightInches 		= 28;
static const int cMiddleHoopHeightInches 	= 61;
static const int cTopHoopHeightInches 		= 98;
		
static const int cTargetWidthInches 	= 24;	// Measured between outer edges of tape.
static const int cTargetHeightInches 	= 18;	// Measured between outer edges of tape.
static const int cTapeWidthInches 		=  2;
static const int cTapeHeightInches		=  2;
static const int cNumTargets			=  4;
static const int cHoopDiameterInches	= 18;

static const int cLowTargetBottomHeightInches = cLowHoopHeightInches + cTapeHeightInches;
static const int cMiddleTargetBottomHeightInches = cMiddleHoopHeightInches + cTapeHeightInches;
static const int cTopTargetBottomHeightInches = cTopHoopHeightInches + cTapeHeightInches;

class DetectedObject
{
public:
	bool		m_bDetected;
	DetectedObject()
	{
		m_bDetected = false;
	}
};

class Target : public DetectedObject
{
public:
	// Scoring and basic metrics
	double		m_RectangularScore;
	double		m_AspectRatioScore;
	double		m_RowEdgeScore;
	double		m_ColEdgeScore;
	int			m_Left;
	int			m_Top;
	int			m_Width;
	int			m_Height;
	
	// Important Calculations
	double		m_DistanceInches;
	double		m_ElevationFromFloorInches;
	double		m_AngleFromRobotCenterDegrees;	

	Target()
	{
		m_RectangularScore = 
			m_AspectRatioScore =
			m_RowEdgeScore =
			m_ColEdgeScore = 0.0;
		m_Left =
			m_Top =
			m_Width =
			m_Height = 0;
		m_DistanceInches = 
			m_ElevationFromFloorInches =
			m_AngleFromRobotCenterDegrees = 0.0;
	}
	
	double TotalScore() const
	{
		return m_RectangularScore + m_AspectRatioScore + m_RowEdgeScore + m_ColEdgeScore;
	}
	
	bool IsLowTarget() { return (m_ElevationFromFloorInches == cLowTargetBottomHeightInches); }
	bool IsMiddleTarget() { return (m_ElevationFromFloorInches == cMiddleTargetBottomHeightInches); }
	bool IsTopTarget() { return (m_ElevationFromFloorInches == cTopTargetBottomHeightInches); }
	
	float GetCenterAsJoystickValue()
	{
		int targetCenterPixelX = m_Left + (m_Width / 2);
		int imageCenterPixelX = cCameraImagePixelWidth/2;
		float joystickValue = float(-1.0) + (float(targetCenterPixelX) / float(imageCenterPixelX));
		return joystickValue;
	}
	
	friend bool operator<(const Target& left, const Target& right);
};

class Hoop : public DetectedObject
{
public:
	int		m_CenterOfMassX;
	int		m_CenterOfMassY;
	Hoop()
	{
		m_CenterOfMassX =
			m_CenterOfMassY = 0;
	}
};

#endif
