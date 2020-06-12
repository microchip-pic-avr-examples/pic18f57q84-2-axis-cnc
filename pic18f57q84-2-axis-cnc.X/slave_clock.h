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

#ifndef CLOCK_H
#define	CLOCK_H

/******************************************************/

#include <stdint.h>
#include "counter.h"
#include "types.h"

/******************************************************/

// Should not be created directly, factory should be used instead
typedef struct NCO {
    RegisterPtr src;
    RegisterPtr inc;
    RegisterPtr con;
} NCO;

// Should not be created directly, factory should be used instead
typedef struct SlaveClock {
    NCO nco;
    uint8_t pps;
} SlaveClock;

/******************************************************/

// Creates a slave clock for the NCO with address bank starting at *nco
// TODO: look into if PPS actually does anything
SlaveClock SlaveClock_Initialize(RegisterPtr nco, uint8_t nco_pps);

void NCO_Enable(NCO nco);
void NCO_Disable(NCO nco);

void NCO_SetSource(NCO nco, uint8_t source);

void NCO_SetInc(NCO nco, uint24_t value);

/******************************************************/

#endif