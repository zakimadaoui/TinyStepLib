#include <Arduino.h>
#include "TinyStepLib.h"

using namespace zmdev;

const  static int MAX_STEPPERS = 6; //allowing many motors will have negative effect on the accuracy of the speed required from the motors
static Stepper* steppers[MAX_STEPPERS];
uint8_t  steppers_count = 0;

void TinyStepLib::begin(){

    if (steppers_count > 0) return ;//ensure that begin is called only ones when steppers are already setup
   
    //init the step count
    steppers_count = 0;

    //setup the timer:

    //init the timer/counter control register (set it to normal mode)
    TCCR2A = 0; 

    //set the timer on CTC mode instead of normal mode, (Clear Timer on Compare match)
    TCCR2A |= (1<< WGM21); 

    //selecting clk T2S /8 (From prescaler)
    //this will provide a square signal with 2MHZ freq | 500 ns period 
    TCCR2B &= ~(1 << CS20); 
    TCCR2B |=  (1 << CS21); 
    TCCR2B &= ~(1 << CS22); 

    //reset counter to 0
    TCNT2 = 0; 

    //set compare register A to value 100, 
    //in combination with 16MHZ clock of atmega328 and an 8 prescaler
    //a clock of 25us period is generated
    //this will allow a maximum speed of 10000 step/sec or (1125 deg/sec in case of 1.8° stepper with 16 microstepping)
    OCR2A = 100; 

    TIMSK2 |= (1 << OCIE2A); //enable compare match interupt A

    //Enable global interrupts
    sei(); 
}

void TinyStepLib::attach(Stepper* stepper){
    steppers[steppers_count] = stepper;
    steppers_count = (steppers_count + 1) % MAX_STEPPERS;
}

void TinyStepLib::detach(Stepper* stepper){
    steppers[stepper->getStepperCount()] = nullptr;
}


ISR(TIMER2_COMPA_vect){
    for (Stepper* stpr : steppers){
        if(stpr != nullptr) stpr->makeAstep();
    }
}
    