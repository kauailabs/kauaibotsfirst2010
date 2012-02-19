#include "TrafficCop.h"

TrafficCop::TrafficCop() 
{
        Requires(trafficcop);
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
}

// Called just before this Command runs the first time
void TrafficCop::Initialize() {
        
}

// Called repeatedly when this Command is scheduled to run

void TrafficCop::Execute() 
{
        bool bTrafficCopForward = oi->getDriverStation()->GetDigitalIn(7);
        bool bTrafficCopBack = oi->getDriverStation()->GetDigitalIn(8);

        if (bTrafficCopForward && bTrafficCopBack)
        {
                trafficcop->Stop();
                
        }
        else if (bTrafficCopForward)
        {
                trafficcop->Forward();
        }
        else if (bTrafficCopBack)
        {
                trafficcop->Back();
        }
        else
        {
                trafficcop->Off();
        }
}

// Make this return true when this Command no longer needs to run execute()
bool TrafficCop::IsFinished() {
        return false;
}

// Called once after isFinished returns true
void TrafficCop::End() {
        
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void TrafficCop::Interrupted() {
}
