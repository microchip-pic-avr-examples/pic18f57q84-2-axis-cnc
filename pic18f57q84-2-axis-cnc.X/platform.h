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

#ifndef PLATFORM_H
#define	PLATFORM_H

/******************************************************/

#include "stepper.h"
#include "slave_clock.h"
#include <stdbool.h>

/******************************************************/

void Platform_Initialize(Stepper x_stepper, Stepper y_stepper, Pin actuator_pin);

// Returns true if the platform is currently being moved by CIPs
bool Platform_IsTravelling(void);

// Reset the platform to (0, 0)
StepperError Platform_Home(void);

// Set the direction both axes move
// 1 = forwards, 0 = backwards
// 0bxy
void Platform_SetDirection(uint8_t dir_info);

// Set the relative speed in x and y axis
void Platform_SetIncrements(uint24_t x, uint24_t y);

// Function that should be triggered by platform traveling a distance
void Platform_DistanceISR(void);

// Function that should be triggered accelerating
void Platform_AccelerationISR(void);

// Travel with a fix increment
void Platform_TravelFixedSpeed(uint16_t distance, uint32_t inc);

// Travel with speed ramping up to the value in the accel LUT at 'index'
void Platform_TravelRampedSpeed(uint16_t distance, uint16_t max);

void Platform_RaiseActuator(void);

void Platform_LowerActuator(void);

/******************************************************/

#endif
