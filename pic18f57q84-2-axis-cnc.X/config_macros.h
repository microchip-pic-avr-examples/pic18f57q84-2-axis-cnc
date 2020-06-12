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

#ifndef CONFIG_M_H
#define	CONFIG_M_H

/******************************************************/

#include "config.h"
#include "hal.h"
#include "speed_tables.h"

/******************************************************/

#if MASTER_NCO == 1
#define NCO_MASTER_LOCATION NCO_1_LOC
#define MASTER_NCO_SOURCE NCO1_MASTER_SOURCE
#define MASTER_CLKR_SRC 0b0111

#elif MASTER_NCO == 2
#define NCO_MASTER_LOCATION NCO_2_LOC
#define MASTER_NCO_SOURCE NCO2_MASTER_SOURCE
#define MASTER_CLKR_SRC 0b1000

#elif MASTER_NCO == 3
#define MASTER_NCO_CON NCO3CON
#define MASTER_NCO_INC NCO3INC
#define MASTER_NCO_SOURCE NCO3_MASTER_SOURCE
#define MASTER_CLKR_SRC 0b1001

#endif

/******************************************************/

#if Y_NCO == 1
#define NCO_Y_LOC NCO_1_LOC
#define NCO_Y_PPS NCO_1_PPS
#define NCO_Y_CLC NCO_1_CLC

#elif Y_NCO == 2
#define NCO_Y_LOC NCO_2_LOC
#define NCO_Y_PPS NCO_2_PPS
#define NCO_Y_CLC NCO_2_CLC

#elif Y_NCO == 3
#define NCO_Y_LOC NCO_3_LOC
#define NCO_Y_PPS NCO_3_PPS
#define NCO_Y_CLC NCO_3_CLC

#endif

/******************************************************/

#if X_NCO == 1
#define NCO_X_LOC NCO_1_LOC
#define NCO_X_PPS NCO_1_PPS
#define NCO_X_CLC NCO_1_CLC

#elif X_NCO == 2
#define NCO_X_LOC NCO_2_LOC
#define NCO_X_PPS NCO_2_PPS
#define NCO_X_CLC NCO_2_CLC

#elif X_NCO == 3
#define NCO_X_LOC NCO_3_LOC
#define NCO_X_PPS NCO_3_PPS
#define NCO_X_CLC NCO_3_CLC

#endif

/******************************************************/

#define MASTER_SMT_PRU SMT1PRU
#define MASTER_SMT_PRH SMT1PRH
#define MASTER_SMT_PRL SMT1PRL
#define MASTER_SMT_CLK SMT1CLK
#define MASTER_SMT_CON0 SMT1CON0
#define MASTER_SMT_CON1 SMT1CON1
#define MASTER_SMT_STAT SMT1STAT
#define MASTER_SMT_TMRL SMT1TMRL
#define MASTER_SMT_TMRH SMT1TMRH

#define MASTER_SMT_IE PIE1bits.SMT1IE
#define MASTER_SMT_IF PIR1bits.SMT1IF

/******************************************************/

#define DECREMENT_POINTER 0b10
#define INCREMENT_POINTER 0b01

#define ACCEL_INFO_SIZE ACCEL_TABLE_SIZE * INC_SIZE + 1
#define DEVICE_INFO_SIZE BUFFER_METADATA_SIZE + CONVERSION_METADATA_SIZE + POSITION_SIZE * 2 + ACCEL_INFO_SIZE // Packets to send per TX + Size of NCO, ticks to mm conversion, x and ly limits

#define POSITION_SIZE 2
#define INC_SIZE 3
#define SPEED_SIZE 1

#define INPUT_SIZE 1 + INC_SIZE * 2 + POSITION_SIZE + SPEED_SIZE // MODE + x INC + Y INC + DISTANCE + SPEED

#define CONVERSION_METADATA_SIZE 4

#define BUFFER_METADATA_SIZE 1

#endif
