#include "MiniIMU9AHRS.h"
#include "Arduino.h"

// Uncomment the below line to use this axis definition: 
   // X axis pointing forward
   // Y axis pointing to the right 
   // and Z axis pointing down.
// Positive pitch : nose up
// Positive roll : right wing down
// Positive yaw : clockwise
//int SENSOR_SIGN[9] = {1,1,1,-1,-1,-1,1,1,1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer
// NOTE:  This set is appropriate if the board is upside down 
// (components points down, silkscreen pointing up)

// Uncomment the below line to use this axis definition: 
   // X axis pointing forward
   // Y axis pointing to the left 
   // and Z axis pointing up.
// Positive pitch : nose down
// Positive roll : right wing down
// Positive yaw : counterclockwise
int SENSOR_SIGN[9] = {1,-1,-1,-1,1,1,1,-1,-1}; //Correct directions x,y,z - gyro, accelerometer, magnetometer
// NOTE:  This set is appropriate if the board is right-side up 
// (components points up, silkscreen pointing down)

const float c_IMUPeriodicRateSecs = 0.02;  // 50Hz

float G_Dt = c_IMUPeriodicRateSecs;    // Integration time (DCM algorithm)  We will run the integration loop at 50Hz if possible

long timer = 0;   //general purpose timer
long timer_old;
long timer24 = 0; //Second timer used to print values 
int AN[6]; //array that stores the gyro and accelerometer data (Order:  Gyro X,Y,Z Accel X,Y,Z
int AN_OFFSET[6]={0,0,0,0,0,0}; //Array that stores the Offset of the sensors (Same order as AN[])

int gyro_x;
int gyro_y;
int gyro_z;
int accel_x;
int accel_y;
int accel_z;
int magnetom_x;
int magnetom_y;
int magnetom_z;
float c_magnetom_x;
float c_magnetom_y;
float c_magnetom_z;
float MAG_Heading;

float Accel_Vector[3]= {0,0,0}; //Store the acceleration in a vector
float Gyro_Vector[3]= {0,0,0};//Store the gyros turn rate in a vector
float Omega_Vector[3]= {0,0,0}; //Corrected Gyro_Vector data
float Omega_P[3]= {0,0,0};//Omega Proportional correction
float Omega_I[3]= {0,0,0};//Omega Integrator
float Omega[3]= {0,0,0};

// Euler angles
float roll;
float pitch;
float yaw;

float lastRoll;
float lastPitch;
float lastYaw;

float errorRollPitch[3]= {0,0,0}; 
float errorYaw[3]= {0,0,0};

unsigned int counter=0;
byte gyro_sat=0;

float DCM_Matrix[3][3]= {
  {
    1,0,0  }
  ,{
    0,1,0  }
  ,{
    0,0,1  }
}; 
float Update_Matrix[3][3]={{0,1,2},{3,4,5},{6,7,8}}; //Gyros here


float Temporary_Matrix[3][3]={
  {
    0,0,0  }
  ,{
    0,0,0  }
  ,{
    0,0,0  }
};

extern void I2C_Init();

extern void Gyro_Init();
extern void Accel_Init();
extern void Compass_Init();

extern void Read_Gyro();
extern void Read_Accel();
extern void Read_Compass();

// Calculates X/Y-axis tilt-compensated compass heading, updates MAG_Heading.
extern void Compass_Heading();

extern void Matrix_update(void);
extern void Normalize(void);
extern void Drift_correction(void);
extern void Euler_angles(void);
extern void Calibrate_Compass( int numIterations );

void setup()
{ 
  I2C_Init();

  delay(1500);			// TODO:  Once it works, tune this to be shorter....
 
  Accel_Init();
  Compass_Init();
  Gyro_Init();
  
  delay(20);
  
  for(int i=0;i<32;i++)    // We take some readings...
    {
    Read_Gyro();
    Read_Accel();
    for(int y=0; y<6; y++)   // Cumulate values
      AN_OFFSET[y] += AN[y];
    delay(20);
    }
    
  for(int y=0; y<6; y++)
    AN_OFFSET[y] = AN_OFFSET[y]/32;
    
  AN_OFFSET[5]-=GRAVITY*SENSOR_SIGN[5];
  
  /*printf( "IMU Offsets:  G X: %4d, Y: %4d, Z: %4d,  A X: %4d, Y: %4d, Z: %4d\n",
		  AN_OFFSET[0],
		  AN_OFFSET[1],
		  AN_OFFSET[2],
		  AN_OFFSET[3],
		  AN_OFFSET[4],
		  AN_OFFSET[5] );*/
		  
  delay(500);				// TODO:  Once it works, tune this to be shorter; this delay seems unnecessary....
    
  timer=millis();
  delay(20);
  counter=0;

  lastPitch = 0;
  lastRoll = 0;
  lastYaw = 0;
  
#ifndef USE_MAGNETOMETER
  MAG_Heading = 0;  
#endif
}

// The goal here is to reset all variables to 'zero' out any drift
// which may have occurred.

#define NUM_ARRAY_ELEMENTS_1D(arr) ( sizeof((arr)) / sizeof((arr)[0]) )
#define INIT_ARRAY_1D( arr, val )  for ( size_t i=0; i < NUM_ARRAY_ELEMENTS_1D(arr); i++ ) { arr[i] = val; }

#define NUM_ARRAY_ELEMENTS_2D_X(arr) ( sizeof((arr)) / sizeof((arr)[0]) )
#define NUM_ARRAY_ELEMENTS_2D_Y(arr) ( sizeof((arr[0])) / sizeof((arr)[0][0]) )
#define INIT_ARRAY_2D( arr, val )  for ( size_t x=0; x < NUM_ARRAY_ELEMENTS_2D_X(arr); x++ ) { for ( size_t y=0; y < NUM_ARRAY_ELEMENTS_2D_Y(arr); y++ ) { arr[x][y] = val; } } 

void MiniIMU9AHRS::reset()
{
    CRITICAL_REGION(m_semaphore)
    {
	
	INIT_ARRAY_1D( errorRollPitch, 0 );
	INIT_ARRAY_1D( errorYaw, 0 );
	INIT_ARRAY_1D( AN, 0 );  // TODO:  Review if this is necessary
	
	INIT_ARRAY_1D( Accel_Vector, 0 );
	INIT_ARRAY_1D( Gyro_Vector, 0 );
	INIT_ARRAY_1D( Omega_Vector, 0 );
	INIT_ARRAY_1D( Omega_P, 0 );
	INIT_ARRAY_1D( Omega_I, 0 );
	INIT_ARRAY_1D( Omega, 0 );
	
	INIT_ARRAY_2D( Temporary_Matrix, 0 );
	
	DCM_Matrix[0][0] = 1;
	DCM_Matrix[0][1] = 0;
	DCM_Matrix[0][2] = 0;
	DCM_Matrix[1][0] = 0;
	DCM_Matrix[1][1] = 1;
	DCM_Matrix[1][2] = 0;
	DCM_Matrix[2][0] = 0;
	DCM_Matrix[2][1] = 0;
	DCM_Matrix[2][2] = 1;
	
	Update_Matrix[0][0] = 0;
	Update_Matrix[0][1] = 1;
	Update_Matrix[0][2] = 2;
	Update_Matrix[1][0] = 3;
	Update_Matrix[1][1] = 4;
	Update_Matrix[1][2] = 5;
	Update_Matrix[2][0] = 6;
	Update_Matrix[2][1] = 7;
	Update_Matrix[2][2] = 8;
	
	counter=0;
	gyro_sat=0;
	
	timer=millis();
	delay(20);
	counter=0;
	
	lastPitch = 0;
	lastRoll = 0;
	lastYaw = 0;
	  
	#ifndef USE_MAGNETOMETER
	  MAG_Heading = 0;  
	#endif	
    }
    END_REGION;	  
}

void MiniIMU9AHRS::loop() //Main Loop
{
	counter++;
	
	timer_old 	= timer;
	timer 		= millis();
	
	if ( timer > timer_old )
	{
		G_Dt = ( timer - timer_old ) / 1000.0;    // Real time of loop run. We use this on the DCM algorithm (gyro integration time)
	}
	else
	{
		G_Dt = 0;		
	}
	
	// *** DCM algorithm
	// Data adquisition
    CRITICAL_REGION(m_semaphore)
    {
		Read_Gyro();   			// Read gyro data
		Read_Accel();     		// Read I2C accelerometer
		
#ifdef USE_MAGNETOMETER		
		if (counter > 5)  		// Read compass data at 10Hz... (5 loop runs)
		{
			counter = 0;
			Read_Compass();    	// Read I2C magnetometer
			Compass_Heading(); 	// Calculate magnetic heading  

		    /*printf("A X: %4d Y: %4d Z: %4d  G X: %4d Y: %4d Z: %4d  M X: %4d Y: %4d Z: %4d\n",
		    		accel_x,
		    		accel_y,
		    		accel_z,
		    		gyro_x,
		    		gyro_y,
		    		gyro_z,
		    		magnetom_x,
		    		magnetom_y,
		    		magnetom_z);*/
		}
#endif		
    }
    END_REGION;
    
    //
	// Direction Cosine Matrix (DCM) Calculations...
	//
    
    Matrix_update(); 
	Normalize();
	Drift_correction();

    CRITICAL_REGION(m_semaphore)
	{
    	lastRoll = roll;
    	lastPitch = pitch;
    	lastYaw = yaw;
    	
    	Euler_angles();
    	
    	/*const float rad_to_deg = (180.0 / M_PI);
    	
    	float pitchDegrees = pitch * rad_to_deg;
    	float rollDegrees  = roll  * rad_to_deg;
    	float yawDegrees   = yaw   * rad_to_deg;
    	
	    printf("Pitch:  %4.2f Roll:  %4.2f Yaw:  %4.2f\n", pitchDegrees, rollDegrees, yawDegrees );
		*/
	}
    
    END_REGION;
}

void MiniIMU9AHRS::CallIntegrator( void *ahrs )
{
    ((MiniIMU9AHRS*)ahrs)->loop();
}

MiniIMU9AHRS::MiniIMU9AHRS()
{
    m_semaphore	= semBCreate(SEM_Q_PRIORITY, SEM_FULL);
    m_notifier 	= new Notifier(MiniIMU9AHRS::CallIntegrator, this);
    
    Calibrate();
    m_notifier->StartPeriodic(c_IMUPeriodicRateSecs);
}

void MiniIMU9AHRS::Calibrate()
{
	setup();
}

MiniIMU9AHRS::~MiniIMU9AHRS()
{
    semFlush(m_semaphore);
    if ( m_notifier )
    {
            delete m_notifier;
            m_notifier = 0;
    }	
}

const double rad_to_deg = (180.0 / M_PI);

void MiniIMU9AHRS::GetEulerAnglesDegrees( double& currPitch, double& currRoll, double& currYaw )
{
    CRITICAL_REGION(m_semaphore)
	{
		currRoll 	= roll;
		currPitch 	= pitch;
		currYaw 	= yaw;
	}
    END_REGION;	

    // Convert from Radians to Degrees
    
    currRoll  *= rad_to_deg;
    currPitch *= rad_to_deg;
    currYaw   *= rad_to_deg;
}

void MiniIMU9AHRS::GetDeltaEulerAnglesDegrees( double& deltaPitch, double& deltaRoll, double& deltaYaw )
{
	float currRoll, currPitch, currYaw, prevRoll, prevPitch, prevYaw;
    
	CRITICAL_REGION(m_semaphore)
	{
		currRoll 	= roll;
		currPitch 	= pitch;
		currYaw 	= yaw;
		
		prevRoll	= lastRoll;
		prevPitch	= lastPitch;
		prevYaw		= lastYaw;
	}
    END_REGION;		

    deltaPitch 	= currPitch - prevPitch;
    deltaRoll	= currRoll 	- prevRoll;
    deltaYaw	= currYaw	- prevYaw;

    // Convert from Radians to Degrees
    
    deltaPitch  *= rad_to_deg;
    deltaRoll 	*= rad_to_deg;
    deltaYaw   	*= rad_to_deg;

}

const float g_scalefactor = float(1) / .0039;	// 3.9mg per digit

void MiniIMU9AHRS::GetAccelGs( double& x, double& y, double& z )
{
    CRITICAL_REGION(m_semaphore)
	{
    	x = accel_x;
    	y = accel_y;
    	z = accel_z;
	}
    
    // scale to Gs
    x /= g_scalefactor;
    y /= g_scalefactor;
    z /= g_scalefactor;
    
    END_REGION;	
}

void MiniIMU9AHRS::GetAngularVelocityDegreesPerSec( double& x, double& y, double& z )
{
    CRITICAL_REGION(m_semaphore)
	{
		x = gyro_x;
		y = gyro_y;
		z = gyro_z;
	}
    END_REGION;	
}

void MiniIMU9AHRS::GetMagnetometerGauss( double& x, double& y, double& z )
{
    CRITICAL_REGION(m_semaphore)
	{
    	x = magnetom_x;
    	y = magnetom_y;
    	z = magnetom_z;
	}
    END_REGION;	
}

void MiniIMU9AHRS::CalibrateCompass( int numIterations )
{
	Calibrate_Compass( numIterations );
}

double MiniIMU9AHRS::GetCompassHeading()
{
    CRITICAL_REGION(m_semaphore)
	{
    	return MAG_Heading;
	}
    END_REGION;	
}
