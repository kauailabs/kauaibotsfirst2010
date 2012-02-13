/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

package team2465.smartdashboard.extension.trackerdisplay;

import edu.wpi.first.wpijavacv.WPIColorImage;
import edu.wpi.first.wpijavacv.WPIImage;
import edu.wpi.first.smartdashboard.camera.WPICameraExtension;
import edu.wpi.first.smartdashboard.robot.Robot;
import edu.wpi.first.wpijavacv.WPIColor;
import edu.wpi.first.wpijavacv.WPIPoint;
import java.io.File;
import java.io.IOException;
import javax.imageio.ImageIO;
import edu.wpi.first.wpilibj.networking.NetworkTable;
import java.util.NoSuchElementException;
/**
 *
 * @author slibert
 */
public class TrackerDisplay extends WPICameraExtension   {
    public static final String NAME = "KauaibotsCamera";
    public static final double CAMERA_HORIZONAL_FOV_DEGREES = 47;
    /*@Override
    public void init() {
    }*/

    @Override
    public WPIImage processImage(WPIColorImage rawImage)
    {
        NetworkTable cameraTable = NetworkTable.getTable("Camera");
        try
        {
            int iNumTargetsFound = cameraTable.getInt("TargetsFound");
            if ( iNumTargetsFound > 0 )
            {
                for ( int i = 0; i < iNumTargetsFound; i++ )
                {
                    String targetName = "Target" + i;
                    NetworkTable targetTable = NetworkTable.getTable(targetName);
                    if ( targetTable != null )
                    {
                        /*double dRectangularScore    = targetTable.getDouble("RectangularScore");
                        double dAspectRatioScore    = targetTable.getDouble("AspectRatioScore");
                        double dColEdgeScore        = targetTable.getDouble("ColEdgeScore");
                        double dRowEdgeScore        = targetTable.getDouble("RowEdgeScore");*/
                        int iLeftPixel              = targetTable.getInt(   "LeftPixel" );
                        int iTopPixel               = targetTable.getInt(   "TopPixel" );
                        int iWidthPixels            = targetTable.getInt(   "WidthPixels" );
                        int iHeightPixels           = targetTable.getInt(   "HeightPixels" );
                        /*double dDistanceInches      = targetTable.getDouble("DistanceInches");
                        double dElevationInches     = targetTable.getDouble("ElevationInches");
                        double dAngleDegrees        = targetTable.getDouble("AngleDegrees");*/

                        // Draw the target rectangle
                        rawImage.drawRect(iLeftPixel, iTopPixel, iWidthPixels, iHeightPixels, WPIColor.RED, 1);

                        int iCenterX = iLeftPixel + (iWidthPixels/2);
                        int iCenterY = iTopPixel + (iHeightPixels/2);

                        // Draw a dot representing the center of the target
                        rawImage.drawRect(  iCenterX-2, iCenterY-2, 5, 5, WPIColor.BLUE, 2);

                        // TODO:  Print the scores
                        // TODO:  Print the distance, elevation, angle
                    }
                }
            }
        }
        catch(NoSuchElementException ex)
        {

        }

        try
        {
            int iHoopFound = cameraTable.getInt("HoopFound");
            if ( iHoopFound > 0 )
            {
                NetworkTable hoopTable = NetworkTable.getTable("Hoop");
                if ( hoopTable != null )
                {
                    int iHoopCenterX = hoopTable.getInt("Center_X");
                    int iHoopCenterY = hoopTable.getInt("Center_Y");

                    // Draw a shape representing the center of the hoop
                    // The shape is a box w/cross-hairs
                    rawImage.drawRect(iHoopCenterX-4,iHoopCenterY-4,8,8,WPIColor.GREEN,1);
                    rawImage.drawLine(  new WPIPoint(iHoopCenterX-4,iHoopCenterY),
                                        new WPIPoint(iHoopCenterX+4, iHoopCenterY),
                                        WPIColor.GREEN, 1);
                    rawImage.drawLine(  new WPIPoint(iHoopCenterX,iHoopCenterY-4),
                                        new WPIPoint(iHoopCenterX, iHoopCenterY+4),
                                        WPIColor.GREEN, 1);
                }
            }
        }
        catch(NoSuchElementException ex)
        {

        }

        try
        {
            double dChuteAngle = Robot.getTable().getDouble("ChuteAngle");
            double dDegreesPerPixel = CAMERA_HORIZONAL_FOV_DEGREES / rawImage.getWidth();

            int iChuteCenterX = (rawImage.getWidth() / 2) + (int)(dChuteAngle / dDegreesPerPixel);
            int iChuteCenterY = rawImage.getHeight() - 10;

            rawImage.drawRect(iChuteCenterX-2,iChuteCenterY-2,5,5,WPIColor.RED,2);
            rawImage.drawLine(  new WPIPoint(iChuteCenterX-1,iChuteCenterY),
                                new WPIPoint(iChuteCenterX+1, iChuteCenterY),
                                WPIColor.RED, 1);
            rawImage.drawLine(  new WPIPoint(iChuteCenterX,iChuteCenterY-1),
                                new WPIPoint(iChuteCenterX, iChuteCenterY+1),
                                WPIColor.RED, 1);
        }
        catch(NoSuchElementException ex)
        {

        }

        try
        {
            File outputfile = new File("C:\\Users\\slibert.E1\\Documents\\WebcamPix\\Raw\\saved.png");
            ImageIO.write(rawImage.getBufferedImage(), "png", outputfile);
        }
        catch (IOException e)
        {
        }
        catch(Exception e)
        {

        }

        return rawImage;
    }
}
