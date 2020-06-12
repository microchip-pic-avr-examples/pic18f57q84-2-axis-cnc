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

#include "acceleration.h"
#include "config_macros.h"
#include "speed_tables.h"
#include "xc.h"

/******************************************************/

#define DMA_PROGRAM_FLASH 0b01

#define NCO_INC_SIZE STEPPER_SPEED_SIZE
#define TMR_PR_SIZE 1

#define TMR2_TRIGGER 0x1B

#define NCO_INC_LUT_LOC inc_amounts
#define TMR_LUT_LOC accel_times

#define NCO_INC_LOC &MASTER_NCO_INC
#define TMR_LOC &T2PR

/******************************************************/

uint16_t ramp_target;

/******************************************************/

// Called by overall initialize
void InitializeTimer() {
    T2CLKCON = 0b1100;
    T2HLTbits.MODE = 0b00000;
    T2CONbits.T2ON = 1;
}

inline void StartTimer(void) {
    T2CONbits.T2ON = 1;
}

inline void StopTimer(void) {
    T2CONbits.T2ON = 0;
}

inline void ResetTimer(void) {
    StopTimer();
    T2TMR = 0;
    StartTimer();
}

void SetTimerPeriod(uint8_t period) {
    T2PR = period;
}

void Acceleration_ClearISR() {
    NCO_DMA_IF = 0;
}

void StopRamp(void) {
    DMASELECT = NCO_DMA;
    DMAnCON0bits.SIRQEN = 0;
    DMAnCON0bits.EN = 0;
    DMASELECT = TMR_DMA;
    DMAnCON0bits.SIRQEN = 0;
    DMAnCON0bits.EN = 0;
}

void Acceleration_RampUp(uint16_t desired_lut_index) {
    ramp_target = desired_lut_index;
    ResetTimer();
    DMASELECT = NCO_DMA;
    DMAnCON0bits.EN = 0;
    DMAnDSA = NCO_INC_LOC; // Destination starts at location of NCO Increment Address
    DMAnSSA = NCO_INC_LUT_LOC; // Source starts at location of NCO lookup table
    DMAnCON1bits.DMODE = INCREMENT_POINTER; // Increment destination location each time
    DMAnCON1bits.SMODE = INCREMENT_POINTER; // Increment source location each time
    DMAnSSZ = desired_lut_index * NCO_INC_SIZE; // Copy bytes from source until desired location is reached

    DMAnCON0bits.SIRQEN = 1;
    DMAnCON0bits.EN = 1;
    DMAnCON0bits.DGO = 1;
    
    DMASELECT = TMR_DMA;
    DMAnCON0bits.EN = 0;
    DMAnDSA = TMR_LOC;
    DMAnSSA = TMR_LUT_LOC; // Source starts at location of NCO lookup table
    DMAnCON1bits.DMODE = INCREMENT_POINTER; // Increment destination location each time
    DMAnCON1bits.SMODE = INCREMENT_POINTER; // Increment source location each time
    DMAnSSZ = desired_lut_index * TMR_PR_SIZE; // Copy bytes from source until desired location is reached

    DMAnCON0bits.SIRQEN = 1;
    DMAnCON0bits.EN = 1;
    DMAnCON0bits.DGO = 1;
}

void Acceleration_RampDown(uint16_t current_lut_index) {
    ResetTimer();
    current_lut_index = current_lut_index - 1;
    
    DMASELECT = NCO_DMA;
    DMAnCON0bits.EN = 0;
    void * start_of_nco_inc = NCO_INC_LOC; // First byte in INC register
    void * end_of_nco_inc = start_of_nco_inc + NCO_INC_SIZE - 1; // Last byte location
    DMAnDSA = end_of_nco_inc; // Start copying to end of location (backwards)
    uint16_t source_size = current_lut_index * NCO_INC_SIZE; // Number of bytes to return to start
    DMAnSSZ = source_size; // Copy until it gets back to start
    void * start_of_lut = NCO_INC_LUT_LOC; // First byte in LUT
    void * location_to_copy = start_of_lut + source_size - 1; // Location to start reversing from
    DMAnSSA = location_to_copy;
    DMAnCON1bits.DMODE = DECREMENT_POINTER; // Decrement destination each time
    DMAnCON1bits.SMODE = DECREMENT_POINTER; // Decrement source each time

    DMAnCON0bits.SIRQEN = 1;
    DMAnCON0bits.EN = 1;
    DMAnCON0bits.DGO = 1;
    
    DMASELECT = TMR_DMA;
    DMAnCON0bits.EN = 0;
    void * start_of_tmr = TMR_LOC;
    void * end_of_tmr = start_of_tmr + TMR_PR_SIZE - 1;
    DMAnDSA = end_of_tmr; // Start copying to end of location (backwards)
    source_size = current_lut_index * TMR_PR_SIZE; // Number of bytes to return to start
    DMAnSSZ = source_size; // Copy until it gets back to start
    start_of_lut = TMR_LUT_LOC; // First byte in LUT
    location_to_copy = start_of_lut + source_size - 1; // Location to start reversing from
    DMAnSSA = location_to_copy;
    DMAnCON1bits.DMODE = DECREMENT_POINTER; // Decrement destination each time
    DMAnCON1bits.SMODE = DECREMENT_POINTER; // Decrement source each time

    DMAnCON0bits.SIRQEN = 1;
    DMAnCON0bits.EN = 1;
    DMAnCON0bits.DGO = 1;
}

void Acceleration_Reverse(void) {
    DMASELECT = NCO_DMA;
    DMAnCON0bits.SIRQEN = 0;
    DMASELECT = TMR_DMA;
    DMAnCON0bits.SIRQEN = 0;
    
    while (DMAnCON0bits.DGO); // Wait for in-process transaction to stop
    DMASELECT = NCO_DMA;
    while (DMAnCON0bits.DGO);

    uint16_t current_index = ramp_target - ((DMAnSCNT) / NCO_INC_SIZE);

    if (current_index == 1) {
        StopRamp();
    } else {
        Acceleration_RampDown(current_index);
    }


}

// Called by overall initialize
void InitializeAccelerationDma(void) {
    DMASELECT = NCO_DMA;

    DMAnDSZ = NCO_INC_SIZE;
    DMAnCON1bits.SSTP = 1;
    DMAnCON1bits.SMR = DMA_PROGRAM_FLASH;
    DMAnSIRQ = TMR2_TRIGGER;


    NCO_DMA_IE = 1;
    
    DMASELECT = TMR_DMA;

    DMAnDSZ = TMR_PR_SIZE;
    DMAnCON1bits.SSTP = 1;
    DMAnCON1bits.SMR = DMA_PROGRAM_FLASH;
    DMAnSIRQ = TMR2_TRIGGER;

}

void Acceleration_Initialize(void) {
    InitializeTimer();
    InitializeAccelerationDma();
}

/******************************************************/
