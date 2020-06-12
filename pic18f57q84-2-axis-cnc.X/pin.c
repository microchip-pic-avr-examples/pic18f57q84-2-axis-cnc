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

#include "pin.h"
#include <xc.h>

/******************************************************/

#define SPACE_BETWEEN_GROUPS (&ANSELB - &ANSELA)

/******************************************************/

inline void SetBit(RegisterPtr reg, uint8_t mask) {
    *reg |= mask;
}

inline void ClearBit(RegisterPtr reg, uint8_t mask) {
    *reg &= ~mask;
}

void Pin_SetHigh(Pin pin) {
    SetBit(pin.lat, pin.mask);
}

void Pin_SetLow(Pin pin) {
    ClearBit(pin.lat, pin.mask);
}

void Pin_MakeOutput(Pin pin) {
    ClearBit(&TRISA + pin.group, pin.mask);
}

void Pin_MakeInput(Pin pin) {
    ClearBit(&ANSELA + (pin.group * SPACE_BETWEEN_GROUPS), pin.mask);
    SetBit(&TRISA + pin.group, pin.mask);
}

void Pin_SetPPS(Pin pin, uint8_t value){
    RegisterPtr RxyPPS = &RA0PPS + (pin.group * 8) + pin.position;
    *RxyPPS = value;
}

bool Pin_Get(Pin pin) {
    if (*pin.port & pin.mask) {
        return true;
    } else {
        return false;
    }
}

Pin Pin_Initialize(uint8_t group, uint8_t position) {
    uint8_t mask = 1 << position;
    RegisterPtr port = &PORTA + group;
    RegisterPtr lat = &LATA + group;
    Pin new_pin = {port, lat, group, mask, position};

    return new_pin;
}

/******************************************************/