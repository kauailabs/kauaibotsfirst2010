#include "ChuteSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/Chute.h"
#define STEERING_UPPER_BOUND_DEGREES 10
#define STEERING_LOWER_BOUND_DEGREES -10
#define STEERING_CENTER_POSITION_DEGREES 0

ChuteSubsystem::ChuteSubsystem() : PIDSubsystem("ChuteSubsystem",2.3,0,0) 
{
	steeringMotor=new Jaguar(STEERING_JAGUAR_CHANNEL);
	steeringPot=new AnalogChannel(STEERING_POTENTIOMETER_MODULE, STEERING_POTENTIOMETER_CHANNEL);
	liftingUp=new Solenoid(FIRST_SOLENOID_MODULE, LIFTING_UP_SOLENOID_CHANNEL);
	liftingDown=new Solenoid(FIRST_SOLENOID_MODULE, LIFTING_DOWN_SOLENOID_CHANNEL);
	triggerRightUp=new Solenoid(FIRST_SOLENOID_MODULE, TRIGGER_RIGHT_UP_SOLENOID_CHANNEL);
	triggerRightDown=new Solenoid(FIRST_SOLENOID_MODULE, TRIGGER_RIGHT_DOWN_SOLENOID_CHANNEL);
	triggerLeftDown=new Solenoid(FIRST_SOLENOID_MODULE, TRIGGER_LEFT_DOWN_SOLENOID_CHANNEL);
	triggerLeftUp=new Solenoid(FIRST_SOLENOID_MODULE, TRIGGER_LEFT_UP_SOLENOID_CHANNEL);
	SetSetpointRange(STEERING_LOWER_BOUND_DEGREES, STEERING_UPPER_BOUND_DEGREES);
	SetSetpoint(STEERING_CENTER_POSITION_DEGREES);
	Enable();
	
	}

void ChuteSubsystem::InitDefaultCommand() 
{
	SetDefaultCommand(new Chute());
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void ChuteSubsystem::ChuteUp()
{
	liftingUp->Set(true);
	liftingDown->Set(false);
}
void ChuteSubsystem::ChuteDown()
{
	liftingUp->Set(false);
	liftingDown->Set(true);

}
void ChuteSubsystem::TriggerOn()
{

	triggerLeftUp->Set(false);
	triggerLeftDown->Set(true);
	triggerRightUp->Set(false);
	triggerRightDown->Set(true);
}

void ChuteSubsystem::TriggerOff()
{
	triggerLeftUp->Set(true);
	triggerLeftDown->Set(false);
	triggerRightUp->Set(true);
	triggerRightDown->Set(false);
}  
void ChuteSubsystem::UsePIDOutput(double output)
{
	steeringMotor->Set(output);
}

double ChuteSubsystem::ReturnPIDInput()
{
	return steeringPot->GetAverageVoltage() / 5.0;
}

void ChuteSubsystem::SetSteeringAngle(double angle)
{
	SetSetpoint(angle);
}


	







// Put methods for controlling this subsystem
// here. Call these from Commands.
