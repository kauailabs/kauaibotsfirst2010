
#include "DashboardDataFormat.h"
#include <vector>

/**
 * Default constructor.  
 */
DashboardDataFormat::DashboardDataFormat(void)
	: m_ds (DriverStation::GetInstance())
{
	// This timer makes sure that the data is not sent to the dashboard more
	// than 10 times per second for efficiency.
	visionTimer = new Timer();
	visionTimer->Start();	
}

DashboardDataFormat::~DashboardDataFormat()
{
	
}

/**
 * Pack data using the correct types and in the correct order to match the
 * "Dashboard Datatype" in the LabVIEW Dashboard project.
 */
void DashboardDataFormat::PackAndSend(Joystick& stick1, MecanumDrive& drive,Kicker& kicker, Tensioner& tensioner)
{
	Dashboard &dashboardPacker = m_ds->GetLowPriorityDashboardPacker();
	
	// Add data to describe the drive system state
	//
	dashboardPacker.AddCluster();
	// Add a cluster of Joystick data
	dashboardPacker.AddCluster();
	dashboardPacker.AddFloat(stick1.GetX());
	dashboardPacker.AddFloat(stick1.GetY());
	dashboardPacker.AddFloat(stick1.GetTwist() * -1);
	dashboardPacker.FinalizeCluster();

	// Add a cluster of motor output speed data
	dashboardPacker.AddCluster();
	dashboardPacker.AddFloat(drive.FrontLeftMotor().Get());
	dashboardPacker.AddFloat(drive.FrontRightMotor().Get());
	dashboardPacker.AddFloat(drive.RearLeftMotor().Get());
	dashboardPacker.AddFloat(drive.RearRightMotor().Get());
	dashboardPacker.FinalizeCluster();

	// Add a cluster of wheel encoder input speed data
	dashboardPacker.AddCluster();
	dashboardPacker.AddFloat(drive.FrontLeftEncoder().Get());
	dashboardPacker.AddFloat(drive.FrontRightEncoder().Get());
	dashboardPacker.AddFloat(drive.RearLeftEncoder().Get());
	dashboardPacker.AddFloat(drive.RearRightEncoder().Get());
	dashboardPacker.FinalizeCluster();
	
	// Add a cluster of inertial navigation sensor data
	
	dashboardPacker.AddCluster();
	dashboardPacker.AddFloat(drive.AccelerometerX().GetAcceleration());
	dashboardPacker.AddFloat(drive.AccelerometerY().GetAcceleration());	
	dashboardPacker.AddFloat(drive.Gyroscope().GetAngle());
	dashboardPacker.FinalizeCluster();

	// Add a cluster of winch status data
	dashboardPacker.AddCluster();
	
	float fireCompleteDetectorValue; 
	bool fireCompleteDetected;
	float ballDetectorValue;
	bool ballDetected;
	bool winchLoaded;
	bool autoLoad;
	bool autoFire;
	
	Kicker::KickerMotorState kickerState;
	kicker.GetStatus(&fireCompleteDetectorValue,&fireCompleteDetected,&ballDetectorValue,&ballDetected,&winchLoaded,&kickerState,&autoLoad,&autoFire);
	
	dashboardPacker.AddBoolean(false); // Auto-load
	dashboardPacker.AddBoolean(false); // Auto-fire
	dashboardPacker.AddBoolean(kickerState==Kicker::Forward); // Winch Fwd
	dashboardPacker.AddBoolean(kickerState==Kicker::Reverse); // Winch Rev
	dashboardPacker.AddBoolean(winchLoaded); // Loaded Sensor
	dashboardPacker.AddBoolean(fireCompleteDetected); // Kicker Complete Sensor
	dashboardPacker.AddFloat(fireCompleteDetectorValue); // Kicker Complete Voltage
	dashboardPacker.AddBoolean(ballDetected); // Ball Detected
	dashboardPacker.AddFloat(ballDetectorValue); // Ball Detected Voltage
	dashboardPacker.FinalizeCluster();
	
	// Add a cluster of tensioner status data
	dashboardPacker.AddCluster();
	
	float tensionerPotentiometerVoltage;
	float tensionerPotentiometerDistanceInFeet;
	Tensioner::TensionerMotorState tensionerState;
	
	tensioner.GetStatus(&tensionerPotentiometerVoltage,&tensionerPotentiometerDistanceInFeet,&tensionerState);
	
	dashboardPacker.AddBoolean(tensionerState==Tensioner::Forward); // Tensioner Fwd
	dashboardPacker.AddBoolean(tensionerState==Tensioner::Reverse); // Tensioner Rev
	dashboardPacker.AddFloat(tensionerPotentiometerVoltage); // Tensioner Voltage	
	dashboardPacker.AddFloat(tensionerPotentiometerDistanceInFeet); // Tensioner Distance
	dashboardPacker.FinalizeCluster();
	
	// Finalize the entire cluster of drive system data
	dashboardPacker.FinalizeCluster();	
	
	// Flush the data to the driver station.
	dashboardPacker.Finalize();
}

/**
 * Send the vision tracking data.
 * Sends the vision information to the dashboard so that the images will be annotated
 * and the graphs will operate.
 */
void DashboardDataFormat::sendVisionData(double joyStickX,
					double gyroAngle,
					double gyroRate,
					double targetX,
					vector<Target> targets) {
	if (visionTimer->Get() < 0.1)
		return;
	visionTimer->Reset();
	Dashboard &dash = DriverStation::GetInstance()->GetHighPriorityDashboardPacker();
	dash.AddCluster(); // wire (2 elements)
	{
		dash.AddCluster(); // tracking data
		{
			dash.AddDouble(joyStickX); // Joystick X
			dash.AddDouble(((((int)gyroAngle) + 360 + 180) % 360) - 180.0); // angle
			dash.AddDouble(0.0); // angular rate
			dash.AddDouble(targetX); // other X
		}
		dash.FinalizeCluster();
		dash.AddCluster(); // target Info (2 elements)
		{
			dash.AddArray(); // targets
			{
                for (unsigned i = 0; i < targets.size(); i++) {
                    dash.AddCluster(); // targets
                    {
                        dash.AddDouble(targets[i].m_score); // target score
                        dash.AddCluster(); // Circle Description (5 elements)
                        {
                            dash.AddCluster(); // Position (2 elements)
                            {
                                dash.AddFloat((float) (targets[i].m_xPos / targets[i].m_xMax)); // X
                                dash.AddFloat((float) targets[i].m_yPos); // Y
                            }
                            dash.FinalizeCluster();

                            dash.AddDouble(targets[i].m_rotation); // Angle
                            dash.AddDouble(targets[i].m_majorRadius); // Major Radius
                            dash.AddDouble(targets[i].m_minorRadius); // Minor Radius
                            dash.AddDouble(targets[i].m_rawScore); // Raw score
                            }
                        dash.FinalizeCluster(); // Position
                        }
                    dash.FinalizeCluster(); // targets
                    }
			}
			dash.FinalizeArray();
			
			dash.AddU32((int) 0);

		}
		dash.FinalizeCluster(); // target Info
	}
	dash.FinalizeCluster(); // wire
	dash.Finalize();
}

