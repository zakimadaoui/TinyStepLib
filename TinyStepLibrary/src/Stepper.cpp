#include "Stepper.h"
#include <Arduino.h>

using namespace zmdev;

Stepper::Stepper(String step_pin, String dir_pin, double spd, double angle_per_step, double init_angle, uint8_t dir = 0):
canRotate(false),
angle_per_step(angle_per_step)
{
    current_angle = correctAngle(init_angle);
    current_step = getStepsFromAngle(current_angle);

    //calculate the speed in step/sec instead of deg/sec
    speed = spd/angle_per_step;

    if (speed > MAX_SPEED) speed = MAX_SPEED;
    if (speed < 1) speed = 1;

    //adjust the TICKS to the speed 
    TICKS = round(MAX_SPEED/speed);

    //setup the pins for the stepper
    setPinAsOutput(step_pin, step_pin_port_bit, step_pin_port_reg);
    setPinAsOutput(dir_pin, dir_pin_port_bit, dir_pin_port_reg);
    
    //make sure the the dir is either 0 or 1 and set it
    this->dir  = abs(dir %2);
    setDirection(dir);
}

//todo: create Move with steps too

void Stepper::moveWithAngle(double angle){
    
    if (isInSpinMode) return;
    

    //stop();
    // canRotate = false
    if (canRotate) {
        pendingAngleAvailable = true;
        pendingAngle = correctAngle(angle);      
        return;
    }
    
    //calculate current angle
    current_angle = current_step*angle_per_step/2; //don't forget to divide by 2 because of the previous multiplication in makeAstep function
    tick = 0;

    //check change in direction and adjust if needed
    target_angle = correctAngle(angle);
    double diff =  target_angle - current_angle;
    
    //correct the direction
    if (diff < 0) setDirection(1); //set dir to negative direction 
    else if (diff > 0)  setDirection(0);//set dir to positive direction

    
    target_steps = current_step + getStepsFromAngle(diff);

    //move towards target
    canRotate = true;
}

void Stepper::makeAstep(){
    { 
        if(canRotate || isInSpinMode){
            tick++;
            //if(true){ //operate at full speed
            if(tick == TICKS){ //this condition controlls the speed of the motor
                tick = 0;
                
                if (isInSpinMode) toggleStepPin();

                else if(current_step != target_steps){
                    toggleStepPin();
                    current_step+=dir_sign;//update steps nbr
                }else{
                    //current_step+=dir_sign; //update steps nbr
                    stop(); //stop the motor
                    if (pendingAngleAvailable)
                    {
                    pendingAngleAvailable = false;
                    moveWithAngle(pendingAngle);
                    }
                    //__onTagetReached();// todo: re enable this ... trigger a callback, this callback should be as fast as possible
                    return;
                }
            }
        }
    }
}

void Stepper::stop(){
    isInSpinMode = false;
    canRotate = false;
    current_angle = current_step*angle_per_step/2; //don't forget to divide by 2 because of the previous multiplication in makeAstep function
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
    * the onTargetReached callback may help with that too.
    * */

    //check flip in direction
    //if ((dir%2) != this->dir) dir_sign*=-1; 
    
    //set the direction
    this->dir = abs(dir % 2);

    if (this->dir == 1) //set dir pin HIGH (negative dir)
    {
         dir_sign = -1;

        if      (dir_pin_port_reg == 0) PORTD |= (1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 1) PORTB |= (1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 2) PORTC |= (1 << dir_pin_port_bit);

    }else         //set dir pin LOW (positive dir)
    {
        dir_sign = 1;
        if      (dir_pin_port_reg == 0) PORTD &= ~(1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 1) PORTB &= ~(1 << dir_pin_port_bit);
        else if (dir_pin_port_reg == 2) PORTC &= ~(1 << dir_pin_port_bit);
    }

}

uint8_t  Stepper::getDirection(){
    return dir;
}

void Stepper::spin(double spd){
    setSpeed(spd);
    if (spd<0) setDirection(1);
    else       setDirection(0);
    isInSpinMode = true;
}

void Stepper::setSpeed(double spd){
    speed = abs(spd)/angle_per_step;
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
    return canRotate;
}

void Stepper::toggleStepPin(){
    if      (step_pin_port_reg == 0) PORTD ^= (1 << step_pin_port_bit);
    else if (step_pin_port_reg == 1) PORTB ^= (1 << step_pin_port_bit);
    else                             PORTC ^= (1 << step_pin_port_bit);
}

void Stepper::toggleDirPin(){
    dir = abs(1-dir);
    if      (dir_pin_port_reg == 0) PORTD ^= (1 << dir_pin_port_bit);
    else if (dir_pin_port_reg == 1) PORTB ^= (1 << dir_pin_port_bit);
    else                            PORTC ^= (1 << dir_pin_port_bit);
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

void Stepper::setInitAngle(double angle){
    current_angle = correctAngle(angle);
    current_step = getStepsFromAngle(current_angle);   
}

double Stepper::correctAngle(double angle){
 return round(angle/angle_per_step)*angle_per_step;   
}

int Stepper::getStepsFromAngle(double angle){
    return 2* (int)round(angle/angle_per_step);// multiply by 2 due to the clock division caused by the toggeling of the step_pin_bit at each clock cycle
}