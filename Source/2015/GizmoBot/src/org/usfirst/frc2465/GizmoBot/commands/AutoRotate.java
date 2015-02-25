package org.usfirst.frc2465.GizmoBot.commands;

import org.usfirst.frc2465.GizmoBot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class AutoRotate extends Command {
    double m_target_angle;
    double m_previous_setpoint;
    
    public AutoRotate(double target_angle) {
        requires(Robot.drive);
        m_target_angle = target_angle;                
        
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
    }

    // Called just before this Command runs the first time
    protected void initialize() {
    	m_previous_setpoint = Robot.drive.getSetpoint();
    	Robot.drive.setSetpoint(m_target_angle);
    	Robot.drive.setAutoRotation(true);
    }

    // Called repeatedly when this Command is scheduled to run
    protected void execute() {
        
        Robot.drive.doMecanum(0.0, 0.0, 0.0);
    }

    // Make this return true when this Command no longer needs to run execute()
    protected boolean isFinished() {
        return Robot.drive.onTarget();
    }

    // Called once after isFinished returns true
    protected void end() {
        
    	Robot.drive.setAutoRotation(false);
        Robot.drive.doMecanum(0, 0, 0);
    	Robot.drive.setSetpoint(m_previous_setpoint);       
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted(){
        
		Robot.drive.setAutoRotation(false);
        Robot.drive.doMecanum(0, 0, 0);
    	Robot.drive.setSetpoint(m_previous_setpoint);       
            
    }
}
