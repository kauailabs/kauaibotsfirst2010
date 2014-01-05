package edu.wpi.first.wpilibj.templates.commands;

import edu.wpi.first.wpilibj.command.CommandGroup;
import edu.wpi.first.wpilibj.templates.OI;

/**
 * <p>This command group combines commands and other command groups to perform
 * a relatively complicated autonomous very robustly. This autonomous:</p>
 * <ol>
 * <li>Prepares to grab the soda</li>
 * <li>Grabs the soda</li>
 * <li>Drive to the table</li>
 * <li>Place the soda</li>
 * <li>Back away from the table</li>
 * <li>Stow the entire arm</li>
 * </ol>
 * 
 * <p>Recommended next step: {@link OI}</p>
 *
 * @author Alex Henning
 */
public class SodaDelivery extends CommandGroup {
    public SodaDelivery() {
        addSequential(new PrepareToGrab());
        addSequential(new Grab());
        addSequential(new DriveToDistance(.11));
        addSequential(new PlaceSoda());
        addSequential(new DriveToDistance(.2));
        addSequential(new Stow());
    }
}
