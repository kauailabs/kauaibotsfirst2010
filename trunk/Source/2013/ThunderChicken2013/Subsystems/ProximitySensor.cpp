#include "ProximitySensor.h"
#include "NetworkCommunication/UsageReporting.h"
#include "Timer.h"
#include "WPIErrors.h"
#include "LiveWindow/LiveWindow.h"
#include <time.h>

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
 * @return the current distance in millimetersheading of the robot in degrees. This distance is
 * based on calculation of the returned rate from the ProximitySensor.
 */

float ProximitySensor::GetDistanceMM( void )
{
	float distance_mm = 0.0;

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
	return "ProximitySensor";
}

void ProximitySensor::InitTable(ITable *subTable) {
	m_table = subTable;
	UpdateTable();
}

ITable * ProximitySensor::GetTable() {
	return m_table;
}
