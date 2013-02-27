/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#ifndef IMU_H_
#define IMU_H_

#include "SensorBase.h"
#include "PIDSource.h"
#include "LiveWindow/LiveWindowSendable.h"
#include "SerialPort.h"
/**
 * Use the angle sensor to read an absolute angle measurement.
 * 
 * This utilizes the Avagotech AEAT-6012-A06 12-bit Absolute Magnetic Encoder.
 * 
 * This sensor requires two DigitalOutput pin (Chip Select, Clock)
 * 
 * This sensor also requires one DigitalInput pin (Data)
 */

class IMU : public SensorBase, public PIDSource, public LiveWindowSendable
{
	SerialPort *pserial_port;
	
public:

	IMU( SerialPort *pport );
	virtual ~IMU();
	virtual float GetPitch();
	virtual float GetRoll();
	virtual float GetYaw();
	
	bool IsConnected();
	void ZeroYaw();
	
	// PIDSource interface, returns yaw component
	double PIDGet();
	
	void UpdateTable();
	void StartLiveWindowMode();
	void StopLiveWindowMode();
	std::string GetSmartDashboardType();
	void InitTable(ITable *subTable);
	ITable * GetTable();

	SerialPort *GetSerialPort() { return pserial_port; }
	void SetYawPitchRoll(float yaw, float pitch, float roll);
	
private:
	void InitIMU();
    Task              m_task;
	float yaw;
	float pitch; 
	float roll;
    
	ITable *m_table;
};
#endif
