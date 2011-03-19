#include "pneumatics.h"
#include "WPILib.h"
/*
 * 1 & 2 are for Fire
 * 
 * 3 & 4 are for Lock
 * 
 * slot 4 & relay 1 are for the spike
 * 
 */

pneumatics::pneumatics(UINT32 Lock1channel, UINT32 Lock2channel, UINT32 Fire1channel, UINT32 Fire2channel,
		UINT32 PressureSwitchchannel, UINT32 Compresschannel)
	:m_Lock1 (8, Lock1channel)
	,m_Lock2 (8, Lock2channel)
	,m_Fire1 (8, Fire1channel)
	,m_Fire2 (8, Fire2channel)
	,m_Compress (4, PressureSwitchchannel, 4, Compresschannel)
{
	m_Fire1.Set(true);
	m_Fire2.Set(false);
	m_Lock1.Set(true);
	m_Lock2.Set(false);
	m_Compress.Start();
}


void pneumatics::Deployment(bool b14)
{
	if (b14 != 0)
	{
	m_Lock1.Set(false);
	m_Lock2.Set(true);
	Wait(0.5);
	m_Fire1.Set(false);
	m_Fire2.Set(true);
	Wait(3.0);
	m_Fire1.Set(true);
	m_Fire2.Set(false);
	}
}
void pneumatics::UnDeployment(bool b14)
{
	if (b14 != 0)
	{
	m_Lock1.Set(true);
	m_Lock2.Set(false);
	m_Fire1.Set(true);
	m_Fire2.Set(false);
	}
}

void pneumatics::CompressedStop(bool b9)
{
	if (b9 != 0)
	m_Compress.Stop();
}

	
pneumatics::~pneumatics()
{
}

