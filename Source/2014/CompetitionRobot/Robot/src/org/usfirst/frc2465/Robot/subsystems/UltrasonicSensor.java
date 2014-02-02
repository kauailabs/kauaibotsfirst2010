/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package org.usfirst.frc2465.Robot.subsystems;

import edu.wpi.first.wpilibj.AnalogChannel;
import edu.wpi.first.wpilibj.PIDSource;
import edu.wpi.first.wpilibj.SensorBase;
import edu.wpi.first.wpilibj.Timer;
import edu.wpi.first.wpilibj.livewindow.LiveWindowSendable;
import edu.wpi.first.wpilibj.tables.ITable;
import com.sun.squawk.util.MathUtils;

/**
 *
 * @author Scott
 */

public class UltrasonicSensor extends SensorBase implements PIDSource, LiveWindowSendable {

    int sensor_range;
    AnalogChannel   analog_channel;
    ITable table;
    
    double volts_to_cm;
    double cm_to_inches;
    
    public UltrasonicSensor(final int slot, final int channel) {
        analog_channel = new AnalogChannel(slot, channel);
        initUltrasonicSensor();
        
        // See getDistanceInches() for location of datasheet,
        // from which the following scale factors were calculated.
        
        volts_to_cm  = 5.0/1024.0;
        cm_to_inches = 1.0/2.54;
    }
    
    /**
     * Initialize the UltrasonicSensor.
     */
    protected void initUltrasonicSensor()
    {
        // Be careful not to set the average bits more than once.
        int num_average_bits = 4;
        if ( analog_channel.getAverageBits() != num_average_bits )
        {
            analog_channel.setAverageBits(num_average_bits); // 2^4 = 16-sample average
            float sampleRate = (float) (50.0 * 
                    (1 << (num_average_bits)));
            analog_channel.getModule().setSampleRate(sampleRate);
            Timer.delay(0.2);
        }
    }

    /**
     * Return the actual distance in millimeters.
     * 
     * @return the current distance in millimeters.
     */

    public double getDistanceInches()
    {
        // Transform voltage to distance.  Scale Factor Values
        // were derived from the datasheet
        //
        // http://maxbotix.com/documents/XL-MaxSonar-EZ_Datasheet.pdf

        double voltage = analog_channel.getAverageVoltage();
        double inches = (voltage * volts_to_cm) * cm_to_inches;
        return inches;
    }
    
    
    public double pidGet() {
        return getDistanceInches();
    }

    public void updateTable() {
        if (table != null) {
            table.putNumber("Value", getDistanceInches());
        }
    }

    public void startLiveWindowMode() {
    }

    public void stopLiveWindowMode() {
    }

    public void initTable(ITable itable) {
        table = itable;
        updateTable();
    }

    public ITable getTable() {
        return table;
    }

    public String getSmartDashboardType() {
        return "Ultrasonic";
    }
    
}

