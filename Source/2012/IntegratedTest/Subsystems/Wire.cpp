/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

extern "C" {
  #include <stdlib.h>
  #include <string.h>
}

#include "Wire.h"

// Initialize Class Variables //////////////////////////////////////////////////

uint8_t 		TwoWire::txAddress = 0;
uint8_t 		TwoWire::transmitting = 0;
I2CDeviceInfo 	TwoWire::m_DeviceInfo[3];
I2C *			TwoWire::m_pCurrI2CDevice = 0;
UINT8 			TwoWire::m_dscModuleNumber = 1;

uint8_t 		TwoWire::rxBuffer[BUFFER_LENGTH];
uint8_t 		TwoWire::rxBufferIndex = 0;
uint8_t 		TwoWire::rxBufferLength = 0;

uint8_t 		TwoWire::txBuffer[BUFFER_LENGTH];
uint8_t 		TwoWire::txBufferIndex = 0;
uint8_t 		TwoWire::txBufferLength = 0;

// Constructors ////////////////////////////////////////////////////////////////

TwoWire::TwoWire()
{
	for ( int i = 0; i < NUM_I2C_DEVICES; i++ )
	{
		m_DeviceInfo[i].pDevice = 0;
		m_DeviceInfo[i].deviceAddress = 0;
	}
	m_pCurrI2CDevice = 0;
	for ( size_t i = 0; i < sizeof(rxBuffer)/sizeof(rxBuffer[0]); i++ )
	{
		rxBuffer[i] = 0;
	}
	for ( size_t i = 0; i < sizeof(txBuffer)/sizeof(txBuffer[0]); i++ )
	{
		txBuffer[i] = 0;
	}
}

// Public Methods //////////////////////////////////////////////////////////////

void TwoWire::begin(void)
{
	// On the arduino, this needs to be called only once.
	  rxBufferIndex = 0;
	  rxBufferLength = 0;

	  txBufferIndex = 0;
	  txBufferLength = 0;
}

I2C *TwoWire::GetDeviceByAddress(uint8_t address)
{
	int iFirstEmptySlot = -1;
	for ( int i = 0; i < NUM_I2C_DEVICES; i++ )
	{
		if ( m_DeviceInfo[i].pDevice )
		{
			if ( m_DeviceInfo[i].deviceAddress == address )
			{
				return m_DeviceInfo[i].pDevice;
			}
		}
		else
		{
			if ( iFirstEmptySlot == -1 )
			{
				iFirstEmptySlot = i;
			}
		}
	}	

	// No more empty slots, and device not in list.  Return null.
	if ( iFirstEmptySlot == -1 ) return 0;

	// Device not yet in list, and an empty slot is avaible
	// Acquire the new I2C device from the DSC Module.
	
	DigitalModule *module = DigitalModule::GetInstance(m_dscModuleNumber);
	if (module)
	{
		m_DeviceInfo[iFirstEmptySlot].pDevice = module->GetI2C(address);
		m_DeviceInfo[iFirstEmptySlot].deviceAddress = address;
		return m_DeviceInfo[iFirstEmptySlot].pDevice;
	}
	
	return 0;	// No DSC Module - return NULL.
}

void TwoWire::begin(uint8_t address)
{
	begin();
	// If the address is not 0, someone is requesting I2C slave mode
	// which is not currently supported.
}

void TwoWire::begin(int address)
{
  begin((uint8_t)address);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity)
{
	// Read the requested number of bytes from the device
	// specified by the device address.  The data is read 
	// into the receive buffer, and can later be read out
	// using the read() method.

	
	// NOTE:  I2C Addresses in Arduino code are 7-bit, whereas
	// I2C Addresses in the WPILibrary are 8-bit.  To transform an
	// Arduino I2C Address to WPILibrary form, shift right one bit.
	
	address <<= 1;
	
	rxBufferIndex = 0;
	rxBufferLength = 0;
	m_pCurrI2CDevice = GetDeviceByAddress(address);
	if ( m_pCurrI2CDevice )
	{
		// clamp to buffer length
		if( quantity > BUFFER_LENGTH )
		{
			quantity = BUFFER_LENGTH;
		}
		
		// perform blocking read into buffer	
		
		if ( !m_pCurrI2CDevice->Transaction( 0, 0, &rxBuffer[rxBufferIndex], quantity ) )
		{
			rxBufferLength = quantity;
		}
	}
    return rxBufferLength;
}

uint8_t TwoWire::requestFrom(int address, int quantity)
{
  return requestFrom((uint8_t)address, (uint8_t)quantity);
}

void TwoWire::beginTransmission(uint8_t address)
{
	// indicate that we are transmitting
	transmitting = 1;
	
	// set address of targeted slave
	
	// NOTE:  I2C Addresses in Arduino code are 7-bit, whereas
	// I2C Addresses in the WPILibrary are 8-bit.  To transform an
	// Arduino I2C Address to WPILibrary form, shift right one bit.
	  
	address <<= 1;
	txAddress = address;
	m_pCurrI2CDevice = GetDeviceByAddress(txAddress);
		
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;  
}

void TwoWire::beginTransmission(int address)
{
	beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(void)
{
	// transmit buffer (blocking)
	uint8_t ret = 4;
	if ( transmitting && (txBufferLength > 0 ) && m_pCurrI2CDevice )
	{
		if ( !m_pCurrI2CDevice->Transaction(txBuffer,txBufferLength,rxBuffer,0) )
		{
			ret = 0;
		}
	}
  
	// reset tx buffer iterator vars
	txBufferIndex = 0;
	txBufferLength = 0;
	// indicate that we are done transmitting
	transmitting = 0;
	m_pCurrI2CDevice = 0;
	return ret;
}

// must be called after beginTransmission(address)
size_t TwoWire::write(uint8_t data)
{
	if(transmitting)
	{
		// in master transmitter mode
		// don't bother if buffer is full
		if(txBufferLength >= BUFFER_LENGTH)
		{
			return 0;
		}
		
		// put byte in tx buffer
		txBuffer[txBufferIndex] = data;
		++txBufferIndex;
		// update amount in buffer   
		txBufferLength = txBufferIndex;
		return 1;
	}
	else
	{
		// Currently, slave writes (responses) are not supported.
	}
	return 0;
}

// must be called after beginTransmission(address)
size_t TwoWire::write(const uint8_t *data, size_t quantity)
{
	if(transmitting)
	{
		// in master transmitter mode
		for(size_t i = 0; i < quantity; ++i)
		{
			write(data[i]);
		}
	}
	else
	{
		// Currently, slave writes (responses) are not supported.
	}
	return quantity;
}

// must be called after requestFrom(address, numBytes)
int TwoWire::available(void)
{
  return rxBufferLength - rxBufferIndex;
}

// must be called after requestFrom(address, numBytes)
int TwoWire::read(void)
{
	int value = -1;
	
	// get each successive byte on each call
	if(rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
		++rxBufferIndex;
	}

	return value;
}

// must be called after requestFrom(address, numBytes)
int TwoWire::peek(void)
{
	int value = -1;
	
	if(rxBufferIndex < rxBufferLength)
	{
		value = rxBuffer[rxBufferIndex];
	}
	
	return value;
}

void TwoWire::flush(void)
{
  // XXX: to be implemented.
}

// Preinstantiate Objects //////////////////////////////////////////////////////

TwoWire Wire = TwoWire();
