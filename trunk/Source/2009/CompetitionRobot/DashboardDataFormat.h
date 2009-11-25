#ifndef __DashboardDataFormat_h__
#define __DashboardDataFormat_h__

#include "WPILib.h"

/**
 * DetectedTarget represents an object currently recognized by the 
 * camera imaging targeting system.
 * 
 */

class DetectedTarget
{
public:
	DetectedTarget()
	{
		m_bDetected = false;
		m_bLockedOntoTarget = false;
		m_CenterOfMassX = m_CenterOfMassY = m_ParticleToImagePercent = 0;
	}
	bool m_bDetected;
	bool m_bLockedOntoTarget;
	double m_CenterOfMassX;
	double m_CenterOfMassY;
	double m_ParticleToImagePercent;
};

/**
 * This class is just an example of one way you could organize the data that you want
 * to send to the dashboard.  The PackAndSend method does all the work.  You could
 * put the packing code directly in your code, but this model protects you from
 * packing data in the wrong order throughout your code.
 * 
 * The data and the format of this structure are just an example.  It is written to
 * match the initial data format expected by the LabVIEW Dashboard project.  Feel free
 * to add data elements or remove them.  Just remember to make any changes consistently
 * between the LabVIEW "Dashboard Datatype" and the data that gets packed by this class.
 */
class DashboardDataFormat : public SensorBase
{
public:
	DashboardDataFormat(void);
	virtual ~DashboardDataFormat();
	float m_AnalogChannels[kAnalogModules][kAnalogChannels];
	UINT8 m_RelayFwd[kDigitalModules];
	UINT8 m_RelayRev[kDigitalModules];
	UINT16 m_DIOChannels[kDigitalModules];
	UINT16 m_DIOChannelsOutputEnable[kDigitalModules];
	UINT8 m_PWMChannels[kDigitalModules][kPwmChannels];
	UINT8 m_SolenoidChannels;
	float m_DriveWheelEncoder;
	float m_FreeWheelEncoder;
	DetectedTarget m_DetectedTargets[3];
	void PackAndSend(void);
	void UpdateDashboard(	AnalogChannel 	*pFrontSteeringPot,
							AnalogChannel 	*pRearSteeringPot,
							AnalogChannel 	*pCannonAzimuthPot,
							Servo			*pCannonElevator,
							int				cannonElevatorChannel,
							DigitalInput	*pLimitSwitch,
							Encoder			*pDriveWheelEncoder,
							Encoder			*pFreeWheelEncoder);
	
private:
	DISALLOW_COPY_AND_ASSIGN(DashboardDataFormat);
	DriverStation *m_ds;
};


#endif // __DashboardDataFormat_h__
