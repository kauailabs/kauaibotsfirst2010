
#include "DashboardDataFormat.h"

/**
 * Default constructor.  
 */
DashboardDataFormat::DashboardDataFormat(void)
	: m_ds (DriverStation::GetInstance())
{
	
}

DashboardDataFormat::~DashboardDataFormat()
{
	
}

/**
 * Pack data using the correct types and in the correct order to match the
 * "Dashboard Datatype" in the LabVIEW Dashboard project.
 */
void DashboardDataFormat::PackAndSend(Joystick& stick1, Joystick& stick2, MecanumDrive& drive)
{
	Dashboard &dashboardPacker = m_ds->GetDashboardPacker();
	
	// Add data to describe the drive system state
	//
	// Add a cluster of Joystick data
	dashboardPacker.AddCluster();
	dashboardPacker.AddFloat(stick1.GetX());
	dashboardPacker.AddFloat(stick1.GetY());
	dashboardPacker.AddFloat(stick2.GetX());
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
	
	// Flush the data to the driver station.
	dashboardPacker.Finalize();
}
