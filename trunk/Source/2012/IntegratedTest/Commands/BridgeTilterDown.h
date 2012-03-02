#ifndef BRIDGETILTERDOWN_H
#define BRIDGETILTERDOWN_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Quinn
 */
class BridgeTilterDown: public CommandBase {
public:
	BridgeTilterDown();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
