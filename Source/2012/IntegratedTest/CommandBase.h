#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include "Commands/Command.h"
#include "Subsystems/GobblerSubsystem.h"
#include "Subsystems/DriveSubsystem.h"
#include "Subsystems/ChuteSubsystem.h"
#include "Subsystems/TilterSubsystem.h"
#include "Subsystems/CompressorSubsystem.h"
#include "Subsystems/HopperSubsystem.h"
#include "Subsystems/CameraSubsystem.h"
#include "OI.h"


/**
 * The base for all commands. All atomic commands should subclass CommandBase.
 * CommandBase stores creates and stores each control system. To access a
 * subsystem elsewhere in your code in your code use CommandBase.examplesubsystem
 */
class CommandBase: public Command {
public:
	CommandBase(const char *name);
	CommandBase();
	static void init();
	// Create a single static instance of all of your subsystems
	static GobblerSubsystem *gobbler;
	static DriveSubsystem *drive;
	static ChuteSubsystem *chute;
	static TilterSubsystem *tilter;
	static CompressorSubsystem *compressor;
	static HopperSubsystem *hopper;
	static CameraSubsystem *camera;
	static OI *oi;
};

#endif
