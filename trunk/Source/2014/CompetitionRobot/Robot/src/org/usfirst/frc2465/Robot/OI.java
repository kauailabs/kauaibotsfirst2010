// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// Java from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


package org.usfirst.frc2465.Robot;

import org.usfirst.frc2465.Robot.commands.*;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.*;
import edu.wpi.first.wpilibj.buttons.*;


/**
 * This class is the glue that binds the controls on the physical operator
 * interface to the commands and command groups that allow control of the robot.
 */
public class OI {
    //// CREATING BUTTONS
    // One type of button is a joystick button which is any button on a joystick.
    // You create one by telling it which joystick it's on and which button
    // number it is.
    // Joystick stick = new Joystick(port);
    // Button button = new JoystickButton(stick, buttonNumber);
    
    // Another type of button you can create is a DigitalIOButton, which is
    // a button or switch hooked up to the cypress module. These are useful if
    // you want to build a customized operator interface.
    // Button button = new DigitalIOButton(1);
    
    // There are a few additional built in buttons you can use. Additionally,
    // by subclassing Button you can create custom triggers and bind those to
    // commands the same as any other Button.
    
    //// TRIGGERING COMMANDS WITH BUTTONS
    // Once you have a button, it's trivial to bind it to a button in one of
    // three ways:
    
    // Start the command when the button is pressed and let it run the command
    // until it is finished as determined by it's isFinished method.
    // button.whenPressed(new ExampleCommand());
    
    // Run the command while the button is being held down and interrupt it once
    // the button is released.
    // button.whileHeld(new ExampleCommand());
    
    // Start the command when the button is released  and let it run the command
    // until it is finished as determined by it's isFinished method.
    // button.whenReleased(new ExampleCommand());

    
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    public JoystickButton defaultTension;
    public JoystickButton detension;
    public JoystickButton armLowBtn;
    public JoystickButton armTeeupBtn;
    public JoystickButton armHandoffBtn;
    public JoystickButton armUpBtn;
    public JoystickButton zeroYawBtn;
    public JoystickButton manualTensionEnableBtn;
    public Joystick driverJoystick;
    public JoystickButton rotateToTargetBtn;
    public JoystickButton fireBtn;
    public JoystickButton aimBtn;
    public JoystickButton readyBtn;
    public Joystick shooterJoystick;
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    public OI() {
        // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS

        shooterJoystick = new Joystick(2);
        
        defaultTension = new JoystickButton(shooterJoystick,5);
        defaultTension.whenPressed(new DefaultTension());
        detension = new JoystickButton(shooterJoystick,3);
        detension.whenPressed(new Detension());
        readyBtn = new JoystickButton(shooterJoystick, 4);
        readyBtn.whenPressed(new PrepareToKick());
        aimBtn = new JoystickButton(shooterJoystick, 10);
        aimBtn.whileHeld(new Aim());
        fireBtn = new JoystickButton(shooterJoystick, 1);
        fireBtn.whenPressed(new KickAndReload());
        rotateToTargetBtn = new JoystickButton(shooterJoystick, 11);
        rotateToTargetBtn.whileHeld(new RotateToTarget());
        manualTensionEnableBtn = new JoystickButton(shooterJoystick,2);
        manualTensionEnableBtn.whileHeld(new ManualTension());
        driverJoystick = new Joystick(1);
        
        armUpBtn = new JoystickButton(driverJoystick, 9);
        armUpBtn.whenPressed(new ArmsUp());
        armHandoffBtn = new JoystickButton(driverJoystick, 8);
        armHandoffBtn.whenPressed(new ArmsHandoff());
        armTeeupBtn = new JoystickButton(driverJoystick, 7);
        armTeeupBtn.whenPressed(new ArmsTeeUp());
        armLowBtn = new JoystickButton(driverJoystick, 6);
        armLowBtn.whenPressed(new ArmsDown());

	    
        // SmartDashboard Buttons
        SmartDashboard.putData("Autonomous", new Autonomous());

        SmartDashboard.putData("RotateToTarget", new RotateToTarget());

        SmartDashboard.putData("Ready", new PrepareToKick());

        SmartDashboard.putData("Aim", new Aim());

        SmartDashboard.putData("Fire", new Kick(true));

        SmartDashboard.putData("ArmLow", new ArmsDown());

        SmartDashboard.putData("ArmMid", new ArmsTeeUp());

        SmartDashboard.putData("ArmHigh", new ArmsHandoff());

        SmartDashboard.putData("ArmUp", new ArmsUp());

        SmartDashboard.putData("ZeroYaw", new ZeroYaw());

        SmartDashboard.putData("DefaultTension", new DefaultTension());
        
        SmartDashboard.putData("Detension", new Detension());
        
        // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
    }
    
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS
    public Joystick getDriverJoystick() {
        return driverJoystick;
    }

    public Joystick getShooterJoystick() {
        return shooterJoystick;
    }

    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=FUNCTIONS
}

