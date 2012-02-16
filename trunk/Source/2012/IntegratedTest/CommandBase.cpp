#include "CommandBase.h"
#include "Subsystems/GobblerSubsystem.h"
#include "Subsystems/DriveSubsystem.h"
#include "Subsystems/ChuteSubsystem.h"
#include "Subsystems/TilterSubsystem.h"
#include "Subsystems/CompressorSubsystem.h"
#include "Subsystems/HopperSubsystem.h"
#include "Commands/Scheduler.h"
#include "Subsystems/CameraSubsystem.h"

CommandBase::CommandBase(const char *name) : Command(name) {
}

CommandBase::CommandBase() : Command() {
}

// Initialize a single static instance of all of your subsystems to NULL
GobblerSubsystem* CommandBase::gobbler = NULL;
ChuteSubsystem* CommandBase::chute = NULL;
DriveSubsystem* CommandBase::drive = NULL;
TilterSubsystem* CommandBase::tilter = NULL;
CompressorSubsystem* CommandBase::compressor = NULL;
HopperSubsystem* CommandBase::hopper = NULL;
CameraSubsystem* CommandBase::camera = NULL;
OI* CommandBase::oi = NULL;

void CommandBase::init() {
    // Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	gobbler = new GobblerSubsystem();
	chute = new ChuteSubsystem();
	drive = new DriveSubsystem();
	tilter = new TilterSubsystem();
	compressor = new CompressorSubsystem();
	hopper = new HopperSubsystem();
	camera = new CameraSubsystem();
	oi = new OI();
}
