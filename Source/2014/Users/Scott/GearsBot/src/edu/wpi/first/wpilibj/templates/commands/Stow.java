package edu.wpi.first.wpilibj.templates.commands;

import edu.wpi.first.wpilibj.command.CommandGroup;
import edu.wpi.first.wpilibj.templates.subsystems.Elevator;
import edu.wpi.first.wpilibj.templates.subsystems.Wrist;

/**
 * <p>This command group simultaneously:</p>
 * <ul>
 * <li>Closes the claw</li>
 * <li>Stows the elevator</li>
 * <li>Stows the wrist</li>
 * </ul>
 * 
 * <p>Related classes: {@link PrepareToGrab}, {@link PlaceSoda} and {@link Grab}
 * 
 * @author Alex Henning
 */
public class Stow extends CommandGroup {
    public Stow() {
        addParallel(new CloseClaw());
        addParallel(new SetElevatorSetpoint(Elevator.STOW));
        addParallel(new SetWristSetpoint(Wrist.STOW));
    }
}
