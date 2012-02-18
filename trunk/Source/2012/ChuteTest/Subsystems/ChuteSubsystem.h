#ifndef Chute_SUBSYSTEM_H
#define Chute_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#define STEERING_UPPER_BOUND_DEGREES 10
#define STEERING_LOWER_BOUND_DEGREES -10
#define STEERING_CENTER_POSITION_DEGREES 0


/**
 *
 *
 * @author ExampleAuthor
 */
class ChuteSubsystem: public PIDSubsystem {
private:
	
	Jaguar* steeringMotor;
	AnalogChannel* steeringPot;
	Solenoid* liftingUp;
	Solenoid* liftingDown;
	Solenoid* triggerOn;
	Solenoid* triggerOff;
	double minimumChuteAngle;
	double maximumChuteAngle;
	double maximumChuteVoltage;
	double minimumChuteVoltage;

	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
public:
	ChuteSubsystem();
	void InitDefaultCommand();
	
	void ChuteUp();
	void ChuteDown();
	void TriggerOn();
	void TriggerOff();
	void UsePIDOutput(double);
	double ReturnPIDInput();
	void SetSteeringAngle(double angle);
	double GetMinimumChuteAngle();
	double GetMaximumChuteAngle();
};

#endif
