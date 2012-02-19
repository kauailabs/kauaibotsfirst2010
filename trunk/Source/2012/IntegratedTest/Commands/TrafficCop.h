#ifndef TRAFFIC_COP_H
#define TRAFFIC_COP_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class TrafficCop: public CommandBase {
public:
        TrafficCop();
        virtual void Initialize();
        virtual void Execute();
        virtual bool IsFinished();
        virtual void End();
        virtual void Interrupted();
};

#endif
