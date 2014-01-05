package edu.wpi.first.wpilibj.templates.commands;

/**
 * <p>This command sets the setpoint for the wrist and waits until it's close
 * enough to the desired setpoint before quitting this command.</p>
 * 
 * <p>Recommended next step: {@link DriveWithJoysticks}<br/>
 * Related command: {@link SetElevatorSetpoint}</p>
 *
 * @author Alex Henning
 */
public class SetWristSetpoint extends CommandBase {
    private double setpoint;
   
    /**
     * Require the wrist and store the desired setpoint.
     * 
     * @param setpoint The desired setpoint for the wrist.
     */
    public SetWristSetpoint(double setpoint) {
        requires(wrist);
        this.setpoint = setpoint;
    }

    // Called just before this Command runs the first time
    /**
     * Set the wrist's setpoint to the desired setpoint
     */
    protected void initialize() {
        wrist.setSetpoint(setpoint);
    }

    // Called repeatedly when this Command is scheduled to run
    /**
     * Do nothing, the PID loop is running in the background
     */
    protected void execute() {
    }

    // Make this return true when this Command no longer needs to run execute()
    /**
     * @return true if the wrist is close enough to in position.
     */
    protected boolean isFinished() {
        return Math.abs(wrist.getPosition() - setpoint) < .08;
    }

    // Called once after isFinished returns true
    protected void end() {
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted() {
    }
}
