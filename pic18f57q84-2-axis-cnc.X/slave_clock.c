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

#include "slave_clock.h"
#include "config_macros.h"

/******************************************************/

SlaveClock SlaveClock_Initialize(RegisterPtr nco, uint8_t nco_pps){
    RegisterPtr inc = nco + NCO_INC_OFFSET;
    RegisterPtr con = nco + NCO_CON_OFFSET;
    RegisterPtr clk = nco + NCO_CLK_OFFSET;
    
    NCO osc = {clk, inc, con};
    
    SlaveClock slave_clock = {osc, nco_pps};
    
    return slave_clock;
}

void NCO_Enable(NCO nco){
    *nco.con |= 0x80;
}

void NCO_Disable(NCO nco){
    *nco.con &= 0x7F;
}

void NCO_SetSource(NCO nco, uint8_t source){
    *nco.src = source;
}

void NCO_SetInc(NCO nco, uint24_t value){
    *(nco.inc + 2) = (value >> 16) & 0xFF;
    *(nco.inc + 1) = (value >> 8) & 0xFF;
    *nco.inc = value & 0xFF;
}

/******************************************************/
