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
	ShootNow(bool bShoot);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	
	bool m_bShoot;
};

#endif
