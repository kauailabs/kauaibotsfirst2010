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
  
  printf( "IMU Offsets:  G X: %4d, Y: %4d, Z: %4d,  A X: %4d, Y: %4d, Z: %4d\n",
		  AN_OFFSET[0],
		  AN_OFFSET[1],
		  AN_OFFSET[2],
		  AN_OFFSET[3],
		  AN_OFFSET[4],
		  AN_OFFSET[5] );
		  
  delay(2000);				// TODO:  Once it works, tune this to be shorter; this delay seems unnecessary....
    
  timer=millis();
  delay(20);
  counter=0;
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
		
		if (counter > 5)  		// Read compass data at 10Hz... (5 loop runs)
		{
			counter = 0;
			Read_Compass();    	// Read I2C magnetometer
			Compass_Heading(); 	// Calculate magnetic heading  

		    printf("A X: %4d Y: %4d Z: %4d  G X: %4d Y: %4d Z: %4d  M X: %4d Y: %4d Z: %4d\n",
		    		accel_x,
		    		accel_y,
		    		accel_z,
		    		gyro_x,
		    		gyro_y,
		    		gyro_z,
		    		magnetom_x,
		    		magnetom_y,
		    		magnetom_z);
		}
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
    	Euler_angles();
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

void MiniIMU9AHRS::GetEulerAnglesDegrees( double& currPitch, double& currRoll, double& currYaw )
{
    CRITICAL_REGION(m_semaphore)
	{
		currRoll 	= roll;
		currPitch 	= pitch;
		currYaw 	= yaw;
	}
    END_REGION;	
}

void MiniIMU9AHRS::GetAccelGs( double& x, double& y, double& z )
{
    CRITICAL_REGION(m_semaphore)
	{
    	x = accel_x;
    	y = accel_y;
    	z = accel_z;
	}
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
	return MAG_Heading;
}
