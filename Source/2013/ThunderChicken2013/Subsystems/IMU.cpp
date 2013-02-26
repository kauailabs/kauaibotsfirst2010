
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"
#include "LiveWindow/LiveWindow.h"
#include <time.h>
#include "IMU.h"

// TODO:
//
// Create thread to receive data from the serial port.
// Develop protocol to communicate the yaw, pitch and roll terms.
// Update semaphore-protected yaw, pitch, and roll terms w/decoded IMU data.
// Include a "is connected" method.
// Add method to "zero" the yaw component.

IMU::IMU( SerialPort *pport )
{
	pserial_port = pport;
	pserial_port->Reset();
	InitIMU();
}

/**
 * Initialize the IMU.
 */
void IMU::InitIMU()
{
	// The IMU serial port configuration is 8 data bits, no parity, one stop bit. 
	// No flow control is used.
	// Conveniently, these are the defaults used by the WPILib's SerialPort class.
	//
	// In addition, the WPILib's SerialPort class also defaults to:
	//
	// Timeout period of 5 seconds
	// Termination ('\n' character)
	// Transmit immediately
	
	LiveWindow::GetInstance()->AddSensor("SwerveDriveSystem","Gyro", this);
}

/**
 * Delete the IMU.
 */
IMU::~IMU()
{
}

bool IMU::IsConnected()
{
	return false;
}

void ZeroYaw()
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
