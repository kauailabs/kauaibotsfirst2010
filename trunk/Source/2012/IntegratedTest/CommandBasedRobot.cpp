#include "WPILib.h"
#include "Commands/Command.h"
#include "Commands/Gobbler.h"
#include "Commands/Drive.h"
#include "Commands/Chute.h"
#include "Commands/Tilter.h"
#include "Commands/Hopper.h"
#include "Commands/Camera.h"
#include "Commands/TrafficCop.h"
#include "Commands/AutonomousPlayRight.h"
#include "Commands/AutonomousPlayLeft.h"
#include "Commands/AutonomousPlayStraight.h"
#include "CommandBase.h"

class CommandBasedRobot : public IterativeRobot {
private:
	Command *autonomousCommand;
	SendableChooser autoChooser;
	
	virtual void RobotInit() {
		CommandBase::init();
		
		//NetworkTable::Initialize();
		
		SmartDashboard*sd=SmartDashboard::GetInstance();
		
		//sd->PutData(CommandBase::chute);
		//sd->PutData(CommandBase::gobbler);
		//sd->PutData(CommandBase::drive);
		//sd->PutData(CommandBase::hopper);
		//sd->PutData(CommandBase::tilter);
		//sd->PutData(CommandBase::camera);
		//sd->PutData(CommandBase::trafficcop);
		
		autoChooser.AddDefault(	"DunkRight",	new AutonomousPlayRight());
		autoChooser.AddObject(	"DunkLeft",		new AutonomousPlayLeft());
		autoChooser.AddObject(	"DunkStraight",	new AutonomousPlayStraight());
		
		sd->PutData("Autonomous Mode Chooser", &autoChooser);
		
		autonomousCommand = 0;
	}
	
	virtual void AutonomousInit() {
		
		// Set the iteration time period to 20 ms; this is more 
		// accurate than the default period (0.0), which executes
		// each iteration whenever a new packet is received from
		// the drive station.
		
		SetPeriod(0.02);
		autonomousCommand = (Command*)autoChooser.GetSelected();
		
		// Start() will schedule the command for execution.
		// However, the command will not actually begin 
		// execution until the Scheduler is run (see AutonomousPeriodic()).
		
		autonomousCommand->Start();
	}
	
	virtual void AutonomousPeriodic() {
		Scheduler::GetInstance()->Run();
	}
	
	virtual void TeleopInit() {

		// Set the iteration time period to "0.0" ms.  This special
		// value means that each teleop iteration (see TeleopPeriodic())
		// will execute whenever a new packet is received from
		// the drive station.  While not as accurate timing-wise
		// as a non-zero period, this assures the fastest possible
		// response to commands (e.g., joystick) from the driver station.
		
		SetPeriod(0.0);
		
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to 
		// continue until interrupted by another command, remove
		// this line or comment it out.
		
		if ( autonomousCommand )
		{
			autonomousCommand->Cancel();
			autonomousCommand = 0;
		}
		
	}
	
	virtual void TeleopPeriodic() {
		Scheduler::GetInstance()->Run();
	}
};

START_ROBOT_CLASS(CommandBasedRobot);

