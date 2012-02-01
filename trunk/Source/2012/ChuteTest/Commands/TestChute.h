#ifndef TEST_CHUTE_H
#define TEST_CHUTE_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class TestChute: public CommandBase {
public:
	TestChute();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
