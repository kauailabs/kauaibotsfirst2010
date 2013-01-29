#ifndef SWERVESTEER_H
#define SWERVESTEER_H
#include "DigitalInput.h"
#include "DigitalOutput.h"
#include "Commands/PIDSubsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author slibert
 */
class SwerveSteer: public PIDSubsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	// set the P, I, and D constants here
	static const double Kp = (1.0/180.0)*.5;
	static const double Ki = 0.0;
	static const double Kd = 0.0;
	DigitalInput data;
	DigitalOutput chip_select;
	DigitalOutput clock;
	Talon motor;
	
public:
	SwerveSteer(int dsc, int data_pin, int chipselect_pin, int clock_pin, int motor_channel);
	double ReturnPIDInput();
	void UsePIDOutput(double output);
	void InitDefaultCommand();
};

#endif
