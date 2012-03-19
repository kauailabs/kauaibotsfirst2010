#ifndef DRIVE_H
#define DRIVE_H

#include "../CommandBase.h"

class Drive : public CommandBase 
{
private:
		enum AutoBalanceState { kDisabled, kAdvancing, kAutoCorrecting, kEBraking };
public:
        Drive();
        void Initialize();
        void Execute();
        bool IsFinished();
        void End();
        void Interrupted();
        bool m_creepOn;
        bool m_lastButtonState;
        AutoBalanceState m_autoBalanceState;
        int  m_autoCorrectCompleteCount;
        float m_lastY;
        float m_maxPitch;
        bool  m_autoBalanceModeActive;
        int	  m_autoBalanceDirection;
protected:
        double InputJoystickAdjust( double dJoystickIn, double dAdjustment, double dExponent, double dMultiplier , double dDead);
};
#endif
