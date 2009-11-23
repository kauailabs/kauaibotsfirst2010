/********************************************************************************
*  Project   		: FIRST Motor Controller
*  File Name  		: TargetDetector.h        
*  Contributors 	: SAL
*  Creation Date 	: Feb 7, 2009
*  Revision History	: Source code & revision history maintained at sourceforge.WPI.edu   
*  File Description	: Demo program showing color tracking using servos
*/                         
/*----------------------------------------------------------------------------*/
/*        Copyright (c) FIRST 2008.  All Rights Reserved.                     */
/*  Open Source Software - may be modified and shared by FRC teams. The code  */
/*  must be accompanied by the FIRST BSD license file in $(WIND_BASE)/WPILib. */
/*----------------------------------------------------------------------------*/

#include "TrackAPI.h" 
#include "WPILib.h"

/**
 * This is a demo program showing the use of the color tracking API to track two colors.
 * It uses the SimpleRobot class as a base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls. Autonomous mode tracks color assuming camera is 
 * mounted on a gimbal with two servos.
 */
class TargetDetector
{
	DriverStation *ds;				// driver station object
	int framesPerSecond;			// number of camera frames to get per second
	int cameraRotation;				// angle that camera is mounted
	TrackingThreshold td1, td2;		// color thresholds
	double savedImageTimestamp;
	
	ParticleAnalysisReport par;		// particle analysis report
		
	enum							// Driver Station jumpers to control program operation
	{ ARCADE_MODE = 1,				// Tank/Arcade jumper is on DS Input 1 (Jumper present is arcade)
	  ENABLE_AUTONOMOUS = 2,		// Autonomous/Teleop jumper is on DS Input 2 (Jumper present is autonomous)
	} jumpers;	                            

public:
	/**
	 * Constructor for a target detector.
	 */
	TargetDetector(void);
	
	/**
	 * Detects a target, returning information about center of mass and particle size.
	 */
	bool Detect(double& centerOfMassX, double& centerOfMassY, double& particleToImagePercent);
}; 

