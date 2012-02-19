#ifndef TRAFFIC_COP_SUBSYSTEM_H
#define TRAFFIC_COP_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class TrafficCopSubsystem: public Subsystem {
private:
	Relay* forward;
	Relay* back;
	Relay* stop;
	Relay* off;
public:
	TrafficCopSubsystem();
	void InitDefaultCommand();
	
	void Forward();
	void Back();
	void Stop();
	void Off();
};

#endif
