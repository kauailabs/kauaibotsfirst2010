// RobotBuilder Version: 0.0.2
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.


#ifndef SWERVEDRIVESYSTEM_H
#define SWERVEDRIVESYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class SwerveDriveSystem: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	SpeedController* left_front_steer_motor;
	Ultrasonic* left_front_angle_sensor;
	PIDController* left_front_steer;
	Encoder* left_front_rpm_sensor;
	SpeedController* left_front_drive_motor;
	PIDController* left_front_drive;
	SpeedController* right_front_steer_motor;
	Ultrasonic* right_front_angle_sensor;
	PIDController* right_front_steer;
	Encoder* right_front_rpm_sensor;
	SpeedController* right_front_drive_motor;
	PIDController* right_front_drive;
	SpeedController* left_back_steer_motor;
	Ultrasonic* left_back_angle_sensor;
	PIDController* left_back_steer;
	Encoder* left_back_rpm_sensor;
	SpeedController* left_back_drive_motor;
	PIDController* left_back_drive;
	SpeedController* right_back_steer_motor;
	Ultrasonic* right_back_angle_sensor;
	PIDController* right_back_steer;
	SpeedController* right_back_drive_motor;
	Encoder* right_back_rpm_sensor;
	PIDController* right_back_drive;
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	SwerveDriveSystem();
	void InitDefaultCommand();
};

#endif
