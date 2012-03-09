#ifndef WAITTIME_H
#define WAITTIME_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Quinn
 */
class WaitTime: public CommandBase {
public:
	WaitTime(float runTime);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	float m_runTime;
};

#endif
