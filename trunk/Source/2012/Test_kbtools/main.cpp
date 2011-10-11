/*
 * main.cpp
 *
 *  Created on: Oct 10, 2011
 *      Author: slibert
 */

#include "Logger.h"
#include "Registry.h"
#include "Thread.h"
#include "MutexLock.h"

// Registry
char *		c_pRegistryFile 		= "./registry.ini";

// RegistryKeys
#define RegKey_TestInt 		"TestInt"
#define RegKey_TestString 	"TestString"
#define RegKey_TestDouble	"TestDouble"

// Logger Defaults
char *		c_pLoggerDirectory 		= "./logs/";
const int 	c_DaysToKeep 			= 3;
const int   c_LogLevel				= LOGGER_DEFAULT_LEVEL;
int			c_LogBufferSize 		= 10000;
int			c_LogFlushFrequency_ms 	= 1000;

class TestThread : public Thread
{
protected:
	Event m_Event;
	int	  m_State;
public:
	TestThread() : Thread("TestThread"), m_State(0) {}
	void ExecLoop();
	void Signal() { m_Event.Signal(); }
	void SetState( int newState )
	{
		MutexLock(GetMutex());
		m_State = newState;
	}
	int GetState()
	{
		MutexLock(GetMutex());
		return m_State;
	}
};

void TestThread::ExecLoop()
{
	LOG(__FILE__,__LINE__,1,"Starting ExecLoop()");		

	for ( int i = 0; i < 3; i++ )
	{
		LOG(__FILE__,__LINE__,1,"Executing ExecLoop Iteration %d",i);		
	}
	
	// Test mutex-protected state
	int currState = GetState();
	LOG(__FILE__,__LINE__,1,"Current Thread State:  %d",currState);		
	
	// Wait for the signal indicating registry is ready to read
	m_Event.Wait();
	
	// Once signal is received, read the values written to the registry	
	Registry& reg = *(Registry::Instance());	
	int testInt = reg[RegKey_TestInt];
	LOG(__FILE__,__LINE__,1,"Read TestInt from Registry:  %d",testInt);			
	string strTest = reg[RegKey_TestString];
	LOG(__FILE__,__LINE__,1,"Read TestString from Registry:  %s",strTest.c_str());			
	double dblTest = reg[RegKey_TestDouble];
	LOG(__FILE__,__LINE__,1,"Read TestDouble from Registry:  %lf",dblTest);			
	
	// Shut down
	LOG(__FILE__,__LINE__,1,"Ending ExecLoop()");		
}

int main(void)
{
	// Initialize Logger
	
	Logger::init((char*)"Test_kbtools", c_LogBufferSize, c_LogFlushFrequency_ms, c_DaysToKeep, c_pLoggerDirectory, c_LogLevel);

	LOG(__FILE__,__LINE__,1,"Starting Test_kbtools...");
	LOGFLUSH();		

	// Initialize Registry, read from file
	
	LOG(__FILE__,__LINE__,1,"Reading Registry File %s\n",c_pRegistryFile);
	Registry& reg = *(Registry::Instance());	
	reg.ReadFromFile(c_pRegistryFile);
 	
	// Create Test Thread
	//
	// This tests the Thread, Event and Mutex classes
	// Also tests cross-thread registry usage
	
	TestThread t;
	
	t.SetState(100);
	
	reg[RegKey_TestInt] 	= 17;
	reg[RegKey_TestString] 	= "Test";
	reg[RegKey_TestDouble] 	= 3.1415926;
	
	t.Signal();
	
	// Wait awhile, allowing the test thread to do it's work 
	Wait(1);	// NOTE:  Wait() periods are in units of seconds
	
	LOG(__FILE__,__LINE__,1,"Ending Test_kbtools...");	
	LOGFLUSH();
}
