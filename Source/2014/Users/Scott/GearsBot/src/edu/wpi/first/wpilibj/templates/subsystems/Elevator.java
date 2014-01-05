package edu.wpi.first.wpilibj.templates.subsystems;

import edu.wpi.first.wpilibj.AnalogChannel;
import edu.wpi.first.wpilibj.Jaguar;
import edu.wpi.first.wpilibj.SpeedController;
import edu.wpi.first.wpilibj.command.PIDSubsystem;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.templates.RobotMap;

/**
 * <p>Another PID subsystem. Nothing special about this one.</p>
 *
 * <p>Related subsystem: {@link Wrist}</p>
 * 
 * @author Alex Henning
 */
public class Elevator extends PIDSubsystem {
    // The constants for the P, I and D portion of PID
    private static final double Kp = 6;
    private static final double Ki = 0.0;
    private static final double Kd = 0.0;
    
    public static final double BOTTOM = 4.6,
            STOW = 1.65,
            TABLE_HEIGHT = 1.58;
    
    SpeedController motor;
    AnalogChannel pot;

    // Initialize your subsystem here
    public Elevator() {
        super("Elevator", Kp, Ki, Kd);
        
        motor = new Jaguar(RobotMap.elevatorMotor);
        pot = new AnalogChannel(RobotMap.elevatorPot);

        // Set default position and start PID
        setSetpoint(STOW);
        enable();
    }
    
    /**
     * @return The value of the potentiometer used as the PID input device.
     *         These values correspond to your PID setpoint, in this case the
     *         value can be anywhere between 0v and 5v.
     */
    protected double returnPIDInput() {
        return pot.getVoltage();
    }
    
    /**
     * @param output The value to set the output to as determined by the PID
     *               algorithm. This gets called each time through the PID loop
     *               to update the output to the motor.
     */
    protected void usePIDOutput(double output) {
        motor.set(output);
    }
    
    /**
     * No default command is necessary because PID runs by default in the backround.
     */
    protected void initDefaultCommand() {}
}
