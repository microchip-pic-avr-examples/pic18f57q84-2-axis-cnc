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

#include "counter.h"
#include "config_macros.h"

/******************************************************/

#define CAPTURE_BIT 0x80
#define ENABLE_BIT 0x80
#define RESET_BIT 0x10

#define FOSC_CLK 0b0000
#define CLKR_CLK 0b1000

/******************************************************/

inline void Counter_SetPeriod(uint16_t value){
    // Modify MSB first to ensure it doesn't lower overall value midway through
    // Assumption is new period value will be higher than current timer value
    
    MASTER_SMT_PRH = value >> 8;
    MASTER_SMT_PRL = value & 0xFF;
    MASTER_SMT_PRU = 0;
}

inline void Enable(){
    MASTER_SMT_CON1 |= ENABLE_BIT;
}

inline void Disable(){
    MASTER_SMT_CON1 &= ~ENABLE_BIT;
}

// SMT requires a few input clock cycles to get started up fully
void Prime(){
    MASTER_SMT_CLK = FOSC_CLK;
    NOP();
    NOP();
    MASTER_SMT_CLK = CLKR_CLK;
}

inline void Counter_Reset(){
    SMT1CON1bits.GO = 0;
    SMT1TMR = 0;
    SMT1CON1bits.GO = 1;
}

void Counter_ClearISR(){
    MASTER_SMT_IF = 0;
}

void Counter_Initialize(){
    // CLK Reference is used since NCO can't directly clock SMT
    CLKRCONbits.EN = 1;
    CLKRCLK = MASTER_CLKR_SRC;
    
    MASTER_SMT_CON0 = 0b10100000;
    MASTER_SMT_CON1 = 0b01000000;
    
    MASTER_SMT_IE = 1;

    Enable();
    Counter_Reset();
    
    Prime();
}

// Note: this reads the raw value of the timer
// This should only be executed if you are sure the value will
// not be updated midway through execution
// This is generally bad practice, but is necessary for immediate access
uint16_t Counter_GetValue(){
    uint16_t result = (MASTER_SMT_TMRH << 8) + MASTER_SMT_TMRL;
    
    return result;
}

void Counter_Start(uint16_t pr){
    Counter_Reset();
    Counter_SetPeriod(pr);
}

/******************************************************/
