
#include "HopperSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/Hopper.h"

HopperSubsystem::HopperSubsystem() : Subsystem("HopperSubsystem") {
        hopperMotor = new Victor(HOPPERMOTOR_VICTOR_CHANNEL);

}
    
void HopperSubsystem::InitDefaultCommand() {
        // Set the default command for a subsystem here.
        SetDefaultCommand(new Hopper());
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void HopperSubsystem::On()
{
        hopperMotor->Set(1);

}

void HopperSubsystem::Off()
{               
        hopperMotor->Set(0);

}
