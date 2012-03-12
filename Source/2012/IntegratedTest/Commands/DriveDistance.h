#ifndef DRIVEDISTANCE_H
#define DRIVEDISTANCE_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Quinn
 */
class DriveDistance: public CommandBase {
public:
	DriveDistance(bool x,double distanceInches,double RPMs,bool ifRange);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
	bool m_ifRange;
	double m_distanceInches;
	bool m_x;
	double m_RPMs;
};

#endif
