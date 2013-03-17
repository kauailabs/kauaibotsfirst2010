/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef ANGLESENSOR_H_
#define ANGLESENSOR_H_

#include "SensorBase.h"
#include "PIDSource.h"
#include "LiveWindow/LiveWindowSendable.h"
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "Notifier.h"
/**
 * Use the angle sensor to read an absolute angle measurement.
 * 
 * This utilizes the Avagotech AEAT-6012-A06 12-bit Absolute Magnetic Encoder.
 * 
 * This sensor requires two DigitalOutput pin (Chip Select, Clock)
 * 
 * This sensor also requires one DigitalInput pin (Data)
 */

class AngleSensor : public SensorBase, public PIDSource, public LiveWindowSendable
{
public:
	// Delay after lowering chip select, and clock high, when reading data
	// This value is specified in the datasheet
	static const long kReadDelayNanoseconds;

	UINT8 dsc;
	DigitalInput data;
	DigitalOutput chip_select;
	DigitalOutput clock;	
	Notifier notifier;
	double cached_angle;
	
	AngleSensor(UINT8 moduleNumber,UINT32 data_pin, UINT32 chipselect_pin, UINT32 clock_pin);
	virtual ~AngleSensor();
	virtual float GetAngle();
	float GetAngleInternal();

	// PIDSource interface
	double PIDGet();
	
	void UpdateTable();
	void StartLiveWindowMode();
	void StopLiveWindowMode();
	std::string GetSmartDashboardType();
	void InitTable(ITable *subTable);
	ITable * GetTable();

private:
	void InitAngleSensor();

	
	ITable *m_table;
};
#endif
