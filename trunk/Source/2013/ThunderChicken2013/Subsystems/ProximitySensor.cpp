#include "ProximitySensor.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"
#include "LiveWindow/LiveWindow.h"
#include <time.h>
#include <math.h>
#include "AnalogModule.h"

ProximitySensor::ProximitySensor(UINT8 analog_module,UINT32 analog_channel, SensorRange range) :
	input(analog_module, analog_channel)
{
	this->range = range;
	InitProximitySensor();
}

/**
 * Initialize the ProximitySensor.
 */
void ProximitySensor::InitProximitySensor()
{
	// Be careful not to set the average bits more than once.
	UINT32 num_average_bits = 4;
	if ( input.GetAverageBits() != num_average_bits )
	{
		input.SetAverageBits(num_average_bits);	// 2^4 = 16-sample average
		float sampleRate = 50.0 * 
			(1 << (num_average_bits));
		input.GetModule()->SetSampleRate(sampleRate);
		Wait(0.2);
	}
	LiveWindow::GetInstance()->AddSensor("Tilter","Ultrasonic", this);
}

/**
 * Delete the ProximitySensor.
 */
ProximitySensor::~ProximitySensor()
{
}

/**
 * Return the actual distance in millimeters.
 * 
 * @return the current distance in millimeters.
 */

float ProximitySensor::GetDistanceMM( void )
{
	float voltage = input.GetAverageVoltage();
	
	// Transform voltage to distance.  Exponential values
	// were derived from the datasheets:
	//
	// GP2D120XJ00F (Short Range [30cm]):  https://www.sparkfun.com/datasheets/Sensors/Infrared/GP2D120XJ00F_SS.pdf
	// GP2Y0A21YK (Medium Range [80cm]):  http://www.sparkfun.com/datasheets/Components/GP2Y0A21YK.pdf
	// GP2Y0A02YK0F (Long Range [150cm]):  https://www.sparkfun.com/datasheets/Sensors/Infrared/gp2y0a02yk_e.pdf
	
	float distance_mm;
	switch ( range )
	{
	case kShortRange:
	
		distance_mm = 126.27 * pow(voltage, -1.102);
		break;
		
	case kMediumRange:
		distance_mm = 267.1 * pow(voltage, -1.23);
		break;
		
	case kLongRange:
		distance_mm = 0.0;	// TODO
		break;
	}
	
	return distance_mm;
}

/**
 * Get the angle in degrees for the PIDSource base object.
 * 
 * @return The angle in degrees.
 */
double ProximitySensor::PIDGet()
{
	return GetDistanceMM();
}

void ProximitySensor::UpdateTable() {
	if (m_table != NULL) {
		m_table->PutNumber("Value", GetDistanceMM());
	}
}

void ProximitySensor::StartLiveWindowMode() {
	
}

void ProximitySensor::StopLiveWindowMode() {
	
}

std::string ProximitySensor::GetSmartDashboardType() {
	return "Ultrasonic";
}

void ProximitySensor::InitTable(ITable *subTable) {
	m_table = subTable;
	UpdateTable();
}

ITable * ProximitySensor::GetTable() {
	return m_table;
}
