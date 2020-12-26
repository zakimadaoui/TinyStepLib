# TinyStepLibrary

This Arduino library provides an object oriented interface for controlling stepper motors with drivers similar to (DRV8825).

The library allows you to:
- use dergrees instead of steps
- allows for simultanious use of up to 6 motors without blocking the whole program
- supports high and low speeds in deg/sec
- more features will be added soon ...

Please note that:

* This is still experimental and requires more tuning
* The library uses TIMER2 of the atmega328 microcontroller, so make sure not to use libraries that occupy the same timer (ex: tone()).


The "main.cpp" file shows an example of how to use the library
