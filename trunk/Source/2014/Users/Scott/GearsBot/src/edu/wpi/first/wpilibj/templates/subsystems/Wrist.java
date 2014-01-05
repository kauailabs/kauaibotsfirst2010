package edu.wpi.first.wpilibj.templates.subsystems;

import edu.wpi.first.wpilibj.AnalogChannel;
import edu.wpi.first.wpilibj.Victor;
import edu.wpi.first.wpilibj.command.PIDSubsystem;
import edu.wpi.first.wpilibj.smartdashboard.SmartDashboard;
import edu.wpi.first.wpilibj.templates.RobotMap;
/**
 * <p>A simple PID subsystem, it enables PID and leaves always on. The PID
 * control loop runs asynchronously in the background, so that once you set a
 * setpoint. It's only is setSetpoint which changes the setpoint the PID is
 * trying to get to. Our PID Input is a potentiometer hooked up to the wrist and
 * our PID output is a motor that drives the wrist. For more information on PID
 * see: https://en.wikipedia.org/wiki/PID_controller If you're having trouble, 
 * just start with the</p>
 * 
 * <p>This subsystem also declares two constants that represent when the wrist
 * at the right height trying to pickup and when th wrist is in stowed position.
 * These can be used as known setpoints. wrist.setSetpoint(Wrist.STOW) would
 * stow the wrist</p>
 * 
 * <p>Recommended next step: {@link DriveTrain}<br/>
 * Related subsystem: {@link Elevator}</p>
 * 
 * @author Alex Henning
 */
public class Wrist extends PIDSubsystem {
    // The constants for the P, I and D portion of PID
    private static final double Kp = -2;
    private static final double Ki = 0.0;
    private static final double Kd = 0.0;
    
    // Constants for preset positions of the wrist
    public static final double PICKUP = 3.5,
            STOW = 1.75;
    
    Victor motor;
    AnalogChannel pot;

    // Initialize your subsystem here
    public Wrist() {
        super("Wrist", Kp, Ki, Kd);
        motor = new Victor(RobotMap.wristMotor);
        pot = new AnalogChannel(RobotMap.wristPot);
        
        // Set the starting setpoint and have PID start running in the background
        setSetpoint(STOW);
        enable(); // - Enables the PID controller.
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
