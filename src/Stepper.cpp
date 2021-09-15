#include "Stepper.h"
#include <Arduino.h>

uint16_t getPortAddress(char port){
    switch (port)
    {
    case 'B':
        return 0x05;

    case 'C':
        return 0x08;

    default://'D'
        return 0x0B;
    }
}

uint16_t getDDrAddress(char port)
{
    switch (port)
    {
    case 'B':
        return 0x04;

    case 'C':
        return 0x07;

    default: //'D' 
        return 0x0A;
    }
}

Stepper::Stepper(char *step_port_pin, char *dir_port_pin, double stepper_angle, int micro_stepping)
{
    // set both step and dir ports and pins
    step_port_addr = getPortAddress(step_port_pin[0]);
    step_pin       = step_port_pin[1] - '0';

    dir_port_addr  = getPortAddress(dir_port_pin[0]);
    dir_pin        = dir_port_pin[1] - '0';

    // set both step and dir as output
    _SFR_IO8(getDDrAddress(step_port_pin[0])) |= (1 << step_pin);
    _SFR_IO8(getDDrAddress(dir_port_pin[0]))  |= (1 << dir_pin );

    // init the direction
    setDirection(0);

    //calculate stepper resolution
    this->angle_per_step = stepper_angle / micro_stepping;
    // set default speed
    setSpeedInDegrees(100);

}

void Stepper::setDirection(uint8_t dir)
{
    dir = dir % 2;                               //correct the dir if not 1 or 0
    this->dir = dir;
    
    if (dir == 0) _SFR_IO8(dir_port_addr) &= ~(1 << dir_pin); //set the dir pin to 0
    else          _SFR_IO8(dir_port_addr) |= (1 << dir_pin); //set the dir pin to 1
}

uint8_t Stepper::getDirection() {return dir;}


void Stepper::pulseStepPin()
{
    _SFR_IO8(step_port_addr) &= ~(1 << step_pin); //set pin to 0
    //_delay_us(1);                               //delay for meeting condition
    _SFR_IO8(step_port_addr) |= (1 << step_pin); //set pin to 1
}


void Stepper::makeAstep()
{
    if (isInSpinMode) pulseStepPin();
    else if (rotateMotor)
    {
        current_tick++;
        if (current_tick == ticks_for_one_step) //this condition controlls the speed of the motor
        {
            current_tick = 0;
            if (current_step != target_steps) // target angle not reached yet
            {
                pulseStepPin();
                current_step++;
            }
            else
            {
                stop(); //stop the motor
                if (__onTagetReached != nullptr) __onTagetReached();// trigger callback, 
            }
        }
    }
}


//this takes only positive values 
void Stepper::moveWithAngle(double angle){
    //stop, cancel spin and reset current_steps
    stop(); 
    // calculate target steps
    target_steps = abs(round(angle / angle_per_step));
    //move towards target
    rotateMotor = true;
}

void Stepper::moveWithSteps(uint32_t steps)
{
    //stop, cancel spin and reset all states
    stop();
    // calculate target steps
    target_steps = steps;
    //move towards target
    rotateMotor = true;
}



void Stepper::stop(){
    rotateMotor = false;
    isInSpinMode = false;
    current_tick = 0;
    current_step = 0;
    target_steps = 0;
}

void Stepper::spin(double spd){
    isInSpinMode = true;
}


void Stepper::setSpeedInDegrees(uint16_t spd)
{
    speed = abs(spd) / angle_per_step; //convert deg/sec to step/sec
    if (speed >= MAX_SPEED) speed = MAX_SPEED;
    ticks_for_one_step = round(MAX_SPEED / speed);
    current_tick  = 0; // reset tick
}

void Stepper::setSpeedInSteps(uint16_t spd)
{
    speed = abs(spd); 
    if (speed >= MAX_SPEED) speed = MAX_SPEED;
    ticks_for_one_step = round(MAX_SPEED / speed);
    current_tick = 0; // reset tick
}

uint16_t Stepper::getSpeedInDegrees() { return (uint16_t)(speed * angle_per_step); }
uint16_t Stepper::getSpeedInSteps() { return (uint16_t)speed; }

void Stepper::setStepperIndex(uint8_t stepper_index){
    this->stepper_index = stepper_index;
};

uint8_t Stepper::getStepperIndex(){
    return stepper_index;
};

void Stepper::setOnTargetReachedCallback(void (*callback)(void)){
    __onTagetReached = callback;
}
