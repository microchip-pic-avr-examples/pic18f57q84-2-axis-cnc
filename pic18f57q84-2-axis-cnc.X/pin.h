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

#ifndef PIN_H
#define	PIN_H

/******************************************************/

#include <stdint.h>
#include <stdbool.h>
#include "types.h"

/******************************************************/

enum PORTS{RA, RB, RC, RD, RE, RF};

// Should not be created directly, factory should be used instead
typedef struct Pin{
    RegisterPtr port;
    RegisterPtr lat;
    uint8_t group;
    uint8_t mask;
    uint8_t position;
} Pin;

// Example pin on RD3
//Pin example = InitializePin(RD, 3);
Pin Pin_Initialize(uint8_t group, uint8_t position);

// Set Pin 'pin' as an output/input
void Pin_MakeOutput(Pin pin);
void Pin_MakeInput(Pin pin);

// Set Pin 'pin' high/low
void Pin_SetHigh(Pin pin);
void Pin_SetLow(Pin pin);

void Pin_SetPPS(Pin pin, uint8_t value);

// Gets the current PORT value of Pin 'pin'
bool Pin_Get(Pin pin);

#endif

