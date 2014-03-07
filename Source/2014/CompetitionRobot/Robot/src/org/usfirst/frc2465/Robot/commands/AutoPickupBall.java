/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.usfirst.frc2465.Robot.commands;

import edu.wpi.first.wpilibj.command.CommandGroup;

/**
 *
 * @author Scott
 */
public class AutoPickupBall extends CommandGroup {
    AutoPickupBall() 
    {
        addSequential( new ArmsDown() );
        addSequential( new ArmsTeeUp() );
        // TODO:  Wait up to 2 seconds for Ball Present
        addSequential( new ArmsUp() );
    }
}
