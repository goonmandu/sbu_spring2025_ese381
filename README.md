# ESE 381 - Embedded Microprocessor Systems Design II
Semester: *Spring 2025*  
Instructor: *Prof. Kenneth Short*  
Course: *ESE 381, Section 01*

Originally authored by Mingi Hwang, a Stony Brook University student studying Computer Engineering.  
If you wish to contact me directly, send an email to hwang.mingi7@gmail.com, or send a friend request to goonmandu on Discord.

I have received an A for the course.

# Environment
Compilation target: Microchip AVR128DB48 (specifically, on the Curiosity Nano development board)<br>
The default compiler (AVR-GCC) and default options provided by Microchip Studio 7 were used.<br>
The UDPI debugger on-board the Curiosity Nano was used to debug all code.

# Directory Structure
`Global`: Contains the `libese381` library, its Doxygen documentation, and its Microchip Studio 7 solution/project files.<br>
`Exam`: Contains files I wrote to test Exam questions.<br>
`LabXX`: Contains relevant lab files.
  - `src`: Contains the C source code.
  - `schem`: Contains the KiCad schematics for the lab.

More specifically, the `libese381` library contains drivers for the following on-chip modules, peripherals, and data structures:
- USART1
- USART3
- SPI0 (master mode)
- TWI0 (master mode)
- Circular Byte Buffer
- ...and miscellaneous ASCII functions.

All solutions and projects can be opened with Microchip Studio directly via the `*.atsln` files.

# Warnings and Disclaimers
**THESE FILES ARE PROVIDED FOR REFERENCE USE ONLY.**

By copying my work and submitting them as your own, you are commiting **ACADEMIC DISHONESTY** and may be referred to the Stony Brook University Academic Judiciary.  
If found responsible, you **WILL** get an **F** as your final grade as per Prof. Short's academic honesty policy.  
I bear **NO RESPONSIBILITY** to your being found of cheating or any other consequence arising from your use of my code. This repository is for personal archival purposes, not to share my answers with other people.

Use online resources responsibly.

# Copyright Notice
The following parts of this repository were derived, adapted, or otherwise sourced from the listed sources:
- [AVR128DB48 Bare-Metal TWI Implementation by Microchip Technology](https://github.com/microchip-pic-avr-examples/avr128db48-bare-metal-twi-mplab/tree/master/twi-host.X/peripherals/TWI)
  - `Global/ese381/ese381/twi0_master.c`
  - `Global/ese381/ese381/twi0_master.h`
- [SCD41 API, HAL, and Example Usage by Sensirion AG](https://github.com/Sensirion/embedded-i2c-scd4x)
  - All of `Lab11/src/Lab11/Lab11/*` and `Lab11/src/Lab11/Lab11-Polling/*` except for the follwoing:
    - `Debug/`
    - `Lab11.componentinfo.xml`
    - `Lab11.cproj`
    - `Lab11-Polling.componentinfo.xml`
    - `Lab11-Polling.cproj`
    - The skeleton for `sensirion_i2c_hal.c`
      - The HAL implementation for both scenarion (ISR-based and polling) was written by myself and is licensed under the MIT License (same as this repository).

