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

#ifndef HAL_H
#define	HAL_H

#include "xc.h"

#define PIR_LOC &PIR0
#define PIE_LOC &PIE0

#define NCO_1_LOC &NCO1CLK
#define NCO_2_LOC &NCO2CLK
#define NCO_3_LOC &NCO3CLK

#define NCO_1_PPS 0x3F
#define NCO_2_PPS NCO_1_PPS + 1
#define NCO_3_PPS NCO_1_PPS + 2

#define NCO_1_CLC 0b00101010
#define NCO_2_CLC 0b00101011
#define NCO_3_CLC 0b00101100

#define NCO1_MASTER_SOURCE 0b10000
#define NCO2_MASTER_SOURCE 0b10001
#define NCO3_MASTER_SOURCE 0b10010

#define NCO1_TU_SOURCE 0b10111
#define NCO2_TU_SOURCE 0b10110
#define NCO3_TU_SOURCE 0b10101

#define NCO1_addr &NCO1CLK
#define NCO_INC_OFFSET (&NCO1INCL - NCO1_addr)
#define NCO_CON_OFFSET (&NCO1CON - NCO1_addr)
#define NCO_CLK_OFFSET (&NCO1CLK - NCO1_addr)

#define CLC_1_OUT 0x01

#define UTMR_1_LOC &TU16ACON0
#define UTMR_1_PIx 0
#define UTMR_1_INT TU16A

#define UTMR_2_LOC &TU16BCON0
#define UTMR_2_PIx 5
#define UTMR_2_INT TU16A

#define UTMR_CON0_OFFSET 0
#define UTMR_CON1_OFFSET (&TU16ACON1 - UTMR_1_LOC)
#define UTMR_CR_OFFSET (&TU16ACRL - UTMR_1_LOC)
#define UTMR_PR_OFFSET (&TU16APRL - UTMR_1_LOC)
#define UTMR_CLK_OFFSET (&TU16ACLK - UTMR_1_LOC)
#define UTMR_HLT_OFFSET (&TU16AHLT - UTMR_1_LOC)
#define UTMR_INT_POS 4

#define UTMR_PRL_WRITE_RESET 0b111111

#endif