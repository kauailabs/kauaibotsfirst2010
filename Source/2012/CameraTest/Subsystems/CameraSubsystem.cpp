#include "CameraSubsystem.h"
#include "../Robotmap.h"
#include "Vision/AxisCamera.h"
#include "nivision.h"
#include "../Commands/Camera.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "Target.h"
#include <algorithm>
#include <math.h>

static SEM_ID cDetectionMutex = semBCreate (SEM_Q_PRIORITY, SEM_FULL);

CameraSubsystem::CameraSubsystem() : 
	Subsystem("CameraSubsystem"),
	Thread("CameraProcessingThread"),
	m_bQuit(false),
	camera(AxisCamera::GetInstance()),
	cameraUp(CAMERA_UP_SOLENOID_CHANNEL),
	cameraDown(CAMERA_DOWN_SOLENOID_CHANNEL)
{
	// Configure Camera Settings
	
	camera.WriteResolution(AxisCamera::kResolution_320x240);
	camera.WriteCompression(30);
	camera.WriteBrightness(37);
	camera.WriteColorLevel(50);
	camera.WriteWhiteBalance(AxisCameraParams::kWhiteBalance_FixedIndoor);
	camera.WriteExposureControl(AxisCameraParams::kExposure_Automatic);
	camera.WriteColorLevel(50);
	// Contrast used w/test images (at Guerberville) was set to a Contrast of 50 (mid-bucket)

	// Force the creation of all network tables used by this subsystem.
	vector<Target> targets(4);
	UpdateNetworkTable(targets,4);
	UpdateNetworkTable(true,&m_lastHoop);
	
	SetDetectionMode( CameraSubsystem::kDetectNone );
	Thread::Start();
}
    
void CameraSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new Camera());
}

// This function executes on the camera subsystem's thread

void CameraSubsystem::ExecLoop ()
{
	SEM_ID dataAvailableSemaphore = camera.GetNewImageSem();
	while ( !m_bQuit )
	{		
		STATUS ret = semTake(dataAvailableSemaphore, WAIT_FOREVER);
		if ( ret == OK )
		{
			CameraSubsystem::DetectionMode detectionMode = GetDetectionMode();
			if ( detectionMode == CameraSubsystem::kDetectTargets )
			{
				GrabAndDetectTargets();
			}
			else if ( detectionMode == CameraSubsystem::kDetectHoop)
			{
				GrabAndDetectHoop();
			}
			else // No detection; clear everything to "nothing detected"
			{
				UpdateNetworkTable(false,&m_lastHoop);
				vector<Target> targets;
				UpdateNetworkTable(targets, 0 );				
			}
			semGive(dataAvailableSemaphore);
		}
	}
}

// Robot Metrics

const int cCameraHeightInches	= 68;				// Measured in the fully-extended position
const int cCameraDistanceFromRobotFrontBumper = 35; // TODO:  Check this

// Derived Measurements

const double cTargetAspectRatio				= double(cTargetWidthInches)/double(cTargetHeightInches);
const double cTapePercentageOfTargetWidth	= double(cTapeWidthInches) / double(cTargetWidthInches);
const double cTapePercentageOfTargetHeight	= double(cTapeWidthInches) / double(cTargetHeightInches);

// The following metrics describe the percentage the tape represents of the entire target
// Given a detected target rectangle, these percentages indicates which portions should be
// visible "tape"; the remainder should be not visible.

const double cLeftVerticalTapePixelPercentage		= cTapePercentageOfTargetWidth / 2;
const double cRightVerticalTapePixelPercentage		= double(1) - (cTapePercentageOfTargetWidth / 2);
const double cTopHorizontalTapePixelPercentage		= cTapePercentageOfTargetHeight / 2;
const double cBottomHorizontalTapePixelPercentage	= double(1) - cTapePercentageOfTargetHeight / 2;

const double cHorizontalTapeRowPerfectScore = 1.0;
const double cVerticalTapeRowPerfectScore 	= 1.0 / cTapePercentageOfTargetWidth; 
const double cVerticalTapeColPerfectScore 	= 1.0;
const double cHorizontalTapeColPerfectScore = 1.0 / cTapePercentageOfTargetHeight;

void CameraSubsystem::UpdateNetworkTable( bool bFoundHoop, Hoop *pHoop )
{
    NetworkTable *pDetectedTargetsTable = NetworkTable::GetTable("Camera");
    if(pDetectedTargetsTable)
    {
       //pDetectedTargetsTable->BeginTransaction();

        pDetectedTargetsTable->PutInt( "HoopFound", (bFoundHoop ? 1 : 0) );
        if ( bFoundHoop && pHoop )
        {	
			NetworkTable *pHoopTable = NetworkTable::GetTable("Hoop");
			if ( pHoopTable )
			{
				pHoopTable->PutInt( "Center_X", pHoop->m_CenterOfMassX );
				pHoopTable->PutInt( "Center_Y", pHoop->m_CenterOfMassY );
			}
        }
        
       //pDetectedTargetsTable->EndTransaction();
    }
}

void CameraSubsystem::UpdateNetworkTable(vector<Target>& targets, int iNumDetectedTargets )
{
    NetworkTable *pCameraTable = NetworkTable::GetTable("Camera");
    if(pCameraTable)
    {
        //pCameraTable->BeginTransaction();
        pCameraTable->PutInt( "TargetsFound", iNumDetectedTargets );
        for(int i = 0;i < iNumDetectedTargets; i++){
            // Calculate height of the targe (based on the height of the center vertical pixel, and known 
            // camera height and camera horizontal pointing angle
            // Calculate angle to each of the targets, based upon the degrees/pixel * the horizontal
            // pixel position of the center
            // Calculate distance to the target
            // Note that this distance is correct if the robot is center of the target is in the
            // center of the field of view and the left and right sides are of the same length
            // Once calculated, add them to the network table
            char targetTableName[256];
            sprintf(targetTableName, "Target%d", i);
            NetworkTable *pTargetTable = pCameraTable->GetTable(targetTableName);
            if(pTargetTable)
            {
                pTargetTable->PutDouble( "RectangularScore",	targets[i].m_RectangularScore);
                pTargetTable->PutDouble( "ApectRatioScore",		targets[i].m_AspectRatioScore);
                pTargetTable->PutDouble( "ColEdgeScore", 		targets[i].m_ColEdgeScore);
                pTargetTable->PutDouble( "RowEdgeScore", 		targets[i].m_RowEdgeScore);
                pTargetTable->PutInt( 	 "LeftPixel", 			targets[i].m_Left);
                pTargetTable->PutInt(	 "TopPixel", 			targets[i].m_Top);
                pTargetTable->PutInt(	 "WidthPixels", 		targets[i].m_Width);
                pTargetTable->PutInt(	 "HeightPixels", 		targets[i].m_Height);
                pTargetTable->PutDouble( "DistanceInches", 		targets[i].m_DistanceInches);
                pTargetTable->PutDouble( "ElevationInches", 	targets[i].m_ElevationFromFloorInches);
                pTargetTable->PutDouble( "AngleDegrees", 		targets[i].m_AngleFromRobotCenterDegrees);
            }
        }

        //pCameraTable->EndTransaction();
    }

}

// Invoked when a new video frame is available from the camera
void CameraSubsystem::GrabAndDetectHoop()
{
	ColorImage *image = camera.GetImage();
	if ( image )
	{
		ParticleFilterCriteria2 criteria[] = {
											{IMAQ_MT_BOUNDING_RECT_WIDTH, 30, 400, false, false},
											{IMAQ_MT_BOUNDING_RECT_HEIGHT, 40, 400, false, false}
		};	
		
		// TODO: Tune RGB Thresholds based on video samples acquired on our practice field.
		Threshold threshold(0, 34, 0, 0, 0, 0);
		
		BinaryImage *thresholdImage		= image->ThresholdRGB(threshold);	// get just the red pixels
		BinaryImage *bigObjectsImage	= thresholdImage->RemoveSmallObjects(false, 2);  // remove small objects (noise)
		BinaryImage *convexHullImage	= bigObjectsImage->ConvexHull(false);  // fill in partial and full rectangles
		BinaryImage *filteredImage		= convexHullImage->ParticleFilter(criteria, 2);  // find the rectangles

		PointFloat centroid;
		Hoop theHoop;
		int result = imaqCentroid(filteredImage->GetImaqImage(), &centroid, NULL); 
		if ( result != 0 )
		{
			theHoop.m_bDetected = true;
			theHoop.m_CenterOfMassX = int(centroid.x);
			theHoop.m_CenterOfMassY = int(centroid.y);
		}
		else
		{
			theHoop.m_bDetected = false;
			theHoop.m_CenterOfMassX = 0;
			theHoop.m_CenterOfMassY = 0;
		}
		// Now, update the thread-safe copy of the latest hoop detection data
		{
			Synchronized sync(cDetectionMutex);
			m_lastHoop = theHoop;
		}
		
	    UpdateNetworkTable( theHoop.m_bDetected, &theHoop );
		
		delete filteredImage;
		delete convexHullImage;
		delete bigObjectsImage;
		delete thresholdImage;
		delete image;
	}
}

// Invoked when a new video frame is available from the camera
void CameraSubsystem::GrabAndDetectTargets()
{
	ColorImage *image = camera.GetImage();
	
	ParticleFilterCriteria2 criteria[] = {
										{IMAQ_MT_BOUNDING_RECT_WIDTH, 12, 400, false, false},
										{IMAQ_MT_BOUNDING_RECT_HEIGHT, 40, 400, false, false}
	};	
	
	// TODO: Tune HSL Thresholds based on video samples acquired on our practice field.
	Threshold threshold(78, 184, 102, 255, 47, 239);
	
	BinaryImage *thresholdImage		= image->ThresholdHSL(threshold);	// get just the luminant pixels
	BinaryImage *bigObjectsImage	= thresholdImage->RemoveSmallObjects(false, 2);  // remove small objects (noise)
	BinaryImage *convexHullImage	= bigObjectsImage->ConvexHull(false);  // fill in partial and full rectangles
	BinaryImage *filteredImage		= convexHullImage->ParticleFilter(criteria, 2);  // find the rectangles
	vector<ParticleAnalysisReport> *reports = filteredImage->GetOrderedParticleAnalysisReports();  // get the results
	vector<Target> targets(reports->size());
	
	// Now, score the particles
	for (unsigned i = 0; i < reports->size(); i++) 
	{
		ParticleAnalysisReport *r = &(reports->at(i));

		/*printf("particle: %d  center_mass_x: %d\n", i, r->center_mass_x);	*/
		
		// Rectangular Score - how close is this to being a rectangular shape?
		
		double boundingBoxArea = r->boundingRect.height * r->boundingRect.width;
		double rectangularScore = (r->particleArea/boundingBoxArea) * 100;

		/*printf("particle:  %d  rectangular score:  %f\n", i, rectangularScore );	*/
		// Aspect Ratio score - how close is the measured aspect ratio to the targets?
		
		double aspectRatio = double(r->boundingRect.width) / double(r->boundingRect.height);
		double aspectRatioScore = (aspectRatio / cTargetAspectRatio) * 100;
		/*printf("particle:  %d  aspect ratio score:  %f\n", i, aspectRatioScore );	*/

		double rowScoreTotal = 0;
		double rowScoreAvg = 0;	
		double colScoreTotal = 0;
		double colScoreAvg = 0;
		
		// Edge score - is the particle appropriately hollow?
		LinearAverages* linearAverages = imaqLinearAverages2(
				bigObjectsImage->GetImaqImage(),
				LinearAveragesMode(IMAQ_COLUMN_AVERAGES | IMAQ_ROW_AVERAGES),
				r->boundingRect );
		if ( linearAverages )
		{
			int iNumCols = linearAverages->columnCount;
			int iNumRows = linearAverages->rowCount;
			// calc average of row avg score and col avg score
			
			for ( int iRow = 0; iRow < iNumRows; iRow++ )
			{
				double fRowPercent = double(iRow) / double(iNumRows);
				bool bHorizontalTapeRow =	(fRowPercent <= cTopHorizontalTapePixelPercentage) ||
											(fRowPercent >= cBottomHorizontalTapePixelPercentage);
				double rowScore = linearAverages->rowAverages[iRow] /
									bHorizontalTapeRow ? 
											cHorizontalTapeRowPerfectScore : cVerticalTapeRowPerfectScore;
				rowScoreTotal += rowScore;						
			}
			
			for ( int iCol = 0; iCol < iNumCols; iCol++ )
			{
				double fColPercent = double(iCol) / double(iNumCols);
				bool bVerticalTapeRow =	(fColPercent <= cLeftVerticalTapePixelPercentage) ||
										(fColPercent >= cRightVerticalTapePixelPercentage);
				double colScore = linearAverages->columnAverages[iCol] /
									bVerticalTapeRow ? 
											cVerticalTapeColPerfectScore : cHorizontalTapeColPerfectScore;
				colScoreTotal += colScore;						
			}
			
			rowScoreAvg = rowScoreTotal / iNumRows;
			colScoreAvg = colScoreTotal / iNumCols;
			
			/*printf("particle:  %d  row edge score:  %f\n", i, rowScoreAvg );	
			printf("particle:  %d  col edge score:  %f\n", i, colScoreAvg );*/	
			
			imaqDispose(linearAverages);
		}
		
		targets[i].m_RectangularScore	= rectangularScore;
		targets[i].m_AspectRatioScore	= aspectRatioScore;
		targets[i].m_ColEdgeScore 		= colScoreAvg;
		targets[i].m_RowEdgeScore		= rowScoreAvg;
		targets[i].m_Left 				= r->boundingRect.left;
		targets[i].m_Top 				= r->boundingRect.top;
		targets[i].m_Width 				= r->boundingRect.width;
		targets[i].m_Height 			= r->boundingRect.height;		
	}
	
	// Find the highest-scoring targets (from 0 to 4 of them)
	std::sort(targets.begin(), targets.end());
	
	// TODO:  Filter the objects based on score.  Update the m_bDetected
	// member variable if they pass.  For now, let them all through
	
	int iNumBestTargets = targets.size();
	if ( iNumBestTargets > cNumTargets )
	{
		iNumBestTargets = cNumTargets;
	}

	// Update the thread-safe detection objects
	{
		Synchronized sync(cDetectionMutex);         	
		for(int iTarget = 0;iTarget < iNumBestTargets; iTarget++)
		{
			targets[iTarget].m_bDetected = true;	// TODO: Update based on passing score
			
			CalculateTargetAngle( targets[iTarget] );
			CalculateTargetHeight( targets[iTarget] );
			CalculateDistance( targets[iTarget] );
			m_lastTarget[iTarget] = targets[iTarget];
		}
		
		// Mark any undetected targets
		
		for ( int iUndetectedTarget = iNumBestTargets; iUndetectedTarget < cNumTargets; iUndetectedTarget++ )
		{
			m_lastTarget[iUndetectedTarget].m_bDetected = false;
		}
	}
    
    UpdateNetworkTable(targets, iNumBestTargets );
    
    // be sure to delete images after using them
	delete reports;
	delete filteredImage;
	delete convexHullImage;
	delete bigObjectsImage;
	delete thresholdImage;
	delete image;
}

// Put methods for controlling this subsystem
// here. Call these from Commands.

void CameraSubsystem::WriteCurrentImage()
{
	char filenameBuffer[512];
	for ( int i = 0; i < 2048; i++ )
	{
		sprintf(filenameBuffer,"image_%d.jpg",i);
		struct stat st;
		if(stat(filenameBuffer,&st) != 0)
		{
			// The file doesn't already exist.
			HSLImage *image = camera.GetImage();
			image->Write( filenameBuffer );	
			delete image;
			break;
		}		
	}
}

void CameraSubsystem::SetDetectionMode( DetectionMode newMode )
{
	if ( newMode != GetDetectionMode() )
	{
		Synchronized sync(cDetectionMutex);         
		m_detectionMode = newMode;
	}
}

CameraSubsystem::DetectionMode CameraSubsystem::GetDetectionMode()
{
	Synchronized sync(cDetectionMutex);         	
	return m_detectionMode;
}

bool CameraSubsystem::GetLastDetectedTargets( Target& t1, Target& t2, Target& t3, Target& t4 )
{
	if ( GetDetectionMode() != CameraSubsystem::kDetectTargets ) return false;
	Synchronized sync(cDetectionMutex);         	
	t1 = m_lastTarget[0];
	t2 = m_lastTarget[1];
	t3 = m_lastTarget[2];
	t4 = m_lastTarget[3];
	return ( 	m_lastTarget[0].m_bDetected || 
				m_lastTarget[1].m_bDetected || 
				m_lastTarget[2].m_bDetected || 
				m_lastTarget[3].m_bDetected );
}

bool CameraSubsystem::GetLastDetectedHoop( Hoop& h)
{
	if ( GetDetectionMode() != CameraSubsystem::kDetectHoop ) return false;	
	Synchronized sync(cDetectionMutex);         	
	h = m_lastHoop;
	return m_lastHoop.m_bDetected;
}

void CameraSubsystem::UpPeriscope()
{
	cameraUp.Set(true);
	cameraDown.Set(false);
}

void CameraSubsystem::DownPeriscope()
{
	cameraDown.Set(true);
	cameraUp.Set(false);
}

bool CameraSubsystem::IsPeriscopeUp()
{
	return cameraUp.Get();
}

void CameraSubsystem::CalculateTargetHeight(Target& t)
{
	if ((cCameraImagePixelHeight/2 >= t.m_Top) && (cCameraImagePixelHeight/2 <= (t.m_Top + t.m_Height)))
	{
		t.m_ElevationFromFloorInches= cMiddleTargetBottomHeightInches;
	}
	else if ((cCameraImagePixelHeight/2) >= (t.m_Top + t.m_Height))
	{
		t.m_ElevationFromFloorInches= cTopTargetBottomHeightInches;
	}
	else 
	{
		t.m_ElevationFromFloorInches= cLowTargetBottomHeightInches;
	}
}  

void CameraSubsystem::CalculateTargetAngle(Target& t)
{
	double offsetFromCenter = (t.m_Left + (t.m_Width/2)) - cCameraImagePixelWidth/2;	
	double offsetFromBottom = cCameraImagePixelHeight - (t.m_Top + (t.m_Height/2)) ;
	double targetAngle = atan(fabs(offsetFromBottom)/fabs(offsetFromCenter));

	if (offsetFromCenter < 0)
	{
		targetAngle = targetAngle * -1;
	}

	t.m_AngleFromRobotCenterDegrees = targetAngle; 
}

#define PI 3.14159265

void CameraSubsystem::CalculateDistance( Target& t )
{
 t.m_DistanceInches = (cCameraImagePixelHeight/t.m_Height)* (cTargetHeightInches/2)/tan((cCameraVerticalFieldOfViewDegrees/2)*(PI/180));
}
