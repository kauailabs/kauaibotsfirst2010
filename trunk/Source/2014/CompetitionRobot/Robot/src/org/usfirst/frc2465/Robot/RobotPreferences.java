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
    
    static public double getArmsMaxAngle() { 
        return Preferences.getInstance().getDouble("ArmsMaxAngle",110.0);
    }
    static public double getArmsMinAngle() {
        return Preferences.getInstance().getDouble("ArmsMinAngle",0);
    }
    static public double getArmsMaxSensorVolts() {
        return Preferences.getInstance().getDouble("ArmsMaxSensorVolts",1.6);
    }
    static public double getArmsMinSensorVolts() {
        return Preferences.getInstance().getDouble("ArmsMinSensorVolts",3.1);        
    }
    static public double getArmsAngleLow() {
        return Preferences.getInstance().getDouble("ArmsAngleLow",0.0);        
    }
    static public double getArmsAngleMid() {
        return Preferences.getInstance().getDouble("ArmsAngleMid",45.0);        
    }
    static public double getArmsAngleHigh() {
        return Preferences.getInstance().getDouble("ArmsAngleHigh",90.0);        
    }
    static public double getArmsAngleUp() {
        return Preferences.getInstance().getDouble("ArmsAngleUp",110.0);        
    }
    static public double getArmsP() {
        return Preferences.getInstance().getDouble("ArmsP", 0.1);
    }
    static public double getArmsI() {
        return Preferences.getInstance().getDouble("ArmsI", 0.01);
    }
    static public double getArmsD() {
        return Preferences.getInstance().getDouble("ArmsD", 0.0);
    }

    /* Ankle Preferences */
    
    static public double getAnkleP() {
        return Preferences.getInstance().getDouble("AnkleP", 0.1);
    }
    static public double getAnkleI() {
        return Preferences.getInstance().getDouble("AnkleI", 0.01);
    }
    static public double getAnkleD() {
        return Preferences.getInstance().getDouble("AnkleD", 0.0);
    }
    static public double getAnkleHeightInches() {
        return Preferences.getInstance().getDouble("AnkleHeightInches",0.0);
    }
    static public double getAnkleBaselineInches() {
        return Preferences.getInstance().getDouble("AnkleBaselineInches",6.0);
    }
    static public double getAnkleDefaultAngle() {
        return Preferences.getInstance().getDouble("AnkleDefaultAngle",45.0);        
    }
    /* Tensioner Preferences */
    
    static public double getTensionerP() {
        return Preferences.getInstance().getDouble("TensionerP", 0.1);
    }
    static public double getTensionerI() {
        return Preferences.getInstance().getDouble("TensionerI", 0.01);
    }
    static public double getTensionerD() {
        return Preferences.getInstance().getDouble("TensionerD", 0.0);
    }    
    static public double getTensionerDefaultDistanceInches() {
        return Preferences.getInstance().getDouble("TensionerDefaultDistanceInches",6.0);
    }
    static public double getTensionerMinDistanceInches() {
        return Preferences.getInstance().getDouble("TensionerMinDistanceInches",0.0);
    }
    static public double getTensionerMaxDistanceInches() {
        return Preferences.getInstance().getDouble("TensionerMaxDistanceInches",12.0);
    }
    
}
