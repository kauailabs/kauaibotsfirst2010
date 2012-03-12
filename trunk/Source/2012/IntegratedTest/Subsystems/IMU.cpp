/*

MinIMU-9-Arduino-AHRS
Pololu MinIMU-9 + Arduino AHRS (Attitude and Heading Reference System)

Copyright (c) 2011 Pololu Corporation.
http://www.pololu.com/

MinIMU-9-Arduino-AHRS is based on sf9domahrs by Doug Weibel and Jose Julio:
http://code.google.com/p/sf9domahrs/

sf9domahrs is based on ArduIMU v1.5 by Jordi Munoz and William Premerlani, Jose
Julio and Doug Weibel:
http://code.google.com/p/ardu-imu/

MinIMU-9-Arduino-AHRS is free software: you can redistribute it and/or modify it
under the terms of the GNU Lesser General Public License as published by the
Free Software Foundation, either version 3 of the License, or (at your option)
any later version.

MinIMU-9-Arduino-AHRS is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for
more details.

You should have received a copy of the GNU Lesser General Public License along
with MinIMU-9-Arduino-AHRS. If not, see <http://www.gnu.org/licenses/>.

*/

#include "L3G4200D.h"
#include "LSM303.h"
#include "Wire.h"
#include "Arduino.h"

#define _EXTERN extern

_EXTERN int SENSOR_SIGN[9];
_EXTERN int AN[6]; //array that stores the gyro and accelerometer data
_EXTERN int AN_OFFSET[6]; //Array that stores the Offset of the sensors
_EXTERN int gyro_x;
_EXTERN int gyro_y;
_EXTERN int gyro_z;
_EXTERN int accel_x;
_EXTERN int accel_y;
_EXTERN int accel_z;
_EXTERN int magnetom_x;
_EXTERN int magnetom_y;
_EXTERN int magnetom_z;

L3G4200D gyro;
LSM303 compass;

void I2C_Init()
{
  Wire.begin();
}

void Gyro_Init()
{
  gyro.writeReg(L3G4200D_CTRL_REG1, 0x0F); // normal power mode, all axes enabled, 100 Hz
  gyro.writeReg(L3G4200D_CTRL_REG4, 0x20); // 2000 dps full scale
}

void Read_Gyro()
{
  gyro.read();
  
  AN[0] = int(gyro.g.x);  // Scott Libert, 3/10/2012 - Added cast
  AN[1] = int(gyro.g.y);
  AN[2] = int(gyro.g.z);
  gyro_x = SENSOR_SIGN[0] * (AN[0] - AN_OFFSET[0]);
  gyro_y = SENSOR_SIGN[1] * (AN[1] - AN_OFFSET[1]);
  gyro_z = SENSOR_SIGN[2] * (AN[2] - AN_OFFSET[2]);
}

void Accel_Init()
{
  compass.writeAccReg(LSM303_CTRL_REG1_A, 0x27); // normal power mode, all axes enabled, 50 Hz
  compass.writeAccReg(LSM303_CTRL_REG4_A, 0x00); // 2 g acceleration scale (was 0x30, 8g)
}

// Reads x,y and z accelerometer registers
void Read_Accel()
{
  compass.readAcc();
  
  AN[3] = int(compass.a.x);
  AN[4] = int(compass.a.y);
  AN[5] = int(compass.a.z);
  accel_x = SENSOR_SIGN[3] * (AN[3] - AN_OFFSET[3]);
  accel_y = SENSOR_SIGN[4] * (AN[4] - AN_OFFSET[4]);
  accel_z = SENSOR_SIGN[5] * (AN[5] - AN_OFFSET[5]);
}

void Compass_Init()
{
  compass.init();
  compass.writeMagReg(LSM303_MR_REG_M, 0x00); // continuous conversion mode
  // 15 Hz default
}

void Read_Compass()
{
  compass.readMag();
  
  magnetom_x = int(SENSOR_SIGN[6] * compass.m.x);
  magnetom_y = int(SENSOR_SIGN[7] * compass.m.y);
  magnetom_z = int(SENSOR_SIGN[8] * compass.m.z);
}

LSM303::vector running_min = {2047, 2047, 2047}, running_max = {-2048, -2048, -2048};

void Calibrate_Compass( int numIterations )
{
	// Run iteratively at 10Hz rate
	// Read LM303 and calibrate the min/maxes
	for ( int i = 0; i < numIterations; i++ )
	{
		compass.read();    
		
		running_min.x = min(running_min.x, compass.m.x);  
		running_min.y = min(running_min.y, compass.m.y);  
		running_min.z = min(running_min.z, compass.m.z);  
		
		running_max.x = max(running_max.x, compass.m.x);  
		running_max.y = max(running_max.y, compass.m.y);  
		running_max.z = max(running_max.z, compass.m.z);    
		
		printf("M min X: %4d Y:  %4d Z:  %4d\n", (int)running_min.x,
				(int)running_min.y,
				(int)running_min.z);
		
		printf("M max X: %4d Y:  %4d Z:  %4d\n", (int)running_max.x,
				(int)running_max.y,
				(int)running_max.z);
		
		delay(100);
	}
}
