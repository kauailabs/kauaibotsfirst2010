#ifndef CAMERA_H
#define CAMERA_H

#include "../CommandBase.h"

/**
 *
 *
 * @author slibert
 */
class CameraUp: public CommandBase {
        bool bCameraReady;
public:
        CameraUp(bool bUp);
        virtual void Initialize();
        virtual void Execute();
        virtual bool IsFinished();
        virtual void End();
        virtual void Interrupted();
        bool m_up;
};

#endif
