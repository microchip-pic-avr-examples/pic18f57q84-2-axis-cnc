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

#include "comms.h"
#include "config_macros.h"
#include "mcc_generated_files/uart1.h"

/******************************************************/

#define RX_LOCATION U1RXB
#define MESSAGE_SIZE BUFFER_NUMBER_PACKETS * PACKET_SIZE
#define RX_SIRQ 0x20

#define INITIAL_CONNECTION_BYTE 0x01
#define START_PROGRAM_BYTE 0x02

#define REQUEST_REFILL 0x55

/******************************************************/

volatile comms_packet buffer[MESSAGE_SIZE * 2];
volatile comms_packet current;
volatile uint8_t remaining = 0;

/******************************************************/

inline void SendByte(uint8_t byte) {
    UART1_Write(byte);
}

void SendMessage(uint8_t * message, uint8_t length) {
    uint8_t i;
    for (i = 0; i < length; i++) {
        uint8_t output = *message++;
        SendByte(output);
    }
}

// Add data at location *data of size length starting at *start_position
void StuffMessage(uint8_t * start_position, void * data, size_t length) {
    size_t i = 0;
    uint8_t * write_ptr = start_position;
    uint8_t * message_ptr = data + length - 1; // TODO figure out if I need to start from the end
    for (i = 0; i < length; i++) {
        uint8_t byte = *message_ptr--;
        *write_ptr++ = byte;
    }
}

void Comms_SendMetadata() {
    // Size of NCO, ticks to mm conversion, x and ly 
    uint8_t device_info[DEVICE_INFO_SIZE];
    uint8_t * write_ptr = device_info;

    // Stuff metadata packet
    
    // MIN_SIZE data_to_write = CONSTANT_BEING_WRITTEN;
    // StuffMessage(write_ptr, &data_to_write, DATA_SIZE_CONSTANT);
    // write_ptr += DATA_SIZE_CONSTANT;
    
    uint8_t buffer_size = BUFFER_NUMBER_PACKETS;
    StuffMessage(write_ptr, &buffer_size, BUFFER_METADATA_SIZE);
    write_ptr += BUFFER_METADATA_SIZE;

    uint32_t ticks_per_m = TICKS_PER_METER;
    StuffMessage(write_ptr, &ticks_per_m, CONVERSION_METADATA_SIZE);
    write_ptr += CONVERSION_METADATA_SIZE;

    uint16_t max_x_metadata = X_MAX;
    StuffMessage(write_ptr, &max_x_metadata, POSITION_SIZE);
    write_ptr += POSITION_SIZE;

    uint16_t max_y_metadata = Y_MAX;
    StuffMessage(write_ptr, &max_y_metadata, POSITION_SIZE);
    write_ptr += POSITION_SIZE;
    
    uint8_t num_accel_points = ACCEL_TABLE_SIZE;
    StuffMessage(write_ptr, &num_accel_points, 1);
    write_ptr += 1;
    
    StuffMessage(write_ptr, inc_amounts, ACCEL_TABLE_SIZE * INC_SIZE);

    SendMessage(device_info, DEVICE_INFO_SIZE);
}

void Comms_InitializeDMA(void) {
    DMASELECT = UART_DMA;

    DMAnDSZ = MESSAGE_SIZE * 2; // Buffer is twice the size of updated region
    DMAnSSZ = 1; // UARTRX is 1 byte
    DMAnSSA = &RX_LOCATION; // Location of RX register
    DMAnDSA = buffer; // Copy to buffer
    DMAnCON1bits.DMODE = INCREMENT_POINTER;
    DMAnCON1bits.SMODE = INCREMENT_POINTER;
    DMAnSIRQ = RX_SIRQ; // Triggered by RX
    DMAnCON0bits.EN = 1;

    DMASELECT = CURRENT_PACKET_DMA;
    
    DMAnDSZ = PACKET_SIZE; // Reading one packet at a time
    DMAnSSZ = MESSAGE_SIZE * 2; // Buffer is twice the size of updated region
    DMAnDSA = &current; // Current instruction buffer location
    DMAnSSA = buffer; // Copy from larger buffer
    DMAnCON1bits.DMODE = INCREMENT_POINTER;
    DMAnCON1bits.SMODE = INCREMENT_POINTER;
    DMAnCON1bits.DSTP = 1;
    DMAnCON0bits.EN = 1;
}

// Request additional packets and reset end of buffer counter
void RefillBuffer(void) {
    SendByte(REQUEST_REFILL);
    remaining = BUFFER_NUMBER_PACKETS;
}

void Comms_LoadNext(void) {
    DMASELECT = CURRENT_PACKET_DMA;

    DMAnCON0bits.DGO = 1;

    remaining--;

    // Refill buffer is empty
    if (!remaining) {
        RefillBuffer();
    }
}

comms_packet Comms_GetCurrent(void){
    DMASELECT = CURRENT_PACKET_DMA; 
    while(DMAnCON0bits.DGO); // Make sure it's not still copying
    
    return current;
}

void Comms_EnableDMA(void) {
    DMASELECT = CURRENT_PACKET_DMA;
    DMAnCON0bits.EN = 1;
    
    DMASELECT = UART_DMA;

    DMAnCON0bits.EN = 1;
    DMAnCON0bits.SIRQEN = 1;
}

void Comms_DisableDMA(void) {
    DMASELECT = UART_DMA;

    DMAnCON0bits.SIRQEN = 0;
    DMAnCON0bits.EN = 0;
    
    DMASELECT = CURRENT_PACKET_DMA;
    DMAnCON0bits.EN = 0;
}

void Comms_Preload(void){
    SendByte(REQUEST_REFILL);
    remaining = BUFFER_NUMBER_PACKETS;
    DMASELECT = UART_DMA;
    while(DMAnDCNT == DMAnDSZ);
    while(DMAnDCNT != DMAnDSZ);
    Comms_LoadNext();
}

uint8_t Comms_GetInstructions(){
    if(!U1FIFObits.RXBE){
        uint8_t instruction = U1RXB;
        switch(instruction) {
            case INITIAL_CONNECTION_BYTE:
                return INITIALIZE_CONNECTION;
            case START_PROGRAM_BYTE:
                return START_PROGRAM;
            default:
                return NO_INSTRUCTION;
        }
    }
    
    return NO_INSTRUCTION;
}

/******************************************************/
