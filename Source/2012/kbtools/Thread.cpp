/*
 * Thread.cpp
 *
 *  Created on: Oct 10, 2011
 *      Author: slibert
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
	// Start the Task
	if (!this->Start((INT32)this))
	{
		wpi_fatal(CompressorTaskError);
	}	
}

/*
 * Default destructor
 */
Thread::~Thread()
{
	
}

