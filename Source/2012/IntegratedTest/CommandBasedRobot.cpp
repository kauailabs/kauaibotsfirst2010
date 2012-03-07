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
		autoChooser.AddDefault("DunkRight",new AutonomousPlayRight());
		autoChooser.AddObject("DunkLeft",new AutonomousPlayLeft());
		sd->PutData("Autonomous Mode Chooser", &autoChooser);
		autonomousCommand = 0;
	}
	
	virtual void AutonomousInit() {
		SetPeriod(0.02);
		autonomousCommand = (Command*)autoChooser.GetSelected();
		autonomousCommand->Start();
	}
	
	virtual void AutonomousPeriodic() {
		Scheduler::GetInstance()->Run();
	}
	
	virtual void TeleopInit() {
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to 
		// continue until interrupted by another command, remove
		// this line or comment it out.
		SetPeriod(0.0);
		if ( autonomousCommand )
		{
			autonomousCommand->Cancel();
		}
	}
	
	virtual void TeleopPeriodic() {
		Scheduler::GetInstance()->Run();
	}
};

START_ROBOT_CLASS(CommandBasedRobot);

