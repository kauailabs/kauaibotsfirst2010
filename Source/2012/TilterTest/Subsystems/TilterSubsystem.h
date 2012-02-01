#ifndef TILTER_SUBSYSTEM_H
#define TILTER_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"


/**
 *
 *
 * @author ExampleAuthor
 */
class TilterSubsystem: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	
	Solenoid*tilterUp;
	Solenoid*tilterDown;
	
public:
	TilterSubsystem();
	
	void InitDefaultCommand();
	void Up();
	void Down();
};

#endif
