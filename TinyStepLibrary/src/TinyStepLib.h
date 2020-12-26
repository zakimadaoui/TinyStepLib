#ifndef TinyStepLib_h
#define TinyStepLib_h

#include "Stepper.h"
namespace zmdev{
class TinyStepLib{

public:
    static void begin();
    static void attach(Stepper* stpr);
    static void detach(Stepper* stpr);
};
}

#endif