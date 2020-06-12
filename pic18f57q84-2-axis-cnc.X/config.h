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

#ifndef CONFIG_H
#define	CONFIG_H

/******************************************************/

#include "hal.h"

/******************************************************/
// Platform Configuration

#define TICKS_PER_METER 12540

// Device maximum configurations
#define Y_MAX 2884
#define X_MAX 6020

// Reverse direction of axes
#define Y_BACKWARDS true
#define X_BACKWARDS false

// How many instructions to store in buffer
#define BUFFER_NUMBER_PACKETS 5

/******************************************************/
// Pinout

// INT1
#define Y_ENABLE_PIN_PORT RA
#define Y_ENABLE_PIN_POS 6

// PWM1
#define Y_STEP_PIN_PORT RB
#define Y_STEP_PIN_POS 4

// CS1
#define Y_DIRECTION_PIN_PORT RD
#define Y_DIRECTION_PIN_POS 4

// AN1
#define Y_MS1_PIN_PORT RA
#define Y_MS1_PIN_POS 0

// RST1
#define Y_MS2_PIN_PORT RA
#define Y_MS2_PIN_POS 7

#define Y_LIMIT_PIN_PORT RA
#define Y_LIMIT_PIN_POS 1

// INT3
#define X_ENABLE_PIN_PORT RA
#define X_ENABLE_PIN_POS 4

// PWM3
#define X_STEP_PIN_PORT RA
#define X_STEP_PIN_POS 5

// CS3
#define X_DIRECTION_PIN_PORT RD
#define X_DIRECTION_PIN_POS 7

// AN3
#define X_MS1_PIN_PORT RA
#define X_MS1_PIN_POS 2

// RST3
#define X_MS2_PIN_PORT RD
#define X_MS2_PIN_POS 5

#define X_LIMIT_PIN_PORT RC
#define X_LIMIT_PIN_POS 7

#define ACTUATOR_PIN_PORT RF
#define ACTUATOR_PIN_POS 2

/******************************************************/
// CIP Selection

#define MASTER_NCO 3
#define X_NCO 2
#define Y_NCO 1

#define X_CLC 0
#define Y_CLC 2

#define MASTER_TIMER TIMER1

/******************************************************/
// DMA assignment and priority
// DMA number is 1 less than actual DMA
// E.g. DMA 1 has value 0 but priority DMA1PR

#define NCO_DMA 0
#define NCO_DMA_PRIORITY DMA1PR
#define NCO_DMA_IF PIR2bits.DMA1SCNTIF
#define NCO_DMA_IE PIE2bits.DMA1SCNTIE

#define TMR_DMA 1
#define TMR_DMA_PRIORITY DMA2PR

#define UART_DMA 2
#define UART_DMA_PRIORITY DMA3PR

#define CURRENT_PACKET_DMA 3
#define CURRENT_PACKET_DMA_PRIORITY DMA4PR

/******************************************************/

#endif
