#include "WPILib.h"
#include "DashboardDataFormat.h"
#include "TargetDetector.h"
#include "GameDrive.h"
#include "LogitechGamepad.h"

/**
 * This is a demo program showing the use of the Dashboard data packing class.
 */ 

static const UINT32 kOversampleBits = 7;
static const UINT32 kAverageBits = 3;
static const float kSamplesPerSecond = 50.0;
static const float kCalibrationSampleTime = 5.0;
static const float kDefaultVoltsPerDegreePerSecond = 0.0125;


class DashboardDataExample : public SimpleRobot
{
	GameDrive *m_pRobotDrive; // robot drive system
	Joystick *m_pCannonJoystick; // only joystick
	LogitechGamepad *m_pGamepad;
	DashboardDataFormat m_dashboardDataFormat;
	
	AnalogChannel * m_pCannonAzimuthPot;
	Servo		  * m_pCannonElevator;	
	DigitalInput  * m_pLimitSwitch;
	DriverStation * m_pDriverStation;
	TargetDetector m_TargetDetector;
	
	// Digital Sidecar Ports
	
	#define DRIVE_WHEEL_ENCODER_FORWARD 1
	#define DRIVE_WHEEL_ENCODER_REVERSE 2
	#define FREE_WHEEL_ENCODER_FORWARD  3
	#define FREE_WHEEL_ENCODER_REVERSE  4
	#define CANNON_LIMIT_SWITCH			5
	
	// Analog Breakout Ports
	
	#define FRONT_STEERING_POT 			1
	#define REAR_STEERING_POT 			2
	#define CANNON_AZIMUTH_POT			3
	
	// Digital Sidecar PWM Outputs
	
	#define CANNON_ELEVATION_SERVO		8
	
	// DriveStation Ports
	#define LOGITECH_GAMEPAD_USB_PORT	1
	
public:
	DashboardDataExample(void)
	{
		m_pGamepad = new LogitechGamepad(LOGITECH_GAMEPAD_USB_PORT);
		m_pRobotDrive = new GameDrive();
		
		//GetWatchdog().SetExpiration(100);
		GetWatchdog().SetEnabled(false);
	
		// TODO:  Set distance per encoder pulse
		
		m_pCannonAzimuthPot = new AnalogChannel(CANNON_AZIMUTH_POT);
		
		m_pCannonElevator = new Servo(CANNON_ELEVATION_SERVO);	
		m_pLimitSwitch = new DigitalInput(CANNON_LIMIT_SWITCH);
		
		m_pDriverStation = DriverStation::GetInstance();
		
		InitAnalogChannel (m_pCannonAzimuthPot);	
	}

	void InitAnalogChannel( AnalogChannel *m_analog )
	{
		m_analog->SetAverageBits(kAverageBits);
		m_analog->SetOversampleBits(kOversampleBits);
		float sampleRate = kSamplesPerSecond * 
			(1 << (kAverageBits + kOversampleBits));
		m_analog->GetModule()->SetSampleRate(sampleRate);
		Wait(1.0);
		m_analog->InitAccumulator();
		Wait(kCalibrationSampleTime);

		INT64 value;
		UINT32 count;
		m_analog->GetAccumulatorOutput(&value, &count);
		
		UINT32 center = (UINT32)((float)value / (float)count + .5);

		printf("Analog Channel Center:  %d\n");
		
		m_analog->SetAccumulatorCenter(center);
		
		printf("Analog Channel Deadband:  %d\n");
		
		m_analog->SetAccumulatorDeadband(0); ///< TODO: compute / parameterize this
		m_analog->ResetAccumulator();
	}
	
	void Autonomous()
	{
		INT32 i=0;
		GetWatchdog().SetEnabled(false);
		while(IsAutonomous())
		{
			Dashboard &dashboard = DriverStation::GetInstance()->GetDashboardPacker();
			
			dashboard.Printf("It's been %f seconds, according to the FPGA.\n", GetClock());
			dashboard.Printf("Iterations: %d\n", ++i);
			UpdateDashboard();
			Wait(0.02);
		}
	}
	
	void OperatorControl()
	{
		INT32 i=0;
		GetWatchdog().SetEnabled(false);
		while (IsOperatorControl())
		{
			Dashboard &dashboard = DriverStation::GetInstance()->GetDashboardPacker();

			dashboard.Printf("It's been %f seconds, according to the FPGA.\n", GetClock());
			dashboard.Printf("Iterations: %d\n", ++i);
			m_pRobotDrive->CustomDrive(m_pGamepad);
			UpdateDashboard();
			Wait(0.02);
		}
	}

	void UpdateDashboard(void)
	{
		m_dashboardDataFormat.m_DetectedTargets[0].m_bDetected = 
			m_TargetDetector.Detect( 
				m_dashboardDataFormat.m_DetectedTargets[0].m_CenterOfMassX,
				m_dashboardDataFormat.m_DetectedTargets[0].m_CenterOfMassY,
				m_dashboardDataFormat.m_DetectedTargets[0].m_ParticleToImagePercent);		
			
		m_dashboardDataFormat.UpdateDashboard(	m_pRobotDrive->FrontSteeringPot(),
												m_pRobotDrive->RearSteeringPot(),
												m_pCannonAzimuthPot,
												m_pCannonElevator,
												CANNON_ELEVATION_SERVO,
												m_pLimitSwitch,
												m_pRobotDrive->DriveWheelEncoder(),
												m_pRobotDrive->FreeWheelEncoder());
	}
};

START_ROBOT_CLASS(DashboardDataExample);

