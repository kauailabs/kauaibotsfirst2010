#include "ChuteSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/Chute.h"
#include "Preferences.h"
#define STEERING_CENTER_POSITION_DEGREES 0

ChuteSubsystem::ChuteSubsystem() : PIDSubsystem("ChuteSubsystem",2.3,0,0) 
{
	minimumChuteAngle=Preferences::GetInstance()->GetDouble("MinimumChuteAngle", -10);
	maximumChuteAngle=Preferences::GetInstance()->GetDouble("MaximumChuteAngle", 10);
	maximumChuteVoltage=Preferences::GetInstance()->GetDouble("MaximumChuteVoltage", 5);
	minimumChuteVoltage=Preferences::GetInstance()->GetDouble("MinimumChuteVoltage", 0);
	steeringMotor=new Jaguar(STEERING_JAGUAR_CHANNEL);
	steeringPot=new AnalogChannel(STEERING_POTENTIOMETER_MODULE, STEERING_POTENTIOMETER_CHANNEL);
	liftingUp=new Solenoid(FIRST_SOLENOID_MODULE, LIFTING_UP_SOLENOID_CHANNEL);
	liftingDown=new Solenoid(FIRST_SOLENOID_MODULE, LIFTING_DOWN_SOLENOID_CHANNEL);
	triggerUp=new Solenoid(FIRST_SOLENOID_MODULE, TRIGGER_UP_SOLENOID_CHANNEL);
	SetSetpointRange(STEERING_LOWER_BOUND_DEGREES, STEERING_UPPER_BOUND_DEGREES);
	SetSetpoint(STEERING_CENTER_POSITION_DEGREES);
	Enable();
	
	}

double ChuteSubsystem::GetMinimumChuteAngle()
{
	return minimumChuteAngle;
}

double ChuteSubsystem::GetMaximumChuteAngle()
{
	return maximumChuteAngle;
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
	triggerUp->Set(false);
}  
void ChuteSubsystem::UsePIDOutput(double output)
{
	steeringMotor->Set(output);
}

double ChuteSubsystem::ReturnPIDInput()
{
	return steeringPot->GetAverageVoltage()* 4 + -10;
}

void ChuteSubsystem::SetSteeringAngle(double angle)
{
	SetSetpoint(angle);
}



	







// Put methods for controlling this subsystem
// here. Call these from Commands.

