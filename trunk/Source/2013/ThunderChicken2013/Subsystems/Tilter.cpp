// RobotBuilder Version: 0.0.2
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// C++ from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in th future.




#include "Tilter.h"
#include "../Robot.h"
#include "../Robotmap.h"
#include <math.h>

#define const mmtoi 0.0393701

Tilter::Tilter() : Subsystem("Tilter") {
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
	height_sensor = RobotMap::tilterheight_sensor;
	motor = RobotMap::tiltermotor;
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
}
    
void Tilter::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
	SetDefaultCommand(new SetShooterTiltAngleDegrees());
	// BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
	// END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
	//SetDefaultCommand(new SetShooterTiltAngleDegrees());
}

void Tilter::PIDWrite(float output){
	motor->Set(output);
}

// PIDSource interface
double Tilter::PIDGet(){
	double theta,X,X2,Y;
	X=*mmtoi;
	theta = atan2((X+X2)/Y);
	theta=*(180/3.14);
	return theta;
}


// Put methods for controlling this subsystem
// here. Call these from Commands.
