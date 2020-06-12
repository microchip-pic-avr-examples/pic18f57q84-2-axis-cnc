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

#include "stepper.h"
#include "hal.h"
#include "mcc_generated_files/mcc.h"
#include "config_macros.h"
#include <stdlib.h>

/******************************************************/

#define FAST_TIME 1500
#define SLOW_TIME 4

#define HOMING_BACKUP 100
#define HOME_MARGIN 10

/******************************************************/

void SelectClc(Stepper stepper) {
    CLCSELECT = stepper.step.clc;
}

void EnableNco(Stepper stepper) {
    NCO_Enable(stepper.step.clock.nco);
}

void DisableNco(Stepper stepper) {
    NCO_Disable(stepper.step.clock.nco);
}

void AdvanceManual() {
    CLCnPOLbits.POL = 1;
    __delay_us(FAST_TIME);
    CLCnPOLbits.POL = 0;
    __delay_us(FAST_TIME);
}

void AdvanceManualSlow() {
    CLCnPOLbits.POL = 1;
    __delay_ms(SLOW_TIME);
    CLCnPOLbits.POL = 0;
    __delay_ms(SLOW_TIME);
}

void Stepper_SetForwards(Stepper stepper) {
    if(stepper.reversed){
        Pin_SetLow(stepper.direction);
    } else {
        Pin_SetHigh(stepper.direction);
    }
}

void Stepper_SetBackwards(Stepper stepper) {
    if(stepper.reversed){
        Pin_SetHigh(stepper.direction);
    } else {
        Pin_SetLow(stepper.direction);
    }
}

StepperError Stepper_Home(Stepper * stepper) {
    SelectClc(*stepper);
    
    // Back up until it hits the limit switch
    Stepper_SetBackwards(*stepper);
    while (!Pin_Get(stepper->limit)) {
        AdvanceManual();
    }

    // Get a slight bit away from the limit switch
    Stepper_SetForwards(*stepper);
    uint8_t i;
    for (i = 0; i < HOMING_BACKUP; i++) {
        AdvanceManual();
    }

    // Check if switch is still considered pressed after backing up
    // If so, something is wrong, error out
    if (Pin_Get(stepper->limit)) {
        return HOMING_ERROR;
    }

    // Slowly back up until it hits the limit switch again
    Stepper_SetBackwards(*stepper);
    while (!Pin_Get(stepper->limit)) {
        AdvanceManualSlow();
    }

    // Give a little bit of margin off the limit switch
    Stepper_SetForwards(*stepper);
    for (i = 0; i < HOME_MARGIN; i++) {
        AdvanceManual();
    }

    // Stepper position is now homed
    stepper->position = 0;

    return NO_ERROR;
}

StepperError Stepper_GoStepsManual(Stepper * stepper, int16_t steps) {
    StepperError error = NO_ERROR;

    uint16_t magnitude = abs(steps);
    
    SelectClc(*stepper);

    if (steps >= 0) {
        Stepper_SetForwards(*stepper);
        uint16_t target = stepper->position + magnitude;

        if (target > stepper->maximum) {
            error = OUT_OF_BOUNDS;
            magnitude = stepper->maximum - stepper->position;
        }

        stepper->position += magnitude;

    } else {
        Stepper_SetBackwards(*stepper);
        if (magnitude > stepper->position) {
            error = OUT_OF_BOUNDS;
            magnitude = stepper->position;
        }

        stepper->position -= magnitude;
    }

    uint16_t i;
    for (i = 0; i < magnitude; i++) {
        AdvanceManual();
    }

    return error;
}

void Stepper_SetNCO(Stepper stepper, uint8_t source) {
    NCO_SetSource(stepper.step.clock.nco, source);
}

void Stepper_SetInc(Stepper stepper, uint24_t value) {
    NCO_SetInc(stepper.step.clock.nco, value);
}

Stepper Stepper_Initialize(uint16_t maximum, Pin enable, StepPin step, Pin direction, Pin limit, bool reversed, uint8_t clk_pps) {
    Pin_MakeOutput(enable);

    Pin_MakeOutput(direction);

    Pin_MakeInput(limit);

    Stepper stepper = {0, maximum, step, direction, limit, reversed};
    
    SelectClc(stepper);
    
    CLCnCON = 0;
    CLCnPOL = 0;
    CLCnSEL0 = 0;
    CLCnSEL1 = 0;
    CLCnSEL2 = 0;
    CLCnSEL3 = 0;
    
    CLCnGLS0 = 0;
    CLCnGLS1 = 0;
    CLCnGLS2 = 0;
    CLCnGLS3 = 0;
    
    CLCnCONbits.MODE = 0b010;
    CLCnSEL0 = clk_pps;
    CLCnGLS0bits.G1D1T = 1;
    
    CLCnPOLbits.G1POL = 0;
    CLCnPOLbits.G2POL = 1;
    CLCnPOLbits.G3POL = 1;
    CLCnPOLbits.G4POL = 1;
    
    CLCnCONbits.EN = 1;
    
    Pin_SetHigh(enable);
    
    EnableNco(stepper);

    return stepper;
}

StepPin StepPin_Initialize(uint8_t group, uint8_t position, SlaveClock clock, uint8_t clc) {
    Pin base_pin = Pin_Initialize(group, position);
    Pin_MakeOutput(base_pin);
    
    Pin_SetPPS(base_pin, CLC_1_OUT + clc);

    StepPin step_pin = {clc, clock};

    return step_pin;
}

/******************************************************/
