#ifndef _INTEGRATING_ACCELEROMETER_H
#define _INTEGRATING_ACCELERMOETER_H

#include "WPILib.h"

#define GRAVITY_CONSTANT 9.81  // Meters/sec^2
#define FEET_PER_METER   3.291 // Feet/Meter

//
// 50 calibration points at 20ms each will cost us a total of 1 second start
// up time.
//
#ifndef ACCEL_NUM_CAL_PTS
    #define ACCEL_NUM_CAL_PTS   50
#endif
#ifndef ACCEL_CAL_INTERVAL
    #define ACCEL_CAL_INTERVAL  0.01    //10ms
#endif

/**
 * This class defines and implements the IntegratingAccelerometer object. 
 * This class periodically sample the accelerometer value for the acceleration
 * value. It also integrates the acceleration value to calculate the velocity
 * and then integrates the velocity to calculate the distance value.
 */
 
#define GetMsecTime()           (GetFPGATime()/1000)
#define ABS(n)                  (((n) < 0)? -(n): (n))
#define DEADBAND(n,t)           ((ABS(n) > (t))? (n): 0)

class IntegratingAccelerometer : public ADXL345_I2C
{
private:
    SEM_ID      m_semaphore;
    Notifier   *m_notifier;
    AllAxes     m_accelData;
    AllAxes     m_zeroOffset;
    AllAxes     m_deadBand;
    double      m_xVel;
    double      m_yVel;
    double      m_zVel;
    double      m_xDist;
    double      m_yDist;
    double      m_zDist;
    bool        m_bEnabled;
    UINT32      m_timestamp;
    
    /**
     * This function is called periodically by the a timer callback to
     * process the accelerometer data. It integrates the data with time
     * to calculate the velocity, integrates it again to calculate
     * distance.
     */
     
    void Integrator()
    {
        CRITICAL_REGION(m_semaphore)
        {
            if (m_bEnabled)
            {
                UINT32 timeCurr = GetMsecTime();
                float period = (float)(timeCurr - m_timestamp)/1000.0;
                m_timestamp = timeCurr;
                
                // Get Current Accelerometer Readings
                
                m_accelData = GetAccelerations();
                
                // Adjust accelerometer data by the calibrated zero offsets
                
                m_accelData.XAxis -= m_zeroOffset.XAxis;
                m_accelData.YAxis -= m_zeroOffset.YAxis;
                m_accelData.ZAxis -= m_zeroOffset.ZAxis;
                
                // Transform accelerometer data from Gs to m/s^2
                
                m_accelData.XAxis = GRAVITY_CONSTANT*
                                    DEADBAND(m_accelData.XAxis,
                                             m_deadBand.XAxis);
                m_accelData.YAxis = GRAVITY_CONSTANT*
                                    DEADBAND(m_accelData.YAxis,
                                             m_deadBand.YAxis);
                m_accelData.ZAxis = GRAVITY_CONSTANT*
                                    DEADBAND(m_accelData.ZAxis,
                                             m_deadBand.ZAxis);

				// Calculate Velocity (m/s)

                m_xVel += m_accelData.XAxis*period;
                m_yVel += m_accelData.YAxis*period;
                m_zVel += m_accelData.ZAxis*period;
                
                // Calculate Distance (m)
                
                m_xDist += m_xVel*period;
                m_yDist += m_yVel*period;
                m_zDist += m_zVel*period;
            }
        }
        END_REGION;
    }   //Integrator

    static void CallIntegrator( void *accel )
    {
        ((IntegratingAccelerometer *)accel)->Integrator();
    }

public:

    /**
     * This function calibrates the zero G point and deadband.
     * It assumes the accelerometer is sitting still at level ground during
     * the calibration. It samples a number of points on all axes and averages
     * them to be the zero G point for the axes. Note that the Z-axis is not
     * really at zero G when it is still. Z-axis should be 1G at level ground.
     * But for the purpose of measuring relative G's on all axes, we calibrate
     * zero G for all axes. During calibration, we also determine the floor
     * and ceiling noise level to form the deadband zone.
     *
     * @param numCalPts Specifies the number of calibration points.
     * @param calInterval Specifies the calibration interval in seconds.
     */
     
    void Calibrate( UINT32 numCalPts, float calInterval )
    {
        AllAxes data;
        AllAxes zeroG;
        AllAxes min;
        AllAxes max;

        zeroG.XAxis = 0.0;
        zeroG.YAxis = 0.0;
        zeroG.YAxis = 0.0;
        min.XAxis = 1000.0;
        min.YAxis = 1000.0;
        min.ZAxis = 1000.0;
        max.XAxis = 0.0;
        max.YAxis = 0.0;
        max.ZAxis = 0.0;

        for (UINT32 i = 0; i < numCalPts; i++)
        {
            data = GetAccelerations();

            zeroG.XAxis += data.XAxis;
            zeroG.YAxis += data.YAxis;
            zeroG.ZAxis += data.ZAxis;

            if (data.XAxis < min.XAxis)
            {
                min.XAxis = data.XAxis;
            }
            else if (data.XAxis > max.XAxis)
            {
                max.XAxis = data.XAxis;
            }

            if (data.YAxis < min.YAxis)
            {
                min.YAxis = data.YAxis;
            }
            else if (data.YAxis > max.YAxis)
            {
                max.YAxis = data.YAxis;
            }

            if (data.ZAxis < min.ZAxis)
            {
                min.ZAxis = data.ZAxis;
            }
            else if (data.ZAxis > max.ZAxis)
            {
                max.ZAxis = data.ZAxis;
            }

            Wait(calInterval);
        }

        CRITICAL_REGION(m_semaphore)
        {
            m_zeroOffset.XAxis = zeroG.XAxis/numCalPts;
            m_zeroOffset.YAxis = zeroG.YAxis/numCalPts;
            m_zeroOffset.ZAxis = zeroG.ZAxis/numCalPts;

            m_deadBand.XAxis = max.XAxis - min.XAxis;
            m_deadBand.YAxis = max.YAxis - min.YAxis;
            m_deadBand.ZAxis = max.ZAxis - min.ZAxis;

            /*TInfo(("AccelZeroG:x=%f,y=%f,z=%f, AccelDeadBand:x=%f,y=%f,z=%f",
                   m_zeroOffset.XAxis, m_zeroOffset.YAxis, m_zeroOffset.ZAxis,
                   m_deadBand.XAxis, m_deadBand.YAxis, m_deadBand.ZAxis));*/
        }
        END_REGION;
    }

    void Calibrate()
    {
        Calibrate(ACCEL_NUM_CAL_PTS, ACCEL_CAL_INTERVAL);    	
    }
    
    IntegratingAccelerometer(UINT8 moduleNumber,
        float  period = 0.02 ) :
        	ADXL345_I2C( moduleNumber )
    {
        m_semaphore = semBCreate(SEM_Q_PRIORITY, SEM_FULL);
        m_notifier = new Notifier(IntegratingAccelerometer::CallIntegrator, this);
        m_accelData.XAxis = 0.0;
        m_accelData.YAxis = 0.0;
        m_accelData.ZAxis = 0.0;
        m_zeroOffset.XAxis = 0.0;
        m_zeroOffset.YAxis = 0.0;
        m_zeroOffset.ZAxis = 0.0;
        m_deadBand.XAxis = 0.0;
        m_deadBand.YAxis = 0.0;
        m_deadBand.ZAxis = 0.0;
        m_xVel = 0.0;
        m_yVel = 0.0;
        m_zVel = 0.0;
        m_xDist = 0.0;
        m_yDist = 0.0;
        m_zDist = 0.0;
        m_bEnabled = false;
        Calibrate();
        m_notifier->StartPeriodic(period);
    }

    ~IntegratingAccelerometer()
    {
        semFlush(m_semaphore);
        if ( m_notifier )
        {
        	delete m_notifier;
        	m_notifier = 0;
        }
    }

    double GetAccelX()	// X Axis in feet/second squared
    {
        double value = m_accelData.XAxis*FEET_PER_METER;
        return value;
    }
    double GetAccelY()	// Y Axis in feet/second squared
    {
        double value = m_accelData.YAxis*FEET_PER_METER;
        return value;
    }
    double GetAccelZ()	// Z Axis in feet/second squared
    {
        double value = m_accelData.ZAxis*FEET_PER_METER;
        return value;
    }

    double GetVelocityX()	// X Axis Velocity in feet/second
    {
        double value = m_xVel*FEET_PER_METER;
        return value;
    }
    double GetVelocityY()	// Y Axis Velocity in feet/second
    {
        double value = m_yVel*FEET_PER_METER;
        return value;
    }
    double GetVelocityZ()	// Z Axis Velocity in feet/second
    {
        double value = m_zVel*FEET_PER_METER;
        return value;
    }

    double GetDistanceX()	// X Distance Traveled in feet/second
    {
        double value = m_xDist*FEET_PER_METER;
        return value;
    }
    double GetDistanceY()	// Y Distance Traveled in feet/second
    {
        double value = m_yDist*FEET_PER_METER;
        return value;
    }
    double GetDistanceZ()	// Z Distance Traveled in feet/second
    {
        double value = m_zDist*FEET_PER_METER;
        return value;
    }

    void Reset()
    {
        CRITICAL_REGION(m_semaphore)
        {
            m_accelData.XAxis = 0.0;
            m_accelData.YAxis = 0.0;
            m_accelData.ZAxis = 0.0;
            m_xVel = 0.0;
            m_yVel = 0.0;
            m_zVel = 0.0;
            m_xDist = 0.0;
            m_yDist = 0.0;
            m_zDist = 0.0;
        }
        END_REGION;
    }

    void SetEnabled( bool bEnabled )
    {
        m_bEnabled = bEnabled;
        if (bEnabled)
        {
            Reset();
            m_timestamp = GetMsecTime();
        }
    }
};

#endif  //ifndef _TRCACCEL_H
