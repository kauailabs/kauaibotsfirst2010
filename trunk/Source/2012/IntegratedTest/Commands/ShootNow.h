#ifndef SHOOTNOW_H
#define SHOOTNOW_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Quinn
 */
class ShootNow: public CommandBase {
public:
	ShootNow();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
