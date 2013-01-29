#include "SwerveSteer.h"
#include "../Robotmap.h"
#include "SmartDashboard/SmartDashboard.h"

SwerveSteer::SwerveSteer(int dsc, int data_pin, int chipselect_pin, int clock_pin, int motor_channel) : 
	PIDSubsystem("SwerveSteer", Kp, Ki, Kd),
	data(dsc, data_pin),
	chip_select(dsc, chipselect_pin),
	clock(dsc, clock_pin),
	motor(dsc, motor_channel)
{
	chip_select.Set(1);
	clock.Set(1);
	motor.SetSafetyEnabled(false);
	// Use these to get going:
	// SetSetpoint() -  Sets where the PID controller should move the system
	//                  to
	// Enable() - Enables the PID controller.
}

double SwerveSteer::ReturnPIDInput() {
	
	chip_select.Set(1);
	unsigned short angle=0;
	
	unsigned short bit;
	double real_Angle;
	clock.Set(1);
	chip_select.Set(0);
	for (int i=0;i<12;i++)
	{
		clock.Set(0);
		clock.Set(1);
		bit=data.Get();
		angle=angle<<1;
		angle|=bit;
	}
	chip_select.Set(1);
	clock.Set(1);
	real_Angle=((double)angle*360)/4096;
	real_Angle=real_Angle-180;
	printf("angle: %f\n",real_Angle);
	return real_Angle;
}

void SwerveSteer::UsePIDOutput(double output) {
	motor.SetSafetyEnabled(false);
	motor.Set(output);
	
}

void SwerveSteer::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//setDefaultCommand(new MySpecialCommand());
}
