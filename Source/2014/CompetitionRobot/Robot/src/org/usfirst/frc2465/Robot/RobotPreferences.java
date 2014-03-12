/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.usfirst.frc2465.Robot;

import edu.wpi.first.wpilibj.Preferences;

/**
 *
 * @author Scott
 * 
 * Defines Robot Preferences, which can be edited on the dashboard and stored
 * in robot flash memory for use every time the robot starts.
 * 
 * See http://wpilib.screenstepslive.com/s/3120/m/7932/l/81114-setting-robot-preferences-from-smartdashboard
 * for details on how to edit the preferences from the dashboard.
 */
public class RobotPreferences {
    
    /* Arm Preferences */
    
    // Physical Limits
    
    static public double getArmsMaxSensorVolts() {
        return Preferences.getInstance().getDouble("ArmsMaxSensorVolts",1.8);
    }
    static public double getArmsMinSensorVolts() {
        return Preferences.getInstance().getDouble("ArmsMinSensorVolts",0.27);        
    }
    
    // PID Controller Settings
    
    static public double getArmsOnTargetToleranceVolts() {
        return Preferences.getInstance().getDouble("ArmsOnTargetToleranceVolts", 0.1 );
    }
    static public double getArmsPGrab() {
        return Preferences.getInstance().getDouble("ArmsGrabP", 0.7);
    }
    static public double getArmsIGrab() {
        return Preferences.getInstance().getDouble("ArmsGrabI", 0.0);
    }
    static public double getArmsDGrab() {
        return Preferences.getInstance().getDouble("ArmsGrabD", 0.0);
    }
    static public double getArmsPLift() {
        return Preferences.getInstance().getDouble("ArmsLiftP", 0.7);
    }
    static public double getArmsILift() {
        return Preferences.getInstance().getDouble("ArmsLiftI", 0.01);
    }
    static public double getArmsDLift() {
        return Preferences.getInstance().getDouble("ArmsLiftD", 0.0);
    }
    static public double getArmsPRetract() {
        return Preferences.getInstance().getDouble("ArmsRetractP", 0.7);
    }
    static public double getArmsIRetract() {
        return Preferences.getInstance().getDouble("ArmsRetractI", 0.01);
    }
    static public double getArmsDRetract() {
        return Preferences.getInstance().getDouble("ArmsRetractD", 0.0);
    }
    static public double getArmsPHandoff() {
        return Preferences.getInstance().getDouble("ArmsHandoffP", 0.7);
    }
    static public double getArmsIHandoff() {
        return Preferences.getInstance().getDouble("ArmsHandoffI", 0.01);
    }
    static public double getArmsDHandoff() {
        return Preferences.getInstance().getDouble("ArmsHandoffD", 0.0);
    }
    
    // States
    
    static public double getArmsDefaultVolts() {
        return Preferences.getInstance().getDouble("ArmsVoltsDefault",
                                                    getArmsVoltsUp());
    }
    static public double getArmsVoltsDown() {
        return Preferences.getInstance().getDouble("ArmsVoltsDown",getArmsMinSensorVolts());        
    }
    static public double getArmsVoltsTeeup() {
        return Preferences.getInstance().getDouble("ArmsVoltsTeeup",0.8);        
    }
    static public double getArmsVoltsHandoff() {
        return Preferences.getInstance().getDouble("ArmsVoltsHandoff",0.7);        
    }
    static public double getArmsVoltsUp() {
        return Preferences.getInstance().getDouble("ArmsVoltsUp",1.8);        
    }
    
    //
    // The jog amount is to move the arms just a little, and is used primarily for diagnostics
    // and development purposes
    //
    
    static public double getArmsJogAmount() {
        return Preferences.getInstance().getDouble ("ArmsJogAmount", 0.1);
    }

    
    /* Tensioner Preferences */
    
    // Physical Limits
    
    static public double getTensionerMinVolts() {
        return Preferences.getInstance().getDouble("TensionerMinVolts",1.02);
    }
    static public double getTensionerMaxVolts() {
        return Preferences.getInstance().getDouble("TensionerMaxVolts",3.8);
    }

    // PID Controller Settings
    
    static public double getTensionerP() {
        return Preferences.getInstance().getDouble("TensionerP", 1.0);
    }
    static public double getTensionerI() {
        return Preferences.getInstance().getDouble("TensionerI", 0.4);
    }
    static public double getTensionerD() {
        return Preferences.getInstance().getDouble("TensionerD", 0.0);
    }    
    static public double getTensionerOnTargetToleranceVolts() {
        return Preferences.getInstance().getDouble("TensionerOnTargetToleranceVolts", 0.02);
    }
    
    // States
    
     static public double getTensionerSlackVolts() {
        return Preferences.getInstance().getDouble("TensionerSlackVolts", getTensionerMinVolts());
    }
    static public double getTensionerDefaultVolts() {
        return Preferences.getInstance().getDouble("TensionerDefaultVolts", getTensionerSlackVolts());
    }

    /* LegPreferences */
    
    static public double getLegMotorSpeed() {
        return Preferences.getInstance().getDouble("LegMotorSpeed", 1.0);
    }
    
    /* Drive Preferences */
    
    // Physical Limits
    
    
    
    // PID Controller Settings
    
    static public double getAutoRotateP() {
        return Preferences.getInstance().getDouble("AutoRotateP", 0.012);
    }
    static public double getAutoRotateI() {
        return Preferences.getInstance().getDouble("AutoRotateI", 0.0005);
    }
    static public double getAutoRotateD() {
        return Preferences.getInstance().getDouble("AutoRotateD", 0.0);
    }
    static public double getAutoRotateOnTargetToleranceDegrees() {
        return Preferences.getInstance().getDouble("AutoRotateOnTargetToleranceDegrees",1.0);
    }
    
    //States
    
    static public double getAutoRotateDefaultTargetDegrees() {
        return Preferences.getInstance().getDouble("AutoRotateDefaultTargetDegrees",0.0);
    }
   
    /* Ball Present Proximity Sensor */
    
    //
    // If the ball present proximity sensor returns a value within these ranges, then the
    // ball is present on the tee
    //
    
    static public double getBallPresentProximityMin()
    {
        return Preferences.getInstance().getDouble ("BallPresentProximityMin", 20.0);
    }
    
    static public double getBallPresentProximityMax()
    {
        return Preferences.getInstance().getDouble ("BallPresentProximityMax", 150.0);
    }
    
    /* Tensioner Ranging */
    // Distance (in feet) to voltage
    // To calculate voltage, v = Multiplier * exp( Exponent * x )
    
    static public double getTensionFeetToVoltsMultipler()
    {
        return Preferences.getInstance().getDouble( "TensionFeetToVoltsMultiplier", .5);
    }
    
    static public double getTensionFeedToVoltsExponent()
    {
        return Preferences.getInstance().getDouble( "TensionFeetToVoltsExponent", .4 );
    }
}
