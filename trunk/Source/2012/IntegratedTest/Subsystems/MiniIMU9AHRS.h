#ifndef MINIIMU9AHRS_H
#define MINIIMU9AHRS_H

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
#include "Wire.h"
#include "WPILib.h"

// LSM303 accelerometer: 8 g sensitivity
// 3.8 mg/digit; 1 g = 256
#define GRAVITY 256  //this equivalent to 1G in the raw data coming from the accelerometer 

#define ToRad(x) ((x)*0.01745329252)  // *pi/180
#define ToDeg(x) ((x)*57.2957795131)  // *180/pi

// L3G4200D gyro: 2000 dps full scale
// 70 mdps/digit; 1 dps = 0.07
#define Gyro_Gain_X 0.07 //X axis Gyro gain
#define Gyro_Gain_Y 0.07 //Y axis Gyro gain
#define Gyro_Gain_Z 0.07 //Z axis Gyro gain
#define Gyro_Scaled_X(x) ((x)*ToRad(Gyro_Gain_X)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Y(x) ((x)*ToRad(Gyro_Gain_Y)) //Return the scaled ADC raw data of the gyro in radians for second
#define Gyro_Scaled_Z(x) ((x)*ToRad(Gyro_Gain_Z)) //Return the scaled ADC raw data of the gyro in radians for second

inline float constrain(float val,float min,float max)
{
	if ( val < min ) return min;
	if ( val > max ) return max;
	return val;
}

// LSM303 magnetometer calibration constants; use the Calibrate example from
// the Pololu LSM303 library to find the right values for your board
#define M_X_MIN -796
#define M_Y_MIN -457
#define M_Z_MIN -424
#define M_X_MAX 197
#define M_Y_MAX 535
#define M_Z_MAX 397

#define Kp_ROLLPITCH 0.02
#define Ki_ROLLPITCH 0.00002
#define Kp_YAW 1.2
#define Ki_YAW 0.00002

/*For debugging purposes*/
//OUTPUTMODE=1 will print the corrected data, 
//OUTPUTMODE=0 will print uncorrected data of the gyros (with drift)
#define OUTPUTMODE 1

//#define PRINT_DCM 0     //Will print the whole direction cosine matrix
#define PRINT_ANALOGS 0 //Will print the analog raw data
#define PRINT_EULER 1   //Will print the Euler angles Roll, Pitch and Yaw

#define STATUS_LED 13 

class MiniIMU9AHRS
{
private:
    SEM_ID      m_semaphore;	
    Notifier   *m_notifier;
    
    static void CallIntegrator( void *ahrs );
    void loop();
    
public:
	MiniIMU9AHRS();
	~MiniIMU9AHRS();
    void GetEulerAnglesDegrees( double& pitch, double& roll, double& yaw );
    void GetAccelGs( double& x, double& y, double& z );
    void GetAngularVelocityDegreesPerSec( double& x, double& y, double& z );
    void GetMagnetometerGauss( double& x, double& y, double& z );
    double GetCompassHeading();
    
    // Calibration Routine
    //
    // Calibrate() calculates zero-g offsets and rotations 
    // for the accelerometer and gyroscope.  This routine currently
    // takes 3-4 second to execute, and is automatically invoked
    // within the constructor of this class.
    //
    // Therefore, it is not typically required to invoke this method.
	void Calibrate();
	
	// CalibrateCompass() ensures the heading derived from the compass
	// is correct; this needs only to be invoked once for each
	// Magnetometer, and calculates the "true north"; the results
	// are used to set the M_X_MIN through M_Z_MAX constants
    void CalibrateCompass( int numIterations );
};

#endif
