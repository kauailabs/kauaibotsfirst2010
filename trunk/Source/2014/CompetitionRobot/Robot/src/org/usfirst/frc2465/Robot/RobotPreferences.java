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
 */
public class RobotPreferences {
    
    /* Arm Preferences */
    
    static public double getMaxArmAngle() { 
        return Preferences.getInstance().getDouble("MaxArmAngle",110.0);
    }
    static public double getMinArmAngle() {
        return Preferences.getInstance().getDouble("MinArmAngle",0);
    }
    static public double getMaxArmSensorVolts() {
        return Preferences.getInstance().getDouble("MaxArmSensorVolts",1.6);
    }
    static public double getMinArmSensorVolts() {
        return Preferences.getInstance().getDouble("MinArmSensorVolts",3.1);        
    }
    static public double getLowArmAngle() {
        return Preferences.getInstance().getDouble("ArmAngleLow",0.0);        
    }
    static public double getMidArmAngle() {
        return Preferences.getInstance().getDouble("ArmAngleLow",45.0);        
    }
    static public double getHighArmAngle() {
        return Preferences.getInstance().getDouble("ArmAngleLow",90.0);        
    }
    static public double getUpArmAngle() {
        return Preferences.getInstance().getDouble("ArmAngleLow",110.0);        
    }
    
}
