// RobotBuilder Version: 1.0
//
// This file was generated by RobotBuilder. It contains sections of
// code that are automatically generated and assigned by robotbuilder.
// These sections will be updated in the future when you export to
// Java from RobotBuilder. Do not put any code or make any change in
// the blocks indicating autogenerated code or it will be lost on an
// update. Deleting the comments indicating the section will prevent
// it from being updated in the future.


package org.usfirst.frc2465.Robot;
    
import com.kauailabs.nav6.frc.IMUAdvanced;
import edu.wpi.first.wpilibj.*;
import edu.wpi.first.wpilibj.livewindow.LiveWindow;

/**
 * The RobotMap is a mapping from the ports sensors and actuators are wired into
 * to a variable name. This provides flexibility changing wiring, makes checking
 * the wiring easier and significantly reduces the number of magic numbers
 * floating around.
 */
public class RobotMap {
    // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS
    public static SerialPort serialPort;
    public static IMUAdvanced imu;
    public static CANJaguar driveLeftFrontSC;
    public static CANJaguar driveRightFrontSC;
    public static CANJaguar driveLeftRearSC;
    public static CANJaguar driveRightRearSC;
    public static RobotDrive robotDrive;
    // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=DECLARATIONS

    public static void init() {
        // BEGIN AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
        try { 
            driveLeftFrontSC = new CANJaguar(2);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
	
        
        try { 
            driveRightFrontSC = new CANJaguar(3);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
	
        
        try { 
            driveLeftRearSC = new CANJaguar(5);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
	
        
        try { 
            driveRightRearSC = new CANJaguar(4);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
	
        
        try {
	        robotDrive = new RobotDrive(driveLeftFrontSC, driveLeftRearSC,
	              driveRightFrontSC, driveRightRearSC);
	        robotDrive.setSafetyEnabled(true);
	        robotDrive.setExpiration(0.1);
	        robotDrive.setSensitivity(0.5);
	        robotDrive.setMaxOutput(1.0);

        } catch (Exception ex) {
            ex.printStackTrace();
        }
	
        // END AUTOGENERATED CODE, SOURCE=ROBOTBUILDER ID=CONSTRUCTORS
        try {
            serialPort = new SerialPort(57600,SerialPort.Port.kUSB);
            
            // You can add a second parameter to modify the 
            // update rate (in hz) from 4 to 100.  The default is 100.
            // If you need to minimize CPU load, you can set it to a
            // lower value, as shown here, depending upon your needs.
            
            // You can also use the IMUAdvanced class for advanced
            // features.

            byte update_rate_hz = 20;
            imu = new IMUAdvanced(serialPort,update_rate_hz);
        } catch (Exception ex) {
            ex.printStackTrace();
        }
        if ( imu != null ) {
            LiveWindow.addSensor("IMU", "Gyro", imu);
        }
    }
}
