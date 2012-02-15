/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef FILTEREDACCELEROMETER_H_
#define FILTEREDACCELEROMETER_H_

#include "AnalogChannel.h"
#include "SensorBase.h"
#include "PIDSource.h"

/** 
 * Handle operation of the accelerometer.
 * The accelerometer reads acceleration directly through the sensor. Many sensors have
 * multiple axis and can be treated as multiple devices. Each is calibrated by finding
 * the center value over a period of time.
 */
class FilteredAccelerometer : public SensorBase, public PIDSource 
{
public:
	explicit FilteredAccelerometer(UINT32 channel);
	FilteredAccelerometer(UINT32 slot, UINT32 channel);
	explicit FilteredAccelerometer(AnalogChannel *channel);
	virtual ~FilteredAccelerometer();
	
	float GetAcceleration();
	void SetSensitivity(float sensitivity);
	void SetZero(float zero);
	double PIDGet();

private:
	void InitAccelerometer();
	
	AnalogChannel *m_analogChannel;
	float m_voltsPerG;
	float m_zeroGVoltage;
	bool m_allocatedChannel;
};

#endif
