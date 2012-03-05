#ifndef DRIVEBRIDGE_H
#define DRIVEBRIDGE_H

#include "Drive.h"

class DriveBridge : public Drive
{
public:
        DriveBridge();
        void Initialize();
        void Execute();
        bool IsFinished();
        void End();
        void Interrupted();
private:
        bool m_bAutoBalanceMode;
};
#endif
