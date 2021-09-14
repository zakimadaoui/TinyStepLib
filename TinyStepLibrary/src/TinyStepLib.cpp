#include <Arduino.h>
#include "TinyStepLib.h"


static Stepper *steppers[MAX_STEPPERS] = {nullptr};
static uint8_t steppers_count = 0;

// setup the timer
void TinyStepLib::init(){

    // init the timer/counter control register (set it to normal mode)
    TCCR2A = 0; 

    // set the timer on CTC mode instead of normal mode, (Clear Timer on Compare match)
    TCCR2A |= (1<< WGM21); 

    // selecting clk T2S /8 (From prescaler)
    // this will provide a square signal with 2MHZ freq | 500 ns period 
    TCCR2B &= ~(1 << CS20); 
    TCCR2B |=  (1 << CS21); 
    TCCR2B &= ~(1 << CS22); 

    // reset counter to 0
    TCNT2 = 0; 

    // set compare register A to value 49 so it counts from 0->49 (50), in combination with 16MHZ 
    // clock and an 8 prescaler you get a clock of 40KHz -> 25us
    // so full speed is 40 000 step/sec !
    // Althouhg this heoretical (full) speed is huge. it gives more resolution
    // for varrying the speed at a smaller scale 
    OCR2A = 49; 

    TIMSK2 |= (1 << OCIE2A); //enable compare match interupt A

    // Enable global interrupts
    sei(); 
}


// the attach detach mechanism allows multiplexing, ie: more than the MAX_STEPPERS limit

void TinyStepLib::attach(Stepper& stepper){

    if (steppers[steppers_count] == nullptr)
    {
        steppers[steppers_count] = &stepper;
    }
    
    for (uint8_t i = 0; i < MAX_STEPPERS; i++)
    {
        if (steppers[i] == nullptr)
        {
            steppers[i] = &stepper;
            break;
        }
    }
    steppers_count = (steppers_count + 1) % MAX_STEPPERS;
}

void TinyStepLib::detach(Stepper& stepper){
    steppers[stepper.getStepperIndex()] = nullptr;
}


ISR(TIMER2_COMPA_vect){
    for (Stepper* stpr : steppers){
        if(stpr != nullptr) stpr->makeAstep();
    }
}
    