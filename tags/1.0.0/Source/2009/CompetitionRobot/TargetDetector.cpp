/********************************************************************************
*  Project   		: FIRST Motor Controller
*  File Name  		: TwoColorDemo.cpp        
*  Contributors 	: ELF
*  Creation Date 	: Jan 3, 2009
*  Revision History	: Source code & revision history maintained at sourceforge.WPI.edu   
*  File Description	: Demo program showing color tracking using servos
*/                         
/*----------------------------------------------------------------------------*/
/*        Copyright (c) FIRST 2008.  All Rights Reserved.                     */
/*  Open Source Software - may be modified and shared by FRC teams. The code  */
/*  must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib. */
/*----------------------------------------------------------------------------*/

#include <iostream.h>
#include "math.h"

#include "AxisCamera.h" 
#include "BaeUtilities.h"
#include "FrcError.h"
#include "TrackAPI.h" 
#include "Target.h" 
#include "WPILib.h"
#include "TargetDetector.h"

#define REMOVED_CODE(x)

// To locally enable debug printing: set the debugFlag to a 1, to disable set to 0
static int TwoColorDemo_debugFlag = 0;
#define DPRINTF if(TwoColorDemo_debugFlag)dprintf

#define PI 3.14159265358979

// for 160x120, 50 pixels = .38 %
#define MIN_PARTICLE_TO_IMAGE_PERCENT 0.25		// target is too small
#define MAX_PARTICLE_TO_IMAGE_PERCENT 20.0		// target is too close

static const UINT32 kOversampleBits = 7;
static const UINT32 kAverageBits = 3;
static const float kSamplesPerSecond = 50.0;
static const float kCalibrationSampleTime = 5.0;
static const float kDefaultVoltsPerDegreePerSecond = 0.0125;



/** Simple test to see if the color is taking up too much of the image */
int tooClose(ParticleAnalysisReport* par) {
	if (par->particleToImagePercent > MAX_PARTICLE_TO_IMAGE_PERCENT)
		return 1;
	return 0;
}

/** Simple test to see if the color is large enough */
int bigEnough(ParticleAnalysisReport* par) {
	if (par->particleToImagePercent < MIN_PARTICLE_TO_IMAGE_PERCENT)
		return 0;
	return 1;
}

TargetDetector::TargetDetector(void)
	{
		ds = DriverStation::GetInstance();
		framesPerSecond = 15;					// number of camera frames to get per second
		cameraRotation = ROT_0;					// input parameter for camera orientation
		memset(&par,0,sizeof(par));				// initialize particle analysis report

		/* image data for tracking - override default parameters if needed */
		/* recommend making PINK the first color because GREEN is more 
		 * subsceptible to hue variations due to lighting type so may
		 * result in false positives */

		// PINK
		
		sprintf (td1.name, "PINK");
		td1.hue.minValue = 220;   
		td1.hue.maxValue = 255;  
		td1.saturation.minValue = 75;   
		td1.saturation.maxValue = 255;      
		td1.luminance.minValue = 85;  
		td1.luminance.maxValue = 255;
		
		// GREEN
		sprintf (td2.name, "GREEN");
		td2.hue.minValue = 55;   
		td2.hue.maxValue = 125;  
		td2.saturation.minValue = 58;   
		td2.saturation.maxValue = 255;    
		td2.luminance.minValue = 92;  
		td2.luminance.maxValue = 255;
		
		/* set up debug output: 
		 * DEBUG_OFF, DEBUG_MOSTLY_OFF, DEBUG_SCREEN_ONLY, DEBUG_FILE_ONLY, DEBUG_SCREEN_AND_FILE 
		 */
		SetDebugFlag(DEBUG_SCREEN_ONLY);
		
		/* start the CameraTask	 */
		if (StartCameraTask(framesPerSecond, 0, k320x240, ROT_180) == -1)
		{
			DPRINTF( LOG_ERROR,"Failed to spawn camera task; exiting. Error code %s", 
					GetVisionErrorText(GetLastVisionError()) );
		}
	
		Wait(1.0);
		
		StartImageAcquisition();
		
		/* allow writing to vxWorks target */
		Priv_SetWriteFileAllowed(1);   		

		savedImageTimestamp = 0.0;	
	}
	
	/**
	 * Adjusts the servo gimbal based on the color tracked.
	 * Driving the robot or operating an arm based on color input from gimbal-mounted 
	 * camera is currently left as an exercise for the teams.
	 */
	bool TargetDetector::Detect(double& centerOfMassX, double& centerOfMassY, double& particleToImagePercent)
	{
		DPRINTF(LOG_DEBUG, "SERVO - looking for COLOR %s ABOVE %s", td2.name, td1.name);
		
		// search variables 
		bool foundColor = false; 
				
		//GetWatchdog().Feed();		// turn watchdog off while debugging	

		// calculate gimbal position based on colors found 
		if ( FindTwoColors(td1, td2, ABOVE, &par) )
		{
			//PrintReport(&par);
			foundColor = true;
			savedImageTimestamp = par.imageTimestamp;	
			DPRINTF(LOG_DEBUG,"image timetamp: %lf", savedImageTimestamp);

			// report center, size of target 
				
			centerOfMassX = par.center_mass_x_normalized;
			centerOfMassY = par.center_mass_y_normalized;
			particleToImagePercent = par.particleToImagePercent;
		}  // end while
		return foundColor;
	}  // end autonomous


