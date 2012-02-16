
#ifndef GOBBLER_SUBSYSTEM_H
#define GOBBLER_SUBSYSTEM_H
#include "Commands/Subsystem.h"
#include "WPILib.h"

/**
 *
 *
 * @author
 */
class GobblerSubsystem: public Subsystem {
private:
        Victor* firstMotor;
        Victor* secondMotor;
        // It's desirable that everything possible under private except
        // for methods that implement subsystem capabilities
public:
        GobblerSubsystem();
        ~GobblerSubsystem();
        void InitDefaultCommand();
        
        void Forward();
        void Stop();
        void Reverse();
        
};

#endif 
