#include "CommandBase.h"
#include "Subsystems/ExampleSubsystem.h"
#include "Commands/Scheduler.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

CommandBase::~CommandBase()
{
	if ( examplesubsystem != NULL )
	{
		delete examplesubsystem;
		examplesubsystem = NULL;
	}
	if ( camerasubsystem != NULL )
	{
		delete camerasubsystem;
		camerasubsystem = NULL;
	}
	if ( oi != NULL )
	{
		delete oi;
		oi = NULL;
	}
}

// Initialize a single static instance of all of your subsystems to NULL
ExampleSubsystem* CommandBase::examplesubsystem = NULL;
CameraSubsystem * CommandBase::camerasubsystem = NULL;

OI* CommandBase::oi = NULL;

void CommandBase::init() {
    // Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	examplesubsystem = new ExampleSubsystem();
	camerasubsystem = new CameraSubsystem();
	oi = new OI();
}
