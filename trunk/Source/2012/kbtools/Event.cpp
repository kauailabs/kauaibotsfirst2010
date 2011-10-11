/*
 * Event.cpp
 *
 *  Created on: Oct 9, 2011
 *      Author: slibert
 */

#include "Event.h"

Event::Event(Event& copyObj)
{
	Init(copyObj.m_timeout_in_ms);
}

/*
 * Default constructor
 */
Event::Event(int timeout_in_ms)
{
	Init(timeout_in_ms);
}

void Event::Init(int timeout_in_ms)
{
	m_vxworksSem = semBCreate (	SEM_Q_PRIORITY,
			 					SEM_EMPTY);
	SetTimeout(timeout_in_ms);	
}


/*
 * Default destructor
 */
Event::~Event()
{
	semDelete(m_vxworksSem);
	m_vxworksSem = NULL;
}

const int us_per_ms = 1000; // Number of microseconds per millesecond

EACTEventTp Event::Wait()
{
	int timeout_in_ticks = m_timeout_in_ms * us_per_ms * SensorBase::kSystemClockTicksPerMicrosecond;
	STATUS status = semTake(m_vxworksSem, timeout_in_ticks);
	if ( status == OK ) return eESignaled;
	else if ( errno == S_objLib_OBJ_TIMEOUT ) return eETimeout;
	else return eEError;
}

void Event::Signal()
{
	semGive(m_vxworksSem);
}
