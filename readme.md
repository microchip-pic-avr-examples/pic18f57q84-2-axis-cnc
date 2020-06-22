<a href="https://www.microchip.com" rel="nofollow"><img src="images/microchip.png" alt="MCHP" width="300"/></a>

# PIC18F57Q84 Two Axis CNC Machine

This project demonstrates an implementation of a 2-axis computer numerical
control (CNC) machine which makes use of the Numerically Controlled Oscillator (NCO), Direct Memory Address (DMA) and other Core Independent Peripherals
(CIPs) to handle acceleration and movement with minimal input from the
microcontroller core.


## Theory of Operation

This is a general overview of the application. A more complete explanation can
 be found in the associated Appllication Note.

 Numerically Controlled Oscillators are a peripheral which work by creating
 an output frequency which is a precise fraction of the input frequency.
 This output frequency is a function of two values, the input frequency, and the
 value of the increment register of the NCO.
 This project makes use of three NCOs, a master NCO which determines overall speed,
  and a slave NCO for each axis which determines the proportion of speed in each
  axis, set up as shown below.

   ![CNC Overview](images/cnc.png)

For instance, if the goal is to move 200 mm in the X axis, and 100 mm in the Y
axis, the X NCO can be set to produce twice the frequency of output as the
Y NCO. This means that X will move twice as fast, and therefore cover twice
the distance.

Additionally, because both frequencies are a function of the master
frequency, if the master frequency is doubled, the speed in both axes will be
doubled, while maintaining the same proportion (e.g. X will still be going twice
as fast as Y, and will therefore still cover twice the distance).

Direct Memory Address (DMA) is a module which can copy data from one memory
location to another without any CPU intervention. DMA operation can be configured to start and/or abort based on several built-in trigger options. Acceleration of the machine can be achieved by using the DMA to gradually increase or decrease the value in the increment register of the NCO, resulting in the frequency of the master NCO, and therefore the speed of movement in the two axes, increasing or decreasing proportionally.

The distance travelled in each direction can be known by counting the number of
pulses from the master NCO. In short, this is because if the master
NCO has produced the requisite number of output ticks, the two slave NCOs have
moved the steppers in both axes the correct distance.


## Related Documentation

<!-- - [AN1234 - Whatever the Title Is](https://www.microchip.com/design-centers/8-bit/pic-mcus/device-selection/pic18f-q84-product-family)-->
- [PIC18F57Q43 Product Page](https://www.microchip.com/wwwproducts/en/PIC18F57Q43)


## Software Used


- MPLAB® X IDE 5.40 or newer [(microchip.com/mplab/mplab-x-ide)](http://www.microchip.com/mplab/mplab-x-ide)
- MPLAB® XC8 2.20 or newer [(microchip.com/mplab/compilers)](http://www.microchip.com/mplab/compilers)
- MPLAB® Code Configurator (MCC) 3.95.0 or newer [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
- MPLAB® Code Configurator (MCC) Device Libraries PIC10 / PIC12 / PIC16 / PIC18 MCUs [(microchip.com/mplab/mplab-code-configurator)](https://www.microchip.com/mplab/mplab-code-configurator)
- Microchip PIC18F-Q Series Device Support 1.7.130 or newer [(packs.download.microchip.com/)](https://packs.download.microchip.com/)

## Hardware Used


- PIC18F57Q43 Curiosity Nano [(DM182029)](https://www.microchip.com/Developmenttools/ProductDetails/DM164150)
- Curiosity Nano Base for Click boards™ [(AC164162)](https://www.microchip.com/Developmenttools/ProductDetails/AC164162)
- 2x Stepper Click board™ [(MIKROE-3402)](https://www.mikroe.com/stepper-click)
- 2 Axis CNC gantry - The code is designed to be flexible, such that it can
be modified to work with almost any XY stepper gantry selected, provided
the power requirements are not too great.
- 2x Limit switches - Switches should be located such that they are pressed when
the platform is at the (0, 0) position, and should be high when pressed.
- 2x stepper motors - The motors used will depend on the gantry, ensure
that they both move the end effector the same distance per tick, and must be
2-pole (4 wire) to work with the Stepper Click board™
- End Effector - Choice of end effector is likewise left flexible. As configured,
the end effector selected should be raised when the pin controlling it is low,
and lowered when the pin is high. This can be modified as needed to work with
a selected end effector.

![Platform](images/platform.png)

## Setup

Most configuration is done through the config.h file. The configurations are:

- `TICKS_PER_METER`: the number of steps of the stepper motor required to move the
end effector one meter
- `X_MAX`/`Y_MAX`: the maximum range, in steps, of the X and Y axes
- `X_BACKWARDS`/`Y_BACKWARDS`: whether to reverse the direction of the stepper
motor on this axis
- `BUFFER_NUMBER_PACKETS`: the size of the command buffer. This can be left at the
default 5

Pinout of the various pins (e.g. limit switches, the Click™ boards, the
actuator, etc) is defined in config.h with two constants per pin,
 `NAME_PIN_PORT` and `NAME_PIN_POS`.
 For example, to have pin Y_ENABLE on RA6, the code would be

```
#define Y_ENABLE_PIN_PORT RA
#define Y_ENABLE_PIN_POS 6
```

As configured, the Y axis Stepper Click board™ should go in slot 1, and the X
axis should be in slot 3 of the curiosity nano base board.

By default the limit switch pin for the Y axis is RA1, and the X axis is RC7.
The actuator is on pin RF2.

If an alternate method of controlling the actuator is desired, the relevant
functions to modify are `Platform_RaiseActuator()` and `Platform_LowerActuator()` in
platform.c which by default simply set ACTUATOR_PIN high/low.


## Operation

The CNC machine should be connected to the PC via USB. Operation of the PC
control GUI is explained in detail in the readme file in the pc-application
directory. An example G-Code that will draw a star is included in that
directory.

For the purposes of creating G-Code, the machine operates in the x and y axis,
with the location of the limit switches as (0, 0) and proceeding in the
positive direction. A value of Z0 will cause the end effector to lower, any
other value will cause it to raise.  Operation is limited to linear movement.
Movement in the Z axis and the X/Y plane simultaneously is not supported.

## Summary


The example G-Code included with the PC application creates the star shown below.

![Output](images/star.png)

This project demonstrates the ability of CIPs to complete tasks that would
traditionally require math and timing intensive tasks done by a more expensive
microcontroller.
