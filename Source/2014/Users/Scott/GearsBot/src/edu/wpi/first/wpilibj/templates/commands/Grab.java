package edu.wpi.first.wpilibj.templates.commands;

import edu.wpi.first.wpilibj.command.CommandGroup;
import edu.wpi.first.wpilibj.templates.subsystems.Elevator;
import edu.wpi.first.wpilibj.templates.subsystems.Wrist;

/**
 * <p>This command group:</p>
 * <ol>
 * <li>Closes the claw and sets the elevator setpoint to stow</li>
 * <li>Sets the wrist setpoint to stow</li>
 * </ol>
 * 
 * <p>Related classes: {@link PrepareToGrab}, {@link PlaceSoda} and {@link Stow}
 * 
 * @author Alex Henning
 */
public class Grab extends CommandGroup {
    public Grab() {
        addSequential(new CloseClaw());
        addParallel(new SetElevatorSetpoint(Elevator.STOW));
        addSequential(new SetWristSetpoint(Wrist.STOW));
    }
}
