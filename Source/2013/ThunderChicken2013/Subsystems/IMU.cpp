
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"
#include "LiveWindow/LiveWindow.h"
#include <time.h>
#include "IMU.h"

IMU::IMU( SerialPort *pport )
{
	pserial_port = pport;
	InitIMU();
}

/**
 * Initialize the IMU.
 */
void IMU::InitIMU()
{
	LiveWindow::GetInstance()->AddSensor("SwerveDriveSystem","Gyro", this);
}

/**
 * Delete the IMU.
 */
IMU::~IMU()
{
}

/**
 * Return the yaw angle in degrees.
 * 
 * This angle increases as the robot spins to the right.
 * 
 * This angle ranges from -180 to 180 degrees.
 */
float IMU::GetYaw( void )
{
	return 0;
}

float IMU::GetPitch( void )
{
	return 0;
}

float IMU::GetRoll( void )
{
	return 0;
}

/**
 * Get the angle in degrees for the PIDSource base object.
 * 
 * @return The angle in degrees.
 */
double IMU::PIDGet()
{
	return GetYaw();
}

void IMU::UpdateTable() {
	if (m_table != NULL) {
		m_table->PutNumber("Value", GetYaw());
	}
}

void IMU::StartLiveWindowMode() {
	
}

void IMU::StopLiveWindowMode() {
	
}

std::string IMU::GetSmartDashboardType() {
	return "Gyro";
}

void IMU::InitTable(ITable *subTable) {
	m_table = subTable;
	UpdateTable();
}

ITable * IMU::GetTable() {
	return m_table;
}
