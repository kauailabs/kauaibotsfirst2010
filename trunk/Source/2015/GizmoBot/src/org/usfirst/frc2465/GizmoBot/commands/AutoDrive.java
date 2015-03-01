package org.usfirst.frc2465.GizmoBot.commands;

import org.usfirst.frc2465.GizmoBot.Robot;

import edu.wpi.first.wpilibj.command.Command;

public class AutoDrive extends Command {
    double m_fwd;
    double m_strafe;
    double m_rotate;
    double m_distance_cm;
    boolean m_auto_rotate;
    double m_auto_rotate_angle;
    double m_previous_setpoint;
    boolean m_previous_autorotate;
    
    public AutoDrive(double fwd, double strafe, double rotate) {
        requires(Robot.drive);
        m_fwd = fwd;
        m_strafe = strafe;
        m_rotate = rotate;
        m_distance_cm = 0.0;
        m_auto_rotate = false;
        m_auto_rotate_angle = 0.0;
        
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
    }

    public AutoDrive(double fwd, double strafe, double rotate, double distance_cm) {
        requires(Robot.drive);
        m_fwd = fwd;
        m_strafe = strafe;
        m_rotate = rotate;
        m_distance_cm = distance_cm;        
        m_auto_rotate = false;
        m_auto_rotate_angle = 0.0;
        
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
    }

    public AutoDrive(double fwd, double strafe, double rotate, double distance_cm, double auto_rotate_angle) {
        requires(Robot.drive);
        m_fwd = fwd;
        m_strafe = strafe;
        m_rotate = rotate;
        m_distance_cm = distance_cm;        
        m_auto_rotate = true;
        m_auto_rotate_angle = auto_rotate_angle;
        // Use requires() here to declare subsystem dependencies
        // eg. requires(chassis);
    }

    // Called just before this Command runs the first time
    protected void initialize() {
    	if ( m_auto_rotate ) {
    		m_previous_autorotate = Robot.drive.getAutoRotation();
    		m_previous_setpoint = Robot.drive.getSetpoint();
    		Robot.drive.setAutoRotation(true);
    		Robot.drive.setSetpoint(m_auto_rotate_angle);
    	}
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
        
    	if ( m_auto_rotate ) {
    		Robot.drive.setAutoRotation(m_previous_autorotate);
    		Robot.drive.setSetpoint(m_previous_setpoint);
    	}
        Robot.drive.doMecanum(0, 0, 0);
        
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted(){
        
    	if ( m_auto_rotate ) {
    		Robot.drive.setAutoRotation(m_previous_autorotate);
    		Robot.drive.setSetpoint(m_previous_setpoint);
    	}
        Robot.drive.doMecanum(0, 0, 0);
            
    }
}
