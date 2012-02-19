#include "TrafficCopSubsystem.h"
#include "../Robotmap.h"

TrafficCopSubsystem::TrafficCopSubsystem() : Subsystem("TrafficCopSubsystem") 
{

}
    
void TrafficCopSubsystem::InitDefaultCommand() 
{
        // Set the default command for a subsystem here.
        //SetDefaultCommand(new MySpecialCommand());
}

void TrafficCopSubsystem::Forward()
{
        forward->Set(Relay::kForward);
        
}
void TrafficCopSubsystem::Back()
{
        back->Set(Relay::kReverse);
}
void TrafficCopSubsystem::Stop()
{
        stop->Set(Relay::kOn);
}
void TrafficCopSubsystem::Off()
{
        off->Set(Relay::kOff);
}

// Put methods for controlling this subsystem
// here. Call these from Commands.
