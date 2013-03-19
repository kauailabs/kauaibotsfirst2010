#include "AngleSensor.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"
#include "LiveWindow/LiveWindow.h"
#include <time.h>
#include "Synchronized.h"

const long AngleSensor::kReadDelayNanoseconds = 500;
static SEM_ID cSensorSemaphore = semBCreate (SEM_Q_PRIORITY, SEM_FULL);   

void AngleSensorNotifyFunc(void* param)
{
	AngleSensor *pSensorObj = (AngleSensor *)param;
	double angle = pSensorObj->GetAngleInternal();
	{
		//Synchronized sync(cSensorSemaphore);
		pSensorObj->cached_angle = angle;
	}
}

AngleSensor::AngleSensor(UINT8 dsc,UINT32 data_pin, UINT32 chipselect_pin, UINT32 clock_pin, double offset_angle) :
	data(dsc, data_pin),
	chip_select(dsc, chipselect_pin),
	clock(dsc, clock_pin),
	notifier(AngleSensorNotifyFunc,this)
{
	this->offset_angle = offset_angle;
	InitAngleSensor();
	notifier.StartPeriodic(.01);
}

/**
 * Initialize the AngleSensor.
 */
void AngleSensor::InitAngleSensor()
{
	chip_select.Set(1);
	clock.Set(1);
	nUsageReporting::report(nUsageReporting::kResourceType_DigitalOutput, chip_select.GetChannel(), dsc - 1);
	LiveWindow::GetInstance()->AddSensor("SwerveDriveSystem","Gyro", this);
}

/**
 * Delete the AngleSensor.
 */
AngleSensor::~AngleSensor()
{
	notifier.Stop();
}

float AngleSensor::GetAngle()
{
	//Synchronized sync(cSensorSemaphore);
	// Subtract the calibrated offset
	float adjusted_angle = cached_angle - offset_angle;
	// Once offset is subtracted, handle wrap-around.
	if ( adjusted_angle < -180.0 ) adjusted_angle += 360.0;
	if ( adjusted_angle > 180.0 ) adjusted_angle -= 360.0;
	return adjusted_angle;
}

/**
 * Return the actual angle in degrees.
 * 
 * The angle is based on the current sensor reading, which is a 12-bit value, giving a range
 * of 4096 digital values (0-4095) across a full-scale range of 360 degrees.
 * 
 * The value output from this function is normalized to a range of -180 to 180 degrees.
 * 
 * The sensor has a chip select and clock pin.  Chip select must be low in order to read data.
 * On the first rising clock edge, the first data bit is valid.
 * 
 * Per the datasheet, 500ns is the latency between when chipselect goes low and data is first valid,
 * and between each time the clock goes high and data is read to ready.
 * 
 * @return the current heading of the robot in degrees. This heading is based on integration
 * of the returned rate from the AngleSensor.
 */
float AngleSensor::GetAngleInternal( void )
{
	struct timespec ts;
	ts.tv_sec=0;
	ts.tv_sec=0;
	ts.tv_nsec=kReadDelayNanoseconds;
	
	//Synchronized sync(cSensorSemaphore);
	chip_select.Set(1);
	unsigned short angle=0;
	
	unsigned short bit;
	double real_Angle;
	
	clock.Set(1);
	chip_select.Set(0);
	nanosleep(&ts,NULL);

	for (int i=0;i<12;i++)
	{
		clock.Set(0);
		clock.Set(1);
		nanosleep(&ts,NULL);
		bit=data.Get();
		angle=angle<<1;
		angle|=bit;
	}
	
	chip_select.Set(1);
	clock.Set(1);
	
	real_Angle = ((double)angle*360)/4096;
	real_Angle = real_Angle-180;

	return real_Angle;
}

/**
 * Get the angle in degrees for the PIDSource base object.
 * 
 * @return The angle in degrees.
 */
double AngleSensor::PIDGet()
{
	return GetAngle();
}

void AngleSensor::UpdateTable() {
	if (m_table != NULL) {
		m_table->PutNumber("Value", GetAngle());
	}
}

void AngleSensor::StartLiveWindowMode() {
	
}

void AngleSensor::StopLiveWindowMode() {
	
}

std::string AngleSensor::GetSmartDashboardType() {
	return "Gyro";
}

void AngleSensor::InitTable(ITable *subTable) {
	m_table = subTable;
	UpdateTable();
}

ITable * AngleSensor::GetTable() {
	return m_table;
}
