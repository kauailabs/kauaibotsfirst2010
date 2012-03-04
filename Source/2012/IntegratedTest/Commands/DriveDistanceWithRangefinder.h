#ifndef DRIVEDISTANCEWITHRANGEFINDER_H
#define DRIVEDISTANCEWITHRANGEFINDER_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Quinn
 */
class DriveDistanceWithRangefinder: public CommandBase {
public:
	DriveDistanceWithRangefinder();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
