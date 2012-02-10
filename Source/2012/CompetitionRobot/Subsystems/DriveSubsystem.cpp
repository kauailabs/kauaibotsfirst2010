#include "DriveSubsystem.h"
#include "../Robotmap.h"
#include "../Commands/Drive.h"

static SEM_ID cAutoRotateSemaphore = semBCreate (SEM_Q_PRIORITY, SEM_FULL);

#define DEGREES_IN_A_CIRCLE	360

DriveSubsystem::DriveSubsystem() : 
	PIDSubsystem(	"DriveSubsystem",
					Preferences::GetInstance()->GetDouble("DriveAutoRotate_P",0.024),
					Preferences::GetInstance()->GetDouble("DriveAutoRotate_I",0.0012),
					Preferences::GetInstance()->GetDouble("DriveAutoRotate_D",0.00) ),
	drive(LEFT_FRONT_CAN_ADDRESS,RIGHT_FRONT_CAN_ADDRESS,LEFT_REAR_CAN_ADDRESS,RIGHT_REAR_CAN_ADDRESS,CANJaguar::kSpeed),
	yaw(DRIVE_YAW_GYRO_CHANNEL),
	pitch(DRIVE_PITCH_GRYO_CHANNEL),
	roll(DRIVE_ROLL_GYRO_CHANNEL),
	frontRanger(FRONT_RANGEFINDER_CHANNEL),
	rightRanger(RIGHT_RANGEFINDER_CHANNEL),
	rearRanger(REAR_RANGEFINDER_CHANNEL),
	leftRanger(LEFT_RANGEFINDER_CHANNEL),
	frontLeftEdgeFinder(DRIVE_FRONT_LEFT_EDGEFINDER_CHANNEL),
	frontRightEdgeFinder(DRIVE_FRONT_RIGHT_EDGEFINDER_CHANNEL),
	rearLeftEdgeFinder(DRIVE_REAR_LEFT_EDGEFINDER_CHANNEL),
	rearRightEdgeFinder(DRIVE_REAR_RIGHT_EDGEFINDER_CHANNEL)
{	
	InitializeSensors();
	
	// Initialize the Auto-rotation feature
	
    m_bAutoRotationMode = false;
    m_pendingAutoRotateAmount = 0.0;
    
    GetPIDController()->SetInputRange(0, DEGREES_IN_A_CIRCLE);	// NOTE:  Used to be -360 to 360
    GetPIDController()->SetContinuous(true);					// TODO:  Review this
    
    double dPIDOutputRange = Preferences::GetInstance()->GetDouble("DriveMaxAutoRotateSpeed", .85);
    
    GetPIDController()->SetOutputRange((dPIDOutputRange*-1), dPIDOutputRange);    // TODO:  Depends upon speed v. voltage mode
    // TODO:  Review this  
    // Tolerance (15.0 = 15%) of the allowable error
    // in the input range.
    //
    // So given a 360 degree input range, a one degree of tolerance
    // would be 1/(360/100)in the PIDController's calculation of OnTarget()
    double dToleranceInDegrees = Preferences::GetInstance()->GetDouble("DriveAutoRotateToleranceInDegrees", 1.0);
    double dToleranceInPercent = dToleranceInDegrees / (double(DEGREES_IN_A_CIRCLE) / double(100));
    GetPIDController()->SetTolerance( dToleranceInPercent );     
}
    
void DriveSubsystem::InitializeSensors()
{
    // Gyroscopes
	//         
	// Gyro is initialized, including calibration, in constructor
	//         
	// Initialize sensivity of the AD22305 Gyroscope         
	yaw.SetSensitivity(.007);  	
	pitch.SetSensitivity(.007);  	
	roll.SetSensitivity(.007);  	
}

void DriveSubsystem::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	SetDefaultCommand(new Drive());
}

//
// NOTE:  If in percentVBus mode, x,y,rot are values from -1 to 1
//        If in speed mode, x,y,rot are values from -RPM_MAX to +RPM_MAX

void DriveSubsystem::DoMecanum( float vX, float vY, float vRot )
{
    if ( m_bAutoRotationMode )         
    {                 
    	vRot = ThreadSafeGetAutoRotateMotorOutputValue();                 
    	vRot *= -1;     // Invert value before sending to the output.                                  
    	//printf("AutoRotate:  %f [Error:  %f] %s\n",(double)vRot, (double)GetPIDController()->GetError(), GetPIDController()->OnTarget() ? "On Target" : "");
    }
	
	drive.DoMecanum( vX, vY, vRot );
}
CANJaguar::ControlMode DriveSubsystem::getMode()
{
	return drive.GetMode();
}

void DriveSubsystem::setMode( CANJaguar::ControlMode newMode )
{
	drive.SetMode( newMode );
}

void DriveSubsystem::GetEulerAngles( double& yawAngle, double& pitchAngle, double& rollAngle)
{
	yawAngle	= ClipGyroAngle(yaw.GetAngle());
	pitchAngle	= ClipGyroAngle(pitch.GetAngle());
	rollAngle	= ClipGyroAngle(roll.GetAngle());
}

void DriveSubsystem::GetRangesInches( double& frontRange, double& rightRange, double& rearRange, double& leftRange )
{
	frontRange	= frontRanger.GetRangeInches();
	rightRange	= rightRanger.GetRangeInches();
	rearRange 	= rearRanger.GetRangeInches();
	leftRange 	= leftRanger.GetRangeInches();
}

void DriveSubsystem::GetEdges( bool &frontLeft, bool& frontRight, bool& rearRight, bool& rearLeft)
{
	frontLeft	= (frontLeftEdgeFinder.Get() != 0);
	frontRight	= (frontRightEdgeFinder.Get() != 0);
	rearLeft	= (rearLeftEdgeFinder.Get() != 0);
	rearRight	= (rearRightEdgeFinder.Get() != 0);
}

double DriveSubsystem::ClipGyroAngle( double dInputAngle )
{
	// The angle from the Gyro is continuous, that is can go beyond 
	// 360 degrees. The DriveSubystem wants a continuous angle-space that
	// wraps at 0 degrees.
	
	double dOutputAngle = 0.0;
	
    if ( dInputAngle >= double(DEGREES_IN_A_CIRCLE) )         
    {                 
    	int divisor = (int)(dInputAngle / (double)DEGREES_IN_A_CIRCLE);
    	dOutputAngle = dInputAngle - (divisor * DEGREES_IN_A_CIRCLE);         
    }         
    else if ( dInputAngle <= double(-DEGREES_IN_A_CIRCLE) )         
    {                 
    	int divisor = (int)(dInputAngle / (double)-DEGREES_IN_A_CIRCLE);                 
    	dOutputAngle = dInputAngle + (divisor * DEGREES_IN_A_CIRCLE);                     
    }  	
    return dOutputAngle;
}

double DriveSubsystem::ReturnPIDInput()
{
	return ClipGyroAngle( yaw.GetAngle() );
}

// NOTE:  This method is invoked by the PID controller, and occurs on a 
// timer interrupt - thus any variables it accesses needs to be 
// made thread-safe.

void DriveSubsystem::UsePIDOutput( double dOutputValue )
{
	ThreadSafeSetAutoMotorOutputValue( dOutputValue );
}

float DriveSubsystem::ThreadSafeGetAutoRotateMotorOutputValue() 
{         
	Synchronized sync(cAutoRotateSemaphore);         
	float autoRotateAmount = m_pendingAutoRotateAmount;         
	return autoRotateAmount; 
}  

void DriveSubsystem::ThreadSafeSetAutoMotorOutputValue( float rotOutput ) 
{         
	Synchronized sync(cAutoRotateSemaphore);         
	m_pendingAutoRotateAmount = rotOutput; 
}  

void DriveSubsystem::SetAutoRotationMode( bool bEnable, double dTargetAngle ) 
{         
	if ( bEnable != m_bAutoRotationMode )         
	{                 
		m_bAutoRotationMode = bEnable;                 
		ThreadSafeSetAutoMotorOutputValue(0);     // Always reset rotation amount                  
		
		if ( bEnable )
		{
			GetPIDController()->SetSetpoint(dTargetAngle);
			GetPIDController()->Enable();
		}
		else
		{
			// TODO:  Review this:  is it preferable to invoke Reset() or Disable() on the 
			// PID Controller?
			GetPIDController()->Reset();
		}
	} 
}  

bool DriveSubsystem::GetAutoRotationMode() 
{         
	return m_bAutoRotationMode; 
}

bool DriveSubsystem::GetAutoRotationTargetReadings( bool& bOnTarget, double& dSetPoint, double& dAngularError )
{
	if ( !m_bAutoRotationMode ) return false;
	
	bOnTarget = GetPIDController()->OnTarget();
	dSetPoint = GetPIDController()->GetSetpoint();
	dAngularError = GetPIDController()->GetError();
	return true;
}

