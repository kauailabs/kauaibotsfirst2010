// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// Java from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


package org.usfirst.frc2465.Robot.commands;

import edu.wpi.first.wpilibj.DriverStation;
import edu.wpi.first.wpilibj.command.Command;
import org.usfirst.frc2465.Robot.Robot;
import org.usfirst.frc2465.Robot.RobotPreferences;

/**
 *
 */
public class  WaitForCameraHot extends Command {

    public WaitForCameraHot() {
        // Use requires() here to declare subsystem dependencies
        requires(Robot.camera);
	
        // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
        // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=REQUIRES
    }

    // Called just before this Command runs the first time
    protected void initialize() {

    }

    // Called repeatedly when this Command is scheduled to run
    protected void execute() {
    }

    // Make this return true when this Command no longer needs to run execute()
    protected boolean isFinished() {
        if ( DriverStation.getInstance().getMatchTime() > 5.0 ) {
            return true;
        }
        return false;
    }

    // Called once after isFinished returns true
    protected void end() {
        // TODO:  stop camera processing
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted() {
        // TODO:  stop camera processing
    }
}
