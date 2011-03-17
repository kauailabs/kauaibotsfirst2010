#ifndef __sloganarm_h__
#define __sloganarm_h__

#include "WPILib.h"

class sloganarm
{
protected:
	CANJaguar m_shoulder;
	CANJaguar m_hand;
	CANJaguar m_elev1;
	CANJaguar m_elev2;
	
public:
	sloganarm (UINT32 shoulderCANAddress,UINT32 handCANAddress, UINT32 elev1CANAddress, UINT32 elev2CANAddress);
	virtual void DoShoulder(bool b4, bool b2);
	virtual void DoHand(bool b5, bool b3);
	virtual void DoElevator1(bool b6, bool b7);
	virtual void DoElevator2(bool b8, bool b9);
		
	virtual ~sloganarm();
	
};
#endif
 
