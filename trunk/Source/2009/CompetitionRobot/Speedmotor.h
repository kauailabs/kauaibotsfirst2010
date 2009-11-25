#ifndef SPEEDMOTOR_H
#define SPEEDMOTOR_H

#include "Jaguar.h"
#include "PIDOutput.h"

class Speedmotor : public Jaguar, public PIDOutput
{
  public:
    Speedmotor(UINT32 slot, UINT32 channel);
    Speedmotor(UINT32 channel);
    void PIDWrite(float output);

};


#endif
