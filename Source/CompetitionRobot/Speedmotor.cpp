#include "Speedmotor.h"


Speedmotor::Speedmotor(UINT32 slot, UINT32 channel)
 : Jaguar(slot, channel)
{}


Speedmotor::Speedmotor(UINT32 channel)
 : Jaguar(channel)
{}


void Speedmotor::PIDWrite(float output)
{
  Set(output);
}
