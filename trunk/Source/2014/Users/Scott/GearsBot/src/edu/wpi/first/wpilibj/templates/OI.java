package edu.wpi.first.wpilibj.templates;

import edu.wpi.first.wpilibj.Joystick;
import edu.wpi.first.wpilibj.buttons.Button;
import edu.wpi.first.wpilibj.buttons.JoystickButton;
import edu.wpi.first.wpilibj.templates.commands.DriveToDistance;
import edu.wpi.first.wpilibj.templates.commands.Grab;
import edu.wpi.first.wpilibj.templates.commands.PrepareToGrab;
import edu.wpi.first.wpilibj.templates.commands.PlaceSoda;
import edu.wpi.first.wpilibj.templates.commands.SodaDelivery;
import edu.wpi.first.wpilibj.templates.commands.Stow;

/**
 * <p>The operator interface class ties the commands that have been implemented
 * to the physical controls of the user. This allows you to bind the same
 * command that executes autonomous to a button. It also reveal the joystick as
 * used by the DriveWithJoysticks command.</p>
 * 
 * @author Alex Henning
 */
public class OI {
    // Create the joystick and of the 8 buttons on it
    Joystick leftJoy = new Joystick(1);
    Button button1 = new JoystickButton(leftJoy, 1),
            button2 = new JoystickButton(leftJoy, 2),
            button3 = new JoystickButton(leftJoy, 3),
            button4 = new JoystickButton(leftJoy, 4),
            button5 = new JoystickButton(leftJoy, 5),
            button6 = new JoystickButton(leftJoy, 6),
            button7 = new JoystickButton(leftJoy, 7),
            button8 = new JoystickButton(leftJoy, 8);
    
    /**
     * Bind the press of each button to a specific command or command group.
     */
    public OI() {
        button1.whenPressed(new PrepareToGrab());
        button2.whenPressed(new Grab());
        button3.whenPressed(new DriveToDistance(0.11));
        button4.whenPressed(new PlaceSoda());
        button6.whenPressed(new DriveToDistance(0.2));
        button8.whenPressed(new Stow());
        
        button7.whenPressed(new SodaDelivery());
    }
    
    /**
     * @return The value of the left joystick.
     */
    public double getLeftSpeed() {
        return leftJoy.getY();
    }
    
    /**
     * @return The value of the right joystick. Note: this uses raw axis because
     *         we have a logitech joystick that resembles a PS controller.
     */
    public double getRightSpeed() {
        return leftJoy.getRawAxis(4);
    }
}

