
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

static SEM_ID cIMUStateSemaphore = semBCreate (SEM_Q_PRIORITY, SEM_FULL);   
static int update_count = 0;

/*** Internal task.
 * 
 * Task which retrieves yaw/pitch/roll updates from the IMU, via the
 * SerialPort.
 **/ 

static char protocol_buffer[256];

#define YAW_PITCH_ROLL_UPDATE_PACKET_LENGTH 27 	// e.g., !y[yaw][pitch][roll][checksum][cr][lf]
												//       where yaw, pitch, roll are floats
												//		 where checksum is 2 ascii-bytes of HEX checksum (all bytes before checksum)
#define YAW_PITCH_ROLL_UPDATE 'y'
#define PACKET_START_CHAR '!'

float parse_float( char *buffer )
{
  char temp[8];
  for ( int i = 0; i < 7; i++ )
  {
    temp[i] = buffer[i];
  }
  temp[7] = 0;
  return atof(temp);
}

bool decode_ypr_update( char *buffer, int length, float* pyaw, float *ppitch, float *proll )
{
  bool ok = false;
  if ( length < YAW_PITCH_ROLL_UPDATE_PACKET_LENGTH ) return false;
  if ( ( buffer[0] == '!' ) && ( buffer[1] == 'y' ) )
  {
    // Calculate Checksum
    uint8_t checksum = 0;
    for ( int i = 0; i < 23; i++ )
    {
      checksum += buffer[i];
    }

    // Decode Checksum
    
    uint8_t first_digit = buffer[23] <= '9' ? buffer[23] - '0' : ((buffer[23] - 'A') + 10);
    uint8_t second_digit = buffer[24] <= '9' ? buffer[24] - '0' : ((buffer[24] - 'A') + 10);
    uint8_t decoded_checksum = (first_digit * 16) + second_digit;

    if ( decoded_checksum != checksum ) return false;

    *pyaw = parse_float( &buffer[2] );
    *ppitch = parse_float( &buffer[9] );
    *proll = parse_float( &buffer[16] );
    
    ok = true;
  }
  return ok;
}

static void imuTask(IMU *imu) 
{
	SerialPort *pport = imu->GetSerialPort();
	pport->SetReadBufferSize(512);
	pport->SetTimeout(2.0);
	pport->EnableTermination('\n');
	
	while (1)
	{ 
		UINT32 bytes_read = pport->Read( protocol_buffer, sizeof(protocol_buffer));
		if ( bytes_read > 0 )
		{
			// Scan the buffer until the "begin packet" character is found
			for ( UINT32 i = 0; i < bytes_read; i++ )
			{
				if ( protocol_buffer[i] == PACKET_START_CHAR )
				{
					char *ppacket = &protocol_buffer[i];
					int num_bytes_remaining = bytes_read - i;
					if ( num_bytes_remaining > 1 )
					{
						switch ( ppacket[1] )
						{
						case YAW_PITCH_ROLL_UPDATE:
							{					
								if ( num_bytes_remaining >= YAW_PITCH_ROLL_UPDATE_PACKET_LENGTH )
								{
									float yaw = 0.0;
									float pitch = 0.0;
									float roll = 0.0;
									if ( decode_ypr_update( ppacket, YAW_PITCH_ROLL_UPDATE_PACKET_LENGTH, &yaw, &pitch, &roll))
									{
										update_count++;
										imu->SetYawPitchRoll(yaw,pitch,roll);
									}
									i += YAW_PITCH_ROLL_UPDATE_PACKET_LENGTH;
								}
							}
							break;
						default:	// unknown packet
							break;
						}
					}
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
