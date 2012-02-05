#ifndef CHUTE_H
#define CHUTE_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class Chute: public CommandBase {
public:
	Chute();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
