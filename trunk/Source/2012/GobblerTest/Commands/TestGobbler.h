#ifndef Test_Gobbler_H
#define Test_Gobbler_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class TestGobbler: public CommandBase {

public:
	TestGobbler();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
