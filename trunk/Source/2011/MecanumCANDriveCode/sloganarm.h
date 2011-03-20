#ifndef __sloganarm_h__
#define __sloganarm_h__

#include "WPILib.h"
#include "constants.h"

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
	virtual void DoHand2(bool b1);
	virtual void DoElevator1(float);
	virtual void DoElevator2(float);
		
	virtual ~sloganarm();
	
};
#endif
 
