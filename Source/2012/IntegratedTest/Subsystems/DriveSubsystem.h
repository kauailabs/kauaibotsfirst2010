#ifndef DRIVE_SUBSYSTEM_H
#define DRIVE_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "MecanumDrive.h"
#include "RangeFinder.h"
#include "FilteredAccelerometer.h"

/**
 *
 *
 * @author slibert
 */
class DriveSubsystem: public PIDSubsystem 
{
public:
        
        enum DriveGear { kLowGear, kHighGear };
        
        DriveSubsystem();
        void InitDefaultCommand();

        // Note:  vX, vY, vRot are velocities.  These values are currently from -1 to 1.
        // TODO:  Review this w/Quinn - should they be in RPM units if CANJaguar::ControlMode = Speed?
        
        void DoMecanum( float vX, float vY, float vRot );

        // Drive Modes
        
        CANJaguar::ControlMode GetControlMode();
        void SetControlMode( CANJaguar::ControlMode );

        int  GetSpeedModeMaxRPMs() { return drive.GetMaxSpeedModeRPMs(); }
        
        void SetAutoRotationMode( bool bEnable, double dTargetAngle );
        bool GetAutoRotationMode();
        
        void SetDriveGear( DriveSubsystem::DriveGear gear );
        DriveSubsystem::DriveGear GetDriveGear();
        
        // Returns true if in auto-rotation mode, false otherwise.
        // If the return value is true:
        // - bOnTarget is true if the PID controller's angular error is within tolerance.
        // - dSetPoint is the setpoint previously requested via SetAutoRotationMode().
        // - dAngularError error is the difference between the Setpoint and the current rotation gyro reading.
        
        bool GetAutoRotationTargetReadings( bool& bOnTarget, double& dSetPoint, double& dAngularError );
        
        // Sensor Readings
        
        void GetEulerAnglesDegrees( double& yaw, double& roll);
        void GetRangesInches( double& frontRange, double& rightRange, double& rearRange, double& leftRange );
        void GetEdges( bool &frontEdge, bool& rightEdge, bool& rearEdge, bool& leftEdge);
        void GetMotorCurrentAmps( double& frontLeft, double& frontRight, double& rearRight, double& rearLeft );
        void GetWheelSpeedsRPM( double& frontLeft, double& frontRight, double& rearRight, double& rearLeft );
        void GetAcclerationG( double& accelX, double& accelY, double& accelZ );
        
private:
        // It's desirable that everything possible under private except
        // for methods that implement subsystem capabilities
        MecanumDrive    drive;
        Gyro                    yaw;
        Gyro                    roll;
        RangeFinder             frontRanger;
        RangeFinder             rightRanger;
        RangeFinder             rearRanger;
        RangeFinder             leftRanger;
        DigitalInput    frontEdgeFinder;
        DigitalInput    rightEdgeFinder;
        DigitalInput    rearEdgeFinder;
        DigitalInput    leftEdgeFinder;
        FilteredAccelerometer   accelerometerX;
        FilteredAccelerometer   accelerometerY;
        FilteredAccelerometer   accelerometerZ;
protected:
        void InitializeSensors();
        double ReturnPIDInput();
        void UsePIDOutput(double);

    bool m_bAutoRotationMode;
    bool m_bAutoRotateSetpointSet;
    float m_pendingAutoRotateAmount;    // Temporary until m_bAutoRotateSetpointSet == true
    DriveGear m_DriveGear;
    float m_LowGearRatio;
    unsigned long m_UpdateDashboardCount;
    int m_UpdateDashboardRate;
    
    // Thread-safe accesors for auto-rotation amount         
        float ThreadSafeGetAutoRotateMotorOutputValue();         
        void ThreadSafeSetAutoMotorOutputValue( float rot );    
        
        double ClipGyroAngle( double dInputAngle );    
        void UpdateDashboardWithSensors();
};

#endif
