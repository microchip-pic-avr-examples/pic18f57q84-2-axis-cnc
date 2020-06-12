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

#ifndef COMMS_H
#define	COMMS_H

/******************************************************/

#include <stdint.h>
#include "types.h"

/******************************************************/

#define PACKET_SIZE 10 // Size of a comms packets

// Packet sent from the PC to the PIC
typedef struct comms_packet {
    uint8_t mode; // 1
    uint16_t distance; // 2
    uint8_t accel_target; // 1
    StepperSpeed x; // 3
    StepperSpeed y; // 3
} comms_packet;

/******************************************************/

enum MasterInstructions {NO_INSTRUCTION, INITIALIZE_CONNECTION, START_PROGRAM};

/******************************************************/

// Initialize DMAs that read from input to buffer, and from buffer to read location
void Comms_InitializeDMA(void);

// DMA will copy data from the RXB as it is received
void Comms_EnableDMA(void);

// DMA will not copy data from the RXB as it is received
void Comms_DisableDMA(void);

// Send the necessary metadata to the PC
void Comms_SendMetadata(void);

// Load the next value from the buffer to the read location
// Will automatically request buffer reload if empty
void Comms_LoadNext(void);

// Get the current value in the read location
comms_packet Comms_GetCurrent(void);

// Request that the PC completely fill the buffer (two transfers)
void Comms_Preload(void);

// Check for instructions from the PC
// Returns one of the instructions from MasterInstructions enum
uint8_t Comms_GetInstructions(void);

/******************************************************/

#endif

