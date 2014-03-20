/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package org.usfirst.frc2465.Robot.commands;

import edu.wpi.first.wpilibj.command.Command;
import org.usfirst.frc2465.Robot.Robot;

/**
 *
 * @author Robotics
 */
public class AutoDrive extends Command {
    double m_fwd;
    double m_strafe;
    double m_rotate;
    
    public AutoDrive(double fwd, double strafe, double rotate) {
        requires(Robot.drive);
        m_fwd = fwd;
        m_strafe = strafe;
        m_rotate = rotate;
                
        
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
