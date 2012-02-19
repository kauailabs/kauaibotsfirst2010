#include "TrafficCopSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/TrafficCop.h"

TrafficCopSubsystem::TrafficCopSubsystem() : Subsystem("TrafficCopSubsystem") 
{
	relay = new Relay(TRAFFIC_COP_SPIKE_RELAY);
}
    
void TrafficCopSubsystem::InitDefaultCommand() 
{
        // Set the default command for a subsystem here.
        SetDefaultCommand(new TrafficCop());
}

void TrafficCopSubsystem::Forward()
{
       relay->Set(Relay::kForward);
        
}
void TrafficCopSubsystem::Back()
{
	relay->Set(Relay::kReverse);
}
void TrafficCopSubsystem::Stop()
{
	relay->Set(Relay::kOn);
}
void TrafficCopSubsystem::Off()
{
	relay->Set(Relay::kOff);
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
