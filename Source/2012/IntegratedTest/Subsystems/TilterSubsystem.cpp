#include "TilterSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/Tilter.h"

TilterSubsystem::TilterSubsystem() : Subsystem("TilterSubsystem") {
        tilterUp = new Solenoid(1, TILTER_UP_SOLENOID_CHANNEL);
        tilterDown= new Solenoid(1, TILTER_DOWN_SOLENOID_CHANNEL);
}
    
void TilterSubsystem::InitDefaultCommand() {
        // Set the default command for a subsystem here.
        SetDefaultCommand(new Tilter());
}


// Put methods for controlling this subsystem
// here. Call these from Commands.

void TilterSubsystem::Up()
{
        tilterUp->Set(true);
        tilterDown->Set(false);
}

void TilterSubsystem::Down()
{               
        tilterDown->Set(true);
        tilterUp->Set(false);
}
