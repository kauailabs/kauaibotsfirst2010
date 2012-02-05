#ifndef HOPPER_H
#define HOPPER_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class Hopper: public CommandBase {
public:
	Hopper();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
