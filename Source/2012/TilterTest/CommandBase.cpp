#include "CommandBase.h"
#include "Subsystems/TilterSubsystem.h"
#include "Commands/Scheduler.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

// Initialize a single static instance of all of your subsystems to NULL
TilterSubsystem* CommandBase::tilter = NULL;
OI* CommandBase::oi = NULL;

void CommandBase::init() {
    // Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	tilter = new TilterSubsystem();
	
	oi = new OI();
}
