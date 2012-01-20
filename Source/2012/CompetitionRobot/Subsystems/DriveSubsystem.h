#ifndef DRIVE_SUBSYSTEM_H
#define DRIVE_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "MecanumDrive.h"

/**
 *
 *
 * @author slibert
 */
class DriveSubsystem: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	MecanumDrive drive;
public:
	DriveSubsystem();
	void InitDefaultCommand();
	
	void DoMecanum( float vX, float vY, float vRot );
	CANJaguar::ControlMode getMode();
	void setMode( CANJaguar::ControlMode );

};

#endif
