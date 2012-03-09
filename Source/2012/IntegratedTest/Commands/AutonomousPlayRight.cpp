#include "AutonomousPlayRight.h"
#include "ShootNow.h"
#include "DriveDistance.h"

AutonomousPlayRight::AutonomousPlayRight() {
        // Add Commands here:
        // e.g. AddSequential(new Command1());
        //      AddSequential(new Command2());
        // these will run in order.
		AddSequential(new DriveDistance(false,-10,100,false));
		AddSequential(new DriveDistance(true,-100,75,true));
		AddParallel(new ShootNow(true, 16.0));
		AddSequential(new DriveDistance(false,10,125,false));
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
