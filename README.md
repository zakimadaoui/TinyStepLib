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

### Setting the direction and speed

The direction can be either 0 or 1. Generally 0 is clockwize and 1 is counterclockwize, but it can be reversed due to the stepper motor connection with the driver.

```cpp
stepper_1.setDirection(0);
```

The speed can be specified in Degrees/sec or in Steps/sec. The errors in the actual speed are discusses in the **Speed errors** section.

```cpp
//set speed to 170°/s
stepper_1.setSpeedInDegrees(170); 

//set speed to 107 steps/sec
stepper_1.setSpeedInSteps(107);
```

### Controlling the position

You can move the stepper motor with an relative angles or in steps or even spin it (keep it rotating).

```cpp
// in angles
stepper_1.moveWithAngle(360.0);
// in steps
stepper_1.moveWithSteps(100);
// spin the motor (keep rotating)
stepper_1.spin()
// call: stepper_1.stop() to stop the motor from spinning
```

### Callback

You can set a callback for each motor so that after the function `moveWithAngle()` or `moveWithSteps()` is called a callback will be triggered after the motor reaches its position (Since this is open loop, it is not garenteed that the stepper will evetually reach that position).  

This callback mechanism allows you to add more functionality. For example, if you are using a position encoder, when the callback triggers you can check whether the motor truly reached its target position. And if there is any step losses you can compensate for it.

```cpp
void onTargetReached1(){
//your logic
}

void setup(){
//...
stepper.setOnTargetReachedCallback(onTargetReached1);
//...
}

```


### Speed errors

As long as you operate the stepper motor at low speeds (less than °/sec or less than steps/sec) then the actual speed of the motor will be very accurate. Howoever, the larger you set the speed, the more it will drift from the desired value.

Another factor that plays an important role in the accuracy of the speed is the micro stepping used. Believe it or not, the more micro stepping you use the less accurate the speed will be. 

You can take a look at the graphs to get a better understanding of both phemomenons mentionned above. They show the difference between the desired speed and the actual one at different micro-stepping settings. The motor used for this graph has a 1.8° step angle.



