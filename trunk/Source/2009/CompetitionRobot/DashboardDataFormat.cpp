
#include "DashboardDataFormat.h"

DashboardDataFormat::DashboardDataFormat(void)
	: m_ds (DriverStation::GetInstance())
{
	int i;
	int j;
	for (i=0;i<kAnalogModules;i++)
	{
		for(j=0;j<kAnalogChannels;j++)
		{
			m_AnalogChannels[i][j] = 0;
		}
	}
	for ( i=0;i<kDigitalModules;i++)
	{
		m_RelayFwd[i]=0;
		m_RelayRev[i]=0;
		m_DIOChannels[i]=0;
		m_DIOChannelsOutputEnable[i]=0;
		for ( j=0;j<kPwmChannels;j++)
		{
			m_PWMChannels[i][j]=0;
		}
	}
	m_SolenoidChannels=0;
	m_DriveWheelEncoder = 0;
	m_FreeWheelEncoder = 0;
}

DashboardDataFormat::~DashboardDataFormat()
{
	
}

/**
 * Pack data using the correct types and in the correct order to match the
 * default "Dashboard Datatype" in the LabVIEW Dashboard project.
 */
void DashboardDataFormat::PackAndSend(void)
{
	Dashboard &dashboardPacker = m_ds->GetDashboardPacker();
	UINT32 module;
	UINT32 channel;
	
	// Pack the analog modules
	for (module = 0; module < kAnalogModules; module++)
	{
		dashboardPacker.AddCluster();
		for (channel = 0; channel < kAnalogChannels; channel++)
		{
			dashboardPacker.AddFloat(m_AnalogChannels[module][channel]);
		}
		dashboardPacker.FinalizeCluster();
	}
	// Pack the digital modules
	for (module = 0; module < kDigitalModules; module++)
	{
		dashboardPacker.AddCluster();
		dashboardPacker.AddU8(m_RelayFwd[module]);
		dashboardPacker.AddU8(m_RelayRev[module]);
		dashboardPacker.AddU16(m_DIOChannels[module]);
		dashboardPacker.AddU16(m_DIOChannelsOutputEnable[module]);
		dashboardPacker.AddCluster();
		for(channel = 0; channel < kPwmChannels; channel++)
		{
			dashboardPacker.AddU8(m_PWMChannels[module][channel]);
		}
		dashboardPacker.FinalizeCluster();
		dashboardPacker.FinalizeCluster();
	}
	// Pack the solenoid module
	dashboardPacker.AddU8(m_SolenoidChannels);
	dashboardPacker.AddDouble(m_DriveWheelEncoder);
	dashboardPacker.AddDouble(m_FreeWheelEncoder);
	
	// Pack the detected targeting information
	for ( int i = 0; i < 3; i++ )
	{
		dashboardPacker.AddCluster();
		
		dashboardPacker.AddU8(m_DetectedTargets[i].m_bDetected);
		dashboardPacker.AddU8(m_DetectedTargets[i].m_bLockedOntoTarget);
		dashboardPacker.AddDouble(m_DetectedTargets[i].m_CenterOfMassX);
		dashboardPacker.AddDouble(m_DetectedTargets[i].m_CenterOfMassY);
		dashboardPacker.AddDouble(m_DetectedTargets[i].m_ParticleToImagePercent);
		
		dashboardPacker.FinalizeCluster();
	}
		
	// Flush the data to the driver station.
	dashboardPacker.Finalize();
}

void DashboardDataFormat::UpdateDashboard(	AnalogChannel 	*pFrontSteeringPot,
						AnalogChannel 	*pRearSteeringPot,
						AnalogChannel 	*pCannonAzimuthPot,
						Servo			*pCannonElevator,
						int				cannonElevatorChannel,
						DigitalInput	*pLimitSwitch,
						Encoder			*pDriveWheelEncoder,
						Encoder			*pFreeWheelEncoder)
{
	static float lastDriveWheelEncoderRate = 0;
	static float lastFreeWheelEncoderRate = 0;
	static float lastEncoderSampleTime = 0;
	
	// Update with Potentiometer values
	m_AnalogChannels[0][pFrontSteeringPot->GetChannel()-1] =
		pFrontSteeringPot->GetAverageVoltage();
	m_AnalogChannels[0][pRearSteeringPot->GetChannel()-1] =
		pRearSteeringPot->GetAverageVoltage();
	m_AnalogChannels[0][pCannonAzimuthPot->GetChannel()-1] =
		pCannonAzimuthPot->GetAverageVoltage();

	// Scale Servo value (0 - 1.0) to fit into a UINT8.
	float floatVal = pCannonElevator->Get() * 256;
	UINT uintVal = (UINT8)floatVal;
	m_PWMChannels[0][cannonElevatorChannel] = uintVal; 
	
	// Set limit switch value into appropriate Digital (Input) channel
	m_DIOChannels[0] = 
		pLimitSwitch->Get() ? 1 << (pLimitSwitch->GetChannel()-1) : 0;

	float currDriveWheelEncoderRate = (float)pDriveWheelEncoder->Get();
	float currFreeWheelEncoderRate = (float)pFreeWheelEncoder->Get();
	float currEncoderSampleTime = GetClock();
	
	float deltaDriveWheelEncoderRate = currDriveWheelEncoderRate - lastDriveWheelEncoderRate;
	float deltaFreeWheelEncoderRate = currFreeWheelEncoderRate - lastFreeWheelEncoderRate;
	
	// Acquire Encoder values, place into dashboard data structure
	m_DriveWheelEncoder = deltaDriveWheelEncoderRate;
	m_FreeWheelEncoder = deltaFreeWheelEncoderRate;			
		
	lastDriveWheelEncoderRate = currDriveWheelEncoderRate;
	lastFreeWheelEncoderRate = currFreeWheelEncoderRate;
	lastEncoderSampleTime = currEncoderSampleTime;
			
	PackAndSend();	
}

