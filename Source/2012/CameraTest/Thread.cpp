/*  
 * Thread.cpp  
 * 
 * Created on: Oct 10, 2011
 * Author: slibert  
 */  

#include "Thread.h"  

void InternalExecLoop( Thread *theThread ) 
{         
	theThread->ExecLoop(); 
}  

/*  
 * Default constructor  
 */ 

Thread::Thread( const char *pThreadName, int priority ) :
		Task( pThreadName, (FUNCPTR) InternalExecLoop, priority) 
{         
}  

void Thread::Start()
{
	// Start the Task         
	if (!Task::Start((INT32)this))         
	{          
		char msgBuffer[512];
		sprintf(msgBuffer, "Unable to Start thread %s", Task::GetName() );
		wpi_assertWithMessage( false, msgBuffer );
	}        	
}

/*  
 * Default destructor  
 */

Thread::~Thread() 
{          
}
