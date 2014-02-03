// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// Java from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


package org.usfirst.frc2465.Robot.subsystems;

import com.sun.squawk.util.MathUtils;
import org.usfirst.frc2465.Robot.RobotMap;
import org.usfirst.frc2465.Robot.commands.*;
import edu.wpi.first.wpilibj.*;
import edu.wpi.first.wpilibj.command.PIDSubsystem;

import edu.wpi.first.wpilibj.command.Subsystem;
import org.usfirst.frc2465.Robot.RobotPreferences;


/**
 *
 */
public class Ankle extends PIDSubsystem {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    ProximitySensor angleSensor = RobotMap.ankleAngleSensor;
    DigitalInput angleMin = RobotMap.ankleAngleMin;
    DigitalInput angleMax = RobotMap.ankleAngleMax;
    SpeedController angleSC = RobotMap.ankleAngleSC;
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    public static double ANKLE_MIN_MOTOR_SPEED = -1;
    public static double ANKLE_MAX_MOTOR_SPEED = 1;
    
    public Ankle() {
        super(  "Ankle",
                RobotPreferences.getAnkleP(),
                RobotPreferences.getAnkleI(),
                RobotPreferences.getAnkleD());
        getPIDController().setContinuous(false);
        getPIDController().setInputRange(
                RobotPreferences.getAnkleMinAngle(), 
                RobotPreferences.getAnkleMaxAngle());
        getPIDController().setOutputRange(
                ANKLE_MIN_MOTOR_SPEED, 
                ANKLE_MAX_MOTOR_SPEED);
        getPIDController().setAbsoluteTolerance(
                RobotPreferences.getAnkleOnTargetToleranceDegrees());
        setSetpoint(
                RobotPreferences.getAnkleDefaultAngle());
        enable();
    }
    
    // Put methods for controlling this subsystem
    // here. Call these from Commands.

    public void initDefaultCommand() {
        // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
        // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DEFAULT_COMMAND
	
        // Set the default command for a subsystem here.
        //setDefaultCommand(new MySpecialCommand());
    }

    protected double returnPIDInput() {
        
        // Transform the distance read by the sensor
        // into an angle
        double x = angleSensor.getDistanceInches();

        // Calculate arc angle from tangent (given length of opposite and adjacent sides)
        double theta_radians = 
                MathUtils.atan2( (RobotPreferences.getAnkleHeightInches()+x),RobotPreferences.getAnkleBaselineInches());
        
        // Convert from Radians to Degrees
        double theta_degrees = theta_radians * (180.0/Math.PI);

        return theta_degrees;
    }

    
    protected void usePIDOutput(double d) {
        
        if ( angleMin.get() || angleMax.get() ) {
            d = 0;
        }
        
        angleSC.set(d);
        
    }
}
