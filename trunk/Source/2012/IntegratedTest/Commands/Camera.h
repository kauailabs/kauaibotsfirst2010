#ifndef CAMERA_H
#define CAMERA_H

#include "../CommandBase.h"

/**
 *
 *
 * @author slibert
 */
class Camera: public CommandBase {
        bool bCameraReady;
public:
        Camera();
        virtual void Initialize();
        virtual void Execute();
        virtual bool IsFinished();
        virtual void End();
        virtual void Interrupted();
};

#endif
