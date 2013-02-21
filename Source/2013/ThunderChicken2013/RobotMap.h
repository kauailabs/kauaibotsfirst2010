// RobotBuilder Version: 0.0.2
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.


#ifndef ROBOTMAP_H
#define ROBOTMAP_H
#include "WPILib.h"
#include "Subsystems/AngleSensor.h"
#include "Subsystems/ProximitySensor.h"

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
class RobotMap {
public:
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	static Servo* cameraazimuth_servo;
	static Servo* cameraelevation_servo;
	static SpeedController* shootermotor;
	static DoubleSolenoid* magazinetrigger;
	static DoubleSolenoid* magazinelifter;
	static ProximitySensor* magazinefirsbee_counter;
	static Compressor* pneumaticscompressor;
	static ProximitySensor* tilterheight_sensor;
	static SpeedController* tiltermotor;
	static SpeedController* climberfront_winch_motor;
	static SpeedController* climberrear_winch_motor;
	static SpeedController* swerveDriveSystemleft_front_steer_motor;
	static AngleSensor* swerveDriveSystemleft_front_angle_sensor;
	static PIDController* swerveDriveSystemleft_front_steer;
	static Encoder* swerveDriveSystemleft_front_rpm_sensor;
	static SpeedController* swerveDriveSystemleft_front_drive_motor;
	static PIDController* swerveDriveSystemleft_front_drive;
	static SpeedController* swerveDriveSystemright_front_steer_motor;
	static AngleSensor* swerveDriveSystemright_front_angle_sensor;
	static PIDController* swerveDriveSystemright_front_steer;
	static Encoder* swerveDriveSystemright_front_rpm_sensor;
	static SpeedController* swerveDriveSystemright_front_drive_motor;
	static PIDController* swerveDriveSystemright_front_drive;
	static SpeedController* swerveDriveSystemleft_back_steer_motor;
	static AngleSensor* swerveDriveSystemleft_back_angle_sensor;
	static PIDController* swerveDriveSystemleft_back_steer;
	static Encoder* swerveDriveSystemleft_back_rpm_sensor;
	static SpeedController* swerveDriveSystemleft_back_drive_motor;
	static PIDController* swerveDriveSystemleft_back_drive;
	static SpeedController* swerveDriveSystemright_back_steer_motor;
	static AngleSensor* swerveDriveSystemright_back_angle_sensor;
	static PIDController* swerveDriveSystemright_back_steer;
	static SpeedController* swerveDriveSystemright_back_drive_motor;
	static Encoder* swerveDriveSystemright_back_rpm_sensor;
	static PIDController* swerveDriveSystemright_back_drive;
	static SerialPort *imu_serial_port;
	static Solenoid* front_climber_solenoid;
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

	static void init();
};
#endif