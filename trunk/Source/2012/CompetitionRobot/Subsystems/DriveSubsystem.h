#ifndef DRIVE_SUBSYSTEM_H
#define DRIVE_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "MecanumDrive.h"
#include "RangeFinder.h"

/**
 *
 *
 * @author slibert
 */
class DriveSubsystem: public PIDSubsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	MecanumDrive	drive;
	Gyro			yaw;
	Gyro			pitch;
	Gyro			roll;
	RangeFinder		frontRanger;
	RangeFinder		rightRanger;
	RangeFinder		rearRanger;
	RangeFinder		leftRanger;
	DigitalInput	frontLeftEdgeFinder;
	DigitalInput	frontRightEdgeFinder;
	DigitalInput	rearLeftEdgeFinder;
	DigitalInput	rearRightEdgeFinder;
protected:
	void InitializeSensors();
	double ReturnPIDInput();
	void UsePIDOutput(double);

public:
	DriveSubsystem();
	void InitDefaultCommand();
	
	void DoMecanum( float vX, float vY, float vRot );
	CANJaguar::ControlMode getMode();
	void setMode( CANJaguar::ControlMode );
	void GetEulerAngles( double& yaw, double& pitch, double& roll);
	void GetRangesInches( double& frontRange, double& rightRange, double& rearRange, double& leftRange );
	void GetEdges( bool &frontLeft, bool& frontRight, bool& rearRight, bool& rearLeft);
};

#endif
