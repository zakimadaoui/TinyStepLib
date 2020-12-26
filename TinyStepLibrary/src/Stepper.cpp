#include "Stepper.h"
#include <Arduino.h>

using namespace zmdev;

Stepper::Stepper(String step_pin, String dir_pin, double spd, double angle_per_step, uint16_t init_angle, uint8_t dir = 1):
started(false),
angle_per_step(angle_per_step),
current_angle(init_angle)
{
    //make sure the the dir is either 0 or 1
    this->dir  = dir %2;

    //calculate the speed in step/sec instead of deg/sec
    speed = spd/angle_per_step;

    if (speed > MAX_SPEED) speed = MAX_SPEED;
    if (speed < 1) speed = 1;

    //adjust the TICKS to the speed 
    TICKS = round(MAX_SPEED/speed);

    //setup the pins for the stepper
    setPinAsOutput(step_pin, step_pin_port_bit, step_pin_port_reg);
    setPinAsOutput(dir_pin, dir_pin_port_bit, dir_pin_port_reg);
    setDirection(dir);
}

void Stepper::moveWith(uint16_t angle){
    /*
    if(current_step != 0){
        //handel the case when this function is called while the motor hasn't finished yet
        //the motor should continue from the angle that it was stopped at
        started = false;
        current_step = 0;
        tick = 0;
        current_angle = (current_step * angle_per_step)/2.0f ;
    }else{
        stop();
    }
    */

    stop();

    target_angle = angle;
    int diff = target_angle - current_angle;
    dir = diff < 0 ? 0 : 1 ; //correct the direction
    //setDirection(dir);
    target_steps = round(abs(diff) * 2.0 / angle_per_step ) ;// multiply by 2 due to the clock division caused by the toggeling of the step_pin_bit at each clock cycle
    started = true;

}

void Stepper::actuate(){
    { 
        if(started){
            ++tick;
            if(tick == TICKS){ //this condition controlls the speed of the motor
                tick = 0;
                if(current_step != target_steps){
                    toggleStepPin();
                }else{
                    stop();
                    __onTagetReached();
                    return;
                }
                current_step++;
            }
        }
    }
}

void Stepper::stop(){
    started = false;
    current_angle = 0;//todo: this may be changed later
    target_steps = 0;
    current_step = 0;
    tick = 0;
}

void Stepper::setDirection(uint8_t dir){

    /*
    * set the direction for the stepper motor, dir mabe only 1 or 0
    * any value other than that will cause the function to exit.
    * 
    * This function may not work properly if used while the stepper is working 
    * since "setup time" and "hold time"  conditions may not meet the ones required
    * by the stepper driver. 
    * 
    * use this function with care and preferebly use it when the motor stops of before even starting it
    * the onTargetReached callback bay help with that too.
    * */

    if (dir > 1) return;

    this->dir = dir;
    //cli();
   
    if (dir == 1) //set dir pin HIGH
    {
        if      (dir_pin_port_reg == 0) PORTD |= (1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 1) PORTB |= (1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 2) PORTC |= (1 << dir_pin_port_bit);

    }else         //set dir pin LOW
    {
        if      (dir_pin_port_reg == 0) PORTD &= ~(1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 1) PORTB &= ~(1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 2) PORTC &= ~(1 << dir_pin_port_bit);
    }
    
    //sei();
}

uint8_t  Stepper::getDirection(){
    return dir;
}

void Stepper::setSpeed(double spd){
    speed = spd/angle_per_step;
    TICKS = round(MAX_SPEED/speed);
}

double  Stepper::getSpeed(){
    //this returns the speed in deg/sec
    return speed * angle_per_step;
}

double  Stepper::getMaxSpeed(){
    //this returns the MAX_SPEED in deg/sec
    return MAX_SPEED * angle_per_step;
}

bool Stepper::isStarted(){
    return started;
}

void Stepper::toggleStepPin(){
    if      (step_pin_port_reg == 0) PORTD ^= (1 << step_pin_port_bit);
    else if (step_pin_port_reg == 1) PORTB ^= (1 << step_pin_port_bit);
    else                             PORTC ^= (1 << step_pin_port_bit);
}

void Stepper::setPinAsOutput(String pin_name, uint8_t &port_bit, uint8_t &port_reg){

    port_bit = (pin_name.charAt(2) - '0');

    const String p  =pin_name.substring(0,2);
    if(p.equals("PD")){
        DDRD |= (1 << port_bit); //set as output
        port_reg = 0;
    }
    else if(p.equals("PB"))
    {
        DDRB |= (1 << port_bit); //set as output
        port_reg = 1;
    }
    else
    {
        DDRC |= (1 << port_bit); //set as output
        port_reg = 2;
    }
}




void Stepper::setStepperCount(uint8_t stepper_count){
        this->stepper_count = stepper_count;
    };

uint8_t Stepper::getStepperCount(){
    return stepper_count;
};

void Stepper::setOnTargetReachedCallback(void (*callback)(void)){
    __onTagetReached = callback;
}