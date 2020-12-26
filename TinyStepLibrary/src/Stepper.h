#ifndef Stepper_h
#define Stepper_h

#include <stdint.h>
#include <Arduino.h>

namespace zmdev {
class Stepper{
private:
    bool         started;
    uint8_t      step_pin_port_bit;
    uint8_t      step_pin_port_reg = -1;
    uint8_t      dir_pin_port_bit;
    uint8_t      dir_pin_port_reg = -1;
    uint8_t      dir;
    uint8_t      stepper_count = 0;
    double       angle_per_step;
    uint16_t     current_angle = 0;
    uint16_t     target_angle = 0;
    uint32_t     target_steps;
    uint16_t     current_step = 0;
    double       speed;
    const double MAX_SPEED = 10000; 
    uint16_t     tick = 0;
    uint16_t     TICKS = 0;



public:
    Stepper(String step_pin, String dir_pin, double speed, double angle_per_step, uint16_t init_angle, uint8_t dir);
    //-----------------------------------
    void moveWith(uint16_t angle);
    void actuate();
    void stop();
    //-----------------------------------
    void setSpeed(double speed);
    double  getSpeed();
    double  getCurrentSpeed(); //todo later when adding motion planning
    double  getMaxSpeed(); 
    //-----------------------------------
    void setDirection(uint8_t dir);
    uint8_t  getDirection();
    //-----------------------------------
    void setStepperCount(uint8_t stepper_count);
    uint8_t getStepperCount();
    //-----------------------------------
    void setOnTargetReachedCallback(void (*onTargetReached)(void));
    //-----------------------------------
    bool isStarted();
    //-----------------------------------

private:

    void (*__onTagetReached)(void); 
    void parseStepPin(String step_pin);
    void setPinAsOutput(String pin_name, uint8_t &port_bit, uint8_t &port_reg);
    void toggleStepPin();
};
}
#endif