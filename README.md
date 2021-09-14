# TinyStepLibrary

This library provides an object oriented interface for controlling stepper motors with drivers similar to (DRV8825). It is based on the 8-bit Timer/Counter2 that comes with the following AVR MCUs: ATmega48A/PA/88A/PA/168A/PA/328/P which means it supports the Arduino Uno board.

## Features
- simultaneous control of up to 6 stepper motors
- control the motor position by angle or steps
- control the motor speed in deg/sec and in step/sec
- other code can still execute while motors are rotating (depends on the number of motor and their speed)

## Disclaimer
This library uses the 8-bit Timer/Counter2 of the AVR microcontroller, so make sure not to use Arduino libraries and functions that use the same timer. One example function that uses the same timer is `tone()`, you have to check for other functions yourself.

## Usage

First include the header file of the library

```cpp
#include "TinyStepLib.h"
```
### creating the Stepper instances

Since this library assumes that you will be using a stepper motor driver similar to the drv8825 or a4988, then for each motor you will have a **STEP** Pin and a **DIR** pin.  A `Stepper` instance can be created as follows:

```cpp
//parameters
char step_port_pin[2]  = "D1";
char dir_port_pin[2]   = "D2";
double motor_angle     = 1.8 ; // 1.8° per step
int micro_stepping     = 16  ; 

// creating a new instance
Stepper stepper(step_port_pin, dir_port_pin, motor_angle, micro_stepping);
```

* **step_port_pin**: this is a two charachter string that specifies which I/O port (B,C or D) and which pin in the port are used for the **STEP** pin. 
* **dir_port_pin**: this is a two charachter string that specifies which I/O port (B,C or D) and which pin in the port are used for the **DIR** pin. 
* **motor_angle**: the step angle of the motor (without micro stepping)
* **micro_stepping**: the micro-stepping used

For example, if an Arduino uno board(with Atmega328p) is used and the **STEP** pin is pin 11 on the board, then ```step_port_pin = "B2"```.   
The following picture can help identifying the name of the other I/O ports and pins for the UNO board pins. If you are using another board you have to check its pin mapping.

![uno pin mapping](https://i.pinimg.com/originals/26/7f/60/267f60a5f7973c27e2bdc2b08b1fb845.png)


Now you can initialize the library and attach the motors. Remeber, a maximum of 6 motors can be attached at the same time. If you want to use more motors you will have to do some  multiplexing by attaching and detaching motors, 


```cpp
#include "TinyStepLib.h"
Stepper stepper("D1", "D2", 1.8, 16);

void setup(){
  //this initializes TIMER2
  TinyStepLib::init();

  //attach the motors
  TinyStepLib::attach(stepper_1);
  TinyStepLib::attach(stepper_2);
  
  //detach can be used to detach a motor and use another in its place
  // TinyStepLib::detach(stepper_6);
  // TinyStepLib::attach(stepper_7);
}

```



### controlling the position
details, code (SOON)

### controlling the speed

details, code and graphs (SOON)
