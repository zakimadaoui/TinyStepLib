#ifndef Stepper_h
#define Stepper_h

#include <stdint.h>

#define MAX_SPEED 40000 // step/sec;

/* How speed works ?
 * get how many times the timer should tick before making a single step 
 * full speed is when a step is made for each timer tick
 * data needed: timer frequency = 40Khz -> this gives full speed of 40 000 step/sec
 *              angle_per_step -> gives angle/sec
 * 
 * so angle/sec -> step/sec -> steps -> MAX_STEPS/steps -> ticks
 * 
 * To simulate this run this matlab script:
 * 
 * MAX_SPEED = 40e3;
 * stepper_resolution = 1.8/16.0
 * steps_in_angle = round(1/stepper_resolution)
 * 
 * % observe steps drift with more resolution
 * 64/1.8
 * 32/1.8
 * 16/1.8
 * 4/1.8
 * 2/1.8
 * 1/1.8
 * 
 * % simulate deg/sec
 * desired = [0:10:400];
 * real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
 * figure(1)
 * plot(desired, real, desired,desired)
 * 
 * stepper_resolution = 1.8/32.0
 * real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
 * figure(2)
 * plot(desired, real, desired, desired)
 * 
 * stepper_resolution = 1.8/64.0
 * real = MAX_SPEED.*stepper_resolution./round(MAX_SPEED./(desired./stepper_resolution));
 * figure(3)
 * plot(desired, real, desired, desired)
 * 
 * % this simulates steps/sec instead of dergrees/sec
 * desired = [0:10:14222];
 * real = MAX_SPEED./round(MAX_SPEED./desired);
 * figure(4)
 * plot(desired, real, desired, desired)
 * */

class Stepper{
private:
    //-----------------------------------------------------------
    bool            rotateMotor = false;
    bool            isInSpinMode = false;
    uint8_t         stepper_index = 0;
    uint8_t         dir = 0;  //TODO: why keep track of the direction ????
    //-----------------------------------------------------------
    double          angle_per_step;
    uint32_t        target_steps = 0;
    uint16_t        current_step = 0;
    //-----------------------------------------------------------
    double          speed; // the speed in steps/sec
    uint16_t        ticks_for_one_step = 0;
    uint16_t        current_tick = 0;
    //-----------------------------------------------------------
    uint16_t        step_port_addr; // port address for step pin
    uint8_t         step_pin;       
    uint16_t        dir_port_addr;  // port address for dir pin
    uint8_t         dir_pin;       



public: 
    Stepper(char* step_port_pin, char* dir_port_pin, double stepper_angle, int micro_stepping);
    //-----------------------------------------------------------
    void moveWithAngle(double angle);
    void moveWithSteps(uint32_t steps);
    void makeAstep();
    void stop();
    //-----------------------------------------------------------
    void spin();
    void setSpeedInDegrees(uint16_t speed);
    void setSpeedInSteps(uint16_t speed);
    uint16_t getSpeedInDegrees();
    uint16_t getSpeedInSteps();
    //------------------------------------------------------------
    void setDirection(uint8_t dir);
    uint8_t  getDirection();
    //------------------------------------------------------------
    void setOnTargetReachedCallback(void (*onTargetReached)(void));
    //------------------------------------------------------------

private:
    void (*__onTagetReached)(void) = nullptr; 
    void pulseStepPin();
    void setStepperIndex(uint8_t stepper_index);
    uint8_t getStepperIndex();

    friend class TinyStepLib;
};

#endif