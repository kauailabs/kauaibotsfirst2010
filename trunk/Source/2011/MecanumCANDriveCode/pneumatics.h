#ifndef _PNEUMATICS_H
#define _PNEUMATICS_H

#include "WPILib.h"
//#include "constants.h"

class pneumatics
{
protected:
	Solenoid m_Lock1;
	Solenoid m_Lock2;
	Solenoid m_Fire1;
	Solenoid m_Fire2;
	Compressor m_Compress;
	
public:
	pneumatics(UINT32 Lock1channel, UINT32 Lock2channel, UINT32 Fire1channel, UINT32 Fire2channel,
			UINT32 PressureSwitchchannel, UINT32 Compresschannel);
	virtual void CompressedStop (bool b9);
	virtual void Deployment (bool b14);
	virtual void UnDeployment (bool b14);
	
	virtual ~pneumatics();
};

#endif
