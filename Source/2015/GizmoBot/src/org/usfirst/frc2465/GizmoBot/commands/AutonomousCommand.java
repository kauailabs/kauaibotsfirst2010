// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// Java from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


package org.usfirst.frc2465.GizmoBot.commands;

import org.usfirst.frc2465.GizmoBot.Robot;

import edu.wpi.first.wpilibj.command.CommandGroup;
import edu.wpi.first.wpilibj.command.WaitCommand;

/**
 *
 */
public class  AutonomousCommand extends CommandGroup {

    public AutonomousCommand(double final_rotation_angle) {
    	requires(Robot.stepLift);
    	requires(Robot.drive);
    	requires(Robot.tilter);
    	/* Top Arms Up to pick up bin and tilts back */
    	this.addParallel(new ZeroYaw());
    	//this.addParallel(new RaiseLift2(),1.0);
    	//this.addParallel(new RaiseStepLift());
    	this.addSequential(new TiltBack());
    	this.addSequential(new WaitCommand(1.0));
    	
    	/* Reverses back 390cm, auto-rotate to 0 degrees (to keep from drifting */
        this.addSequential(new AutoDrive(0.15,0,0,420.0),3.0);

    	/* rotate -90 degrees */
    	
    	this.addSequential(new AutoRotate(final_rotation_angle),2.0);
    	
    	/* Tilt Forward */
    	
    	this.addParallel(new AutoDrive(0,0,0),2.5);
        this.addSequential(new TiltForward(),2.5);
    	
    	this.addSequential(new AutoDrive(0,0,0),10.0);
    	//this.addSequential(new LowerLift2(), 0.5);
    	//this.addSequential(new CloseGrab2(), 0.5);
    	//this.addSequential(new RaiseLift2(), 0.5);
    	
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
	
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
        return false;
    }

    // Called once after isFinished returns true
    protected void end() {
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted() {
    }
}