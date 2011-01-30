/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2008. All Rights Reserved.							  */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib.  */
/*----------------------------------------------------------------------------*/

#include "FilteredAccelerometer.h"
#include "AnalogModule.h"
#include "Utility.h"
#include "WPIStatus.h"

/**
 * Common function for initializing the accelerometer.
 */
void FilteredAccelerometer::InitAccelerometer()
{
	m_voltsPerG = 1.0;
	m_zeroGVoltage = 2.5;
	// Set the averaging total to larger than the default;
	// this is to overcome the nois signal from the accelerometer
	// when the default averaging window width is used.
	m_analogChannel->SetAverageBits(10);
}

/**
 * Create a new instance of an accelerometer.
 * 
 * The accelerometer is assumed to be in the first analog module in the given analog channel. The
 * constructor allocates desired analog channel.
 */
FilteredAccelerometer::FilteredAccelerometer(UINT32 channel)
{
	m_analogChannel = new AnalogChannel(channel);
	m_allocatedChannel = true;
	InitAccelerometer();
}

/**
 * Create new instance of accelerometer.
 * 
 * Make a new instance of the accelerometer given a module and channel. The constructor allocates
 * the desired analog channel from the specified module
 */
FilteredAccelerometer::FilteredAccelerometer(UINT32 slot, UINT32 channel)
{
	m_analogChannel = new AnalogChannel(slot, channel);
	m_allocatedChannel = true;
	InitAccelerometer();
}

/**
 * Create a new instance of Accelerometer from an existing AnalogChannel.
 * Make a new instance of accelerometer given an AnalogChannel. This is particularly
 * useful if the port is going to be read as an analog channel as well as through
 * the Accelerometer class.
 */
FilteredAccelerometer::FilteredAccelerometer(AnalogChannel *channel)
{
	if (channel == NULL)
	{
		wpi_fatal(NullParameter);
	}
	else
	{
		m_analogChannel = channel;
		InitAccelerometer();
	}
	m_allocatedChannel = false;
}
	
/**
 * Delete the analog components used for the accelerometer.
 */
FilteredAccelerometer::~FilteredAccelerometer()
{
	if (m_allocatedChannel)
	{
		delete m_analogChannel;
	}
}

/**
 * Return the acceleration in Gs.
 * 
 * The acceleration is returned units of Gs.
 * 
 * @return The current acceleration of the sensor in Gs.
 */
float FilteredAccelerometer::GetAcceleration()
{
	return (m_analogChannel->GetAverageVoltage() - m_zeroGVoltage) / m_voltsPerG;
}

/**
 * Set the accelerometer sensitivity.
 * 
 * This sets the sensitivity of the accelerometer used for calculating the acceleration.
 * The sensitivity varys by accelerometer model. There are constants defined for various models.
 * 
 * @param sensitivity The sensitivity of accelerometer in Volts per G.
 */
void FilteredAccelerometer::SetSensitivity(float sensitivity)
{
	m_voltsPerG = sensitivity;
}

/**
 * Set the voltage that corresponds to 0 G.
 * 
 * The zero G voltage varys by accelerometer model. There are constants defined for various models.
 * 
 * @param zero The zero G voltage.
 */
void FilteredAccelerometer::SetZero(float zero)
{
	m_zeroGVoltage = zero;
}

/**
 * Get the Acceleration for the PID Source parent.
 * 
 * @return The current acceleration in Gs.
 */ 
double FilteredAccelerometer::PIDGet()
{
	return GetAcceleration();
}
