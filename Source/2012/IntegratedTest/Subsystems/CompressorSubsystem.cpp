#include "CompressorSubsystem.h"
#include "../Robotmap.h"

CompressorSubsystem::CompressorSubsystem() : Subsystem("CompressorSubsystem"), 
	c(PRESSURE_SWITCH_IN_CHANNEL,COMPRESSOR_RELAY_OUT_CHANNEL) 
{
	c.Start();
}
    
void CompressorSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}


// Put methods for controlling this subsystem
// here. Call these from Commands.
