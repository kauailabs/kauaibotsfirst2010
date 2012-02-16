/*
 * Thread.h
 *
 *  Created on: Oct 10, 2011
 *      Author: slibert
 */

/*
 * Description:
 */

#include "WPILib.h"
//#include "Mutex.h"

class Thread : public Task
{
protected:
        //Mutex m_Mutex;
public:
        Thread( const char *pThreadName, int priority = Task::kDefaultPriority );
        ~Thread();
        void Start();
        virtual void ExecLoop () = 0;
        //Mutex* GetMutex() { return &m_Mutex; }
};
