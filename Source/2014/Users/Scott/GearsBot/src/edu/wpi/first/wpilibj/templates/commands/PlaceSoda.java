package edu.wpi.first.wpilibj.templates.commands;

import edu.wpi.first.wpilibj.command.CommandGroup;
import edu.wpi.first.wpilibj.templates.subsystems.Elevator;
import edu.wpi.first.wpilibj.templates.subsystems.Wrist;

/**
 * <p>This command group:</p>
 * <ol>
 * <li>Moves the elevator to table height and sets the wrist to place</li>
 * <li>Opens the claw</li>
 * </ol>
 * 
 * <p>Related classes: {@link PrepareToGrab}, {@link Grab} and {@link Stow}
 * 
 * @author Alex Henning
 */
public class PlaceSoda extends CommandGroup {
    public PlaceSoda() {
        addParallel(new SetElevatorSetpoint(Elevator.TABLE_HEIGHT));
        addSequential(new SetWristSetpoint(Wrist.PICKUP));
        addSequential(new OpenClaw());
    }
}
