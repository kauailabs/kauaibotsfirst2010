/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef PROXIMITYSENSOR_H_
#define PROXIMITYSENSOR_H_

#include "SensorBase.h"
#include "PIDSource.h"
#include "LiveWindow/LiveWindowSendable.h"
#include "AnalogChannel.h"
/**
 * Use the angle sensor to read an absolute angle measurement.
 * 
 * This utilizes the Avagotech AEAT-6012-A06 12-bit Absolute Magnetic Encoder.
 * 
 * This sensor requires two DigitalOutput pin (Chip Select, Clock)
 * 
 * This sensor also requires one DigitalInput pin (Data)
 */

class ProximitySensor : public SensorBase, public PIDSource, public LiveWindowSendable
{
public:
	
	enum SensorRange { kShortRange, kMediumRange, kLongRange };
	
	ProximitySensor(UINT8 analog_module,UINT32 analog_channel, SensorRange sensor_range);
	virtual ~ProximitySensor();
	virtual float GetDistanceMM();

	// PIDSource interface
	double PIDGet();
	
	void UpdateTable();
	void StartLiveWindowMode();
	void StopLiveWindowMode();
	std::string GetSmartDashboardType();
	void InitTable(ITable *subTable);
	ITable * GetTable();

private:
	void InitProximitySensor();
	
	AnalogChannel input;
	SensorRange range;
	
	ITable *m_table;
};
#endif
