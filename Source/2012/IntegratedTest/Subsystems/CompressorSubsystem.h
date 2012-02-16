#ifndef COMPRESSORSUBSYSTEM_H
#define COMPRESSORSUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author guavacados
 */
class CompressorSubsystem: public Subsystem {
private:
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities
	Compressor c;
public:
	CompressorSubsystem();
	void InitDefaultCommand();
};

#endif
