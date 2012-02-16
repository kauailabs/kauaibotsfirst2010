#ifndef GOBBLER_H
#define GOBBLER_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class Gobbler: public CommandBase {
public:
	Gobbler();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
