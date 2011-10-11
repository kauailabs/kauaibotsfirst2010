/*
 * Event.h
 *
 *  Created on: Oct 9, 2011
 *      Author: slibert
 */

/*
 * Description:
 */

#ifndef _Event_H
#define _Event_H

#include "WPILib.h"

enum EACTEventTp {eESignaled , eETimeout , eEError };

class Event {
protected:

	SEM_ID m_vxworksSem;
    int m_timeout_in_ms;
    
    void Init(int timeout_in_ms);    
    
public:
    Event(Event& copy);
    Event(int timeout_in_ms = WAIT_FOREVER);	// To wait forever, set to WAIT_FOREVER
    ~Event();
    EACTEventTp Wait();
    void Signal();
    
    // Sets timeout.  To wait forever, set to WAIT_FOREVER;
    
    void SetTimeout(int timeout_in_ms){ m_timeout_in_ms = timeout_in_ms;}	
};

#endif // _Event_H
