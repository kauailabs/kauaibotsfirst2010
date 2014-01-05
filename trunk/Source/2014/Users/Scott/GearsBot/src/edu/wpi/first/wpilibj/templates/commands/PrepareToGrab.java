package edu.wpi.first.wpilibj.templates.commands;

import edu.wpi.first.wpilibj.command.CommandGroup;
import edu.wpi.first.wpilibj.templates.subsystems.Elevator;
import edu.wpi.first.wpilibj.templates.subsystems.Wrist;

/**
 * <p>This is a group of commands that does the following three things
 * simultaneously:</p>
 * <ul>
 * <li>Sets the wrist setpoint to grab</li>
 * <li>Sets the elevator setpoint to the bottom</li>
 * <li>Opens the claw</li>
 * </ul>
 *
 * <p>Recommended next step: {@link SodaDelivery}<br/>
 * Related classes: {@link Grab}, {@link PlaceSoda} and {@link Stow}
 * 
 * @author Alex Henning 
 */
public class PrepareToGrab extends CommandGroup {
    public PrepareToGrab() {
        addParallel(new SetWristSetpoint(Wrist.PICKUP));
        addParallel(new SetElevatorSetpoint(Elevator.BOTTOM));
        addParallel(new OpenClaw());
    }
}
