// RobotBuilder Version: 1.5
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// Java from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


package org.usfirst.frc2465.GizmoBot;

import java.nio.ByteBuffer;
import java.nio.IntBuffer;

import edu.wpi.first.wpilibj.I2C;
import edu.wpi.first.wpilibj.IterativeRobot;
import edu.wpi.first.wpilibj.command.Command;
import edu.wpi.first.wpilibj.command.Scheduler;
import edu.wpi.first.wpilibj.hal.PDPJNI;
import edu.wpi.first.wpilibj.livewindow.LiveWindow;
import edu.wpi.first.wpilibj.smartdashboard.SendableChooser;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;

import org.usfirst.frc2465.GizmoBot.commands.*;
import org.usfirst.frc2465.GizmoBot.subsystems.*;
import org.usfirst.frc2465.GizmoBot.Robot;
import org.usfirst.frc2465.GizmoBot.RobotMap;
import org.usfirst.frc2465.GizmoBot.subsystems.Drive;

/**
 * The VM is configured to automatically run this class, and to call the
 * functions corresponding to each mode, as described in the IterativeRobot
 * documentation. If you change the name of this class or the package after
 * creating this project, you must also update the manifest file in the resource
 * directory.
 */
public class Robot extends IterativeRobot {

    Command autonomousCommand;
    SendableChooser autoChooser;

    public static OI oi;
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    public static Lift2 lift2;
    public static Lift1 lift1;
    public static Grab2 grab2;
    public static Grab1 grab1;
    public static Tilter tilter;
    public static StepLift stepLift;
    public static TopCam topCam;
    public static ArmCam armCam;
    public static ToteRanger toteRanger;
    public static Drive drive;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    private IntBuffer status;
    public static LIDAR lidar;
    
    /**
     * This function is run when the robot is first started up and should be
     * used for any initialization code.
     */
    public void robotInit() {
    RobotMap.init();
        // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
        lift1 = new Lift1();
        lift2 = new Lift2();
        grab2 = new Grab2();
        grab1 = new Grab1();
        tilter = new Tilter();
        stepLift = new StepLift();
        topCam = new TopCam();
        armCam = new ArmCam();
        toteRanger = new ToteRanger();
        drive = new Drive();

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
        // OI must be constructed after subsystems. If the OI creates Commands 
        //(which it very likely will), subsystems are not guaranteed to be 
        // constructed yet. Thus, their requires() statements may grab null 
        // pointers. Bad news. Don't move it.
        oi = new OI();

        // instantiate the command used for the autonomous period
        // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
        autonomousCommand = null;

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=AUTONOMOUS
        status = ByteBuffer.allocateDirect(4).asIntBuffer();
        try 
        {
        	lidar = new LIDAR(I2C.Port.kMXP);
	    	lidar.start();
    	}
        catch( Exception ex ) 
        {        	
        	lidar = null;
        }
        autoChooser = new SendableChooser();
        autoChooser.addDefault("ToteBinLeft", new AutonomousCommand(-90));
        autoChooser.addObject("ToteBinRight", new AutonomousCommand(90));
        autoChooser.addObject("Frozen", new Still());
        SmartDashboard.putData("Autonomous Chooser",autoChooser);

    }

    /**
     * This function is called when the disabled button is hit.
     * You can use it to reset subsystems before shutting down.
     */
    public void disabledInit(){
    	//lidar.stop();
    }

    public void disabledPeriodic() {
        Scheduler.getInstance().run();
        updateDashboard();
    }

    public void autonomousInit() {
        // schedule the autonomous command (example)
    	//lidar.start();
        if ( lidar != null )
    	{
	    	lidar.start();
    	}
        
        autonomousCommand = (Command) autoChooser.getSelected();
        autonomousCommand.start();        
    }
    
    /**
     * This function is called periodically during autonomous
     */
    public void autonomousPeriodic() {
        Scheduler.getInstance().run();
        updateDashboard();
    }

    public void teleopInit() {
        // This makes sure that the autonomous stops running when
        // teleop starts running. If you want the autonomous to 
        // continue until interrupted by another command, remove
        // this line or comment it out.
        if (autonomousCommand != null) autonomousCommand.cancel();
        //lidar.stop();
    }

    /**
     * This function is called periodically during operator control
     */
    public void teleopPeriodic() {
        Scheduler.getInstance().run();
        updateDashboard();
    }

    /**
     * This function is called periodically during test mode
     */
    public void testPeriodic() {
        LiveWindow.run();
    }
    public void updateDashboard() {
        // Drive
        
        if(RobotMap.imu != null)
        {
	    	SmartDashboard.putBoolean("IMU_Connected",      RobotMap.imu.isConnected());
	        SmartDashboard.putNumber( "IMU_Yaw",            RobotMap.imu.getYaw());
	        SmartDashboard.putNumber( "IMU_CompassHeading", RobotMap.imu.getCompassHeading());
        }
        
        if(Robot.drive != null)
        {
        	SmartDashboard.putBoolean("FOD_Enabled",        Robot.drive.getFODEnabled() );
        }
        if(Robot.lidar != null)
        {
        	SmartDashboard.putNumber("LIDAR_Distance_CM", Robot.lidar.getDistance());
        }
        SmartDashboard.putNumber( "Current_Total",			PDPJNI.getPDPTotalCurrent(status));
    }
}
