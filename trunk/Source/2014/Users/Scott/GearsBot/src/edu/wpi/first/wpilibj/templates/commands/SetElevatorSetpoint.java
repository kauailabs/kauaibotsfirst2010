package edu.wpi.first.wpilibj.templates.commands;

/**
 * <p>Same idea as SetWristSetpoint except for the elevator instead of the wrist.</p>
 * 
 * <p>Related command: {@link SetWristSetpoint}</p>
 * 
 * @author Alex Henning
 */
public class SetElevatorSetpoint extends CommandBase {
    private double setpoint;
   
    /**
     * Require the elevator and store the desired setpoint.
     * 
     * @param setpoint The desired setpoint for the elevator.
     */
    public SetElevatorSetpoint(double setpoint) {
        requires(elevator);
        this.setpoint = setpoint;
    }

    // Called just before this Command runs the first time
    /**
     * Set the wrist's setpoint to the desired setpoint
     */
    protected void initialize() {
        elevator.setSetpoint(setpoint);
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
        return Math.abs(elevator.getPosition() - setpoint) < .05;
    }

    // Called once after isFinished returns true
    protected void end() {
    }

    // Called when another command which requires one or more of the same
    // subsystems is scheduled to run
    protected void interrupted() {
    }
}
