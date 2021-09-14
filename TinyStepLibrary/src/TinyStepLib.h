#ifndef TinyStepLib_h
#define TinyStepLib_h

#include "Stepper.h"

//TODO: mention this limit in github repo
#define MAX_STEPPERS 6

class TinyStepLib{

public:

    static void init();
    static void attach(Stepper& stpr);
    static void detach(Stepper& stpr);

private:
    TinyStepLib();
    ~TinyStepLib();
    TinyStepLib(const TinyStepLib&) = delete;
    TinyStepLib &operator=(const TinyStepLib &src) = delete;

};
#endif