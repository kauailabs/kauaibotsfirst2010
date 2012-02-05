#ifndef HOPPER_SUBSYSTEM_H
#define HOPPER_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"


/**
 *
 *
 * @author Jasmine
 */
class HopperSubsystem: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	Victor*hopperMotor;
	
public:
	HopperSubsystem();
	
	
	void InitDefaultCommand();
	void Off();
	void On();
};

#endif
