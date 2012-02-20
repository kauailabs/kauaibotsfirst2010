#ifndef TEST_TILTER_H
#define TEST_TILTER_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class Tilter: public CommandBase {
public:
        Tilter();
        virtual void Initialize();
        virtual void Execute();
        virtual bool IsFinished();
        virtual void End();
        virtual void Interrupted();
        bool m_tilterDown;
        bool m_lastButtonState;
};

#endif
