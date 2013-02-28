
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"
#include "LiveWindow/LiveWindow.h"
#include <time.h>
#include "IMU.h"
#include "IMUProtocol.h"

// TODO:
//
// Include a "is connected" method.
// Add method to "zero" the yaw component.

static SEM_ID cIMUStateSemaphore = semBCreate (SEM_Q_PRIORITY, SEM_FULL);   
static int update_count = 0;

/*** Internal task.
 * 
 * Task which retrieves yaw/pitch/roll updates from the IMU, via the
 * SerialPort.
 **/ 

static char protocol_buffer[256];
static void imuTask(IMU *imu) 
{
	SerialPort *pport = imu->GetSerialPort();
	pport->SetReadBufferSize(512);
	pport->SetTimeout(2.0);
	pport->EnableTermination('\n');

	float yaw = 0.0;
	float pitch = 0.0;
	float roll = 0.0;	
	
	while (1)
	{ 
		UINT32 bytes_read = pport->Read( protocol_buffer, sizeof(protocol_buffer) );
		if ( bytes_read > 0 )
		{
			UINT32 i = 0;
			// Scan the buffer looking for valid packets
			while ( i < bytes_read )
			{
				int bytes_remaining = bytes_read - i;
				int packet_length = IMUProtocol::decodeYPRUpdate( &protocol_buffer[i], bytes_remaining, yaw, pitch, roll ); 
				if ( packet_length > 0 )
				{
					update_count++;
					imu->SetYawPitchRoll(yaw,pitch,roll);
					i += packet_length;
				}
				else // current index is not the start of a valid packet; increment
				{
					i++;
				}
			}
		}
	}
}

IMU::IMU( SerialPort *pport ) :
	m_task ("IMU", (FUNCPTR)imuTask,Task::kDefaultPriority+1)  
{
	yaw = 0.0;
	pitch = 0.0;
	roll = 0.0;
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
	Synchronized sync(cIMUStateSemaphore);
	return this->yaw;
}

float IMU::GetPitch( void )
{
	Synchronized sync(cIMUStateSemaphore);
	return this->pitch;
}

float IMU::GetRoll( void )
{
	Synchronized sync(cIMUStateSemaphore);
	return this->roll;
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

void IMU::SetYawPitchRoll(float yaw, float pitch, float roll)
{
	Synchronized sync(cIMUStateSemaphore);
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
}
