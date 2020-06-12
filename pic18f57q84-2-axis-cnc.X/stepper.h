/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. 
    
    Subject to your compliance with these terms, you may use Microchip software and any 
    derivatives exclusively with Microchip products. It is your responsibility to comply with third party 
    license terms applicable to your use of third party software (including open source software) that 
    may accompany Microchip software.
    
    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER 
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY 
    IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS 
    FOR A PARTICULAR PURPOSE.
    
    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP 
    HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO 
    THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL 
    CLAIMS IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT 
    OF FEES, IF ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS 
    SOFTWARE.
 */

/******************************************************/

#ifndef STEPPER_H
#define	STEPPER_H

/******************************************************/

#include "pin.h"
#include "slave_clock.h"

/******************************************************/

// Return values of stepper functions
typedef uint8_t StepperError;

enum ERRORS {
    NO_ERROR, OUT_OF_BOUNDS, HOMING_ERROR
};

/******************************************************/

// Should not be created directly, factory should be used instead
typedef struct StepPin {
    uint8_t clc;
    SlaveClock clock;
} StepPin;

typedef struct Stepper {
    uint16_t position, maximum;
    StepPin step;
    Pin direction, limit;
    bool reversed;

} Stepper;

/******************************************************/

// Function automatically sets supplied pin to required configuration
Stepper Stepper_Initialize(uint16_t maximum, Pin enable, StepPin step, Pin direction, Pin limit, bool reversed, uint8_t clk_pps);

void Stepper_SetNCO(Stepper stepper, uint8_t source);

StepPin StepPin_Initialize(uint8_t group, uint8_t position, SlaveClock clock, uint8_t clc);

// Causes Stepper 'stepper' to manually travel 'steps' steps
// Will stop at the defined limits of the Stepper (i.e 0 and maximum)
StepperError Stepper_GoStepsManual(Stepper * stepper, int16_t steps);

StepperError Stepper_Home(Stepper * stepper);

void Stepper_SetInc(Stepper stepper, uint24_t value);

void Stepper_SetBackwards(Stepper stepper);
void Stepper_SetForwards(Stepper stepper);

/******************************************************/

#endif

