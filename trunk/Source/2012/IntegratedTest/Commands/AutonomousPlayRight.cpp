#include "AutonomousPlayRight.h"
#include "ShootNow.h"
#include "DriveDistance.h"
#include "WaitTime.h"

AutonomousPlayRight::AutonomousPlayRight() {
        // Add Commands here:
        // e.g. AddSequential(new Command1());
        //      AddSequential(new Command2());
        // these will run in order.
	
		AddSequential(new DriveDistance(false,-16,125,false));
		//AddSequential(new WaitTime(1.0));
		AddParallel(new ShootNow(true, 15.0));
		AddSequential(new DriveDistance(true,-80,125,false));
		//AddSequential(new WaitTime(1.0));
		AddSequential(new DriveDistance(false,16,125,false));
		
        // To run multiple commands at the same time,
        // use AddParallel()
        // e.g. AddParallel(new Command1());
        //      AddSequential(new Command2());
        // Command1 and Command2 will run in parallel.
		
        // A command group will require all of the subsystems that each member
        // would require.
        // e.g. if Command1 requires chassis, and Command2 requires arm,
        // a CommandGroup containing them would require both the chassis and the
        // arm.
}
