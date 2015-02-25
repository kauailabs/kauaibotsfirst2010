package org.usfirst.frc2465.GizmoBot.commands;

import org.usfirst.frc2465.GizmoBot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class AutoDrive extends Command {
    double m_fwd;
    double m_strafe;
    double m_rotate;
    double m_distance_cm;
    
    public AutoDrive(double fwd, double strafe, double rotate) {
        requires(Robot.drive);
        m_fwd = fwd;
        m_strafe = strafe;
        m_rotate = rotate;
        m_distance_cm = 0.0;
                
        
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
    }

    public AutoDrive(double fwd, double strafe, double rotate, double distance_cm) {
        requires(Robot.drive);
        m_fwd = fwd;
        m_strafe = strafe;
        m_rotate = rotate;
        m_distance_cm = distance_cm;        
        
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
    }

    // Called just before this Command runs the first time
    protected void initialize() {
    }

    // Called repeatedly when this Command is scheduled to run
    protected void execute() {
        
        Robot.drive.doMecanum(m_strafe, m_fwd, m_rotate);
    }

    // Make this return true when this Command no longer needs to run execute()
    protected boolean isFinished() {
    	if ( m_distance_cm > 0.0 ) {
    		if ( Robot.lidar.getDistance() >= m_distance_cm ) {
    			return true;
    		}
    	}
    	return false;
    }

    // Called once after isFinished returns true
    protected void end() {
        
        Robot.drive.doMecanum(0, 0, 0);
        
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted(){
        
            Robot.drive.doMecanum(0, 0, 0);
            
    }
}
