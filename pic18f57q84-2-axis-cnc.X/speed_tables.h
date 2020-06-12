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

#ifndef SPEED_TABLES_H
#define	SPEED_TABLES_H

/******************************************************/

#include "types.h"

/******************************************************/

#define ACCEL_TABLE_SIZE 71

const uint8_t accel_times[] = {
	1,
	1,
	3,
	4,
	5,
	7,
	9,
	11,
	13,
	15,
	18,
	21,
	23,
	26,
	30,
	33,
	36,
	40,
	44,
	48,
	52,
	56,
	61,
	66,
	70,
	75,
	81,
	86,
	91,
	97,
	103,
	109,
	115,
	121,
	128,
	135,
	141,
	148,
	156,
	163,
	170,
	178,
	186,
	194,
	202,
	210,
	219,
	228,
	236,
	245,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255,
	255
};

const StepperSpeed inc_amounts[] = {
	{0xa, 0, 0},
	{0xb, 0, 0},
	{0xc, 0, 0},
	{0xd, 0, 0},
	{0xe, 0, 0},
	{0xf, 0, 0},
	{0x10, 0, 0},
	{0x11, 0, 0},
	{0x12, 0, 0},
	{0x13, 0, 0},
	{0x14, 0, 0},
	{0x15, 0, 0},
	{0x16, 0, 0},
	{0x17, 0, 0},
	{0x18, 0, 0},
	{0x19, 0, 0},
	{0x1a, 0, 0},
	{0x1b, 0, 0},
	{0x1c, 0, 0},
	{0x1d, 0, 0},
	{0x1e, 0, 0},
	{0x1f, 0, 0},
	{0x20, 0, 0},
	{0x21, 0, 0},
	{0x22, 0, 0},
	{0x23, 0, 0},
	{0x24, 0, 0},
	{0x25, 0, 0},
	{0x26, 0, 0},
	{0x27, 0, 0},
	{0x28, 0, 0},
	{0x29, 0, 0},
	{0x2a, 0, 0},
	{0x2b, 0, 0},
	{0x2c, 0, 0},
	{0x2d, 0, 0},
	{0x2e, 0, 0},
	{0x2f, 0, 0},
	{0x30, 0, 0},
	{0x31, 0, 0},
	{0x32, 0, 0},
	{0x33, 0, 0},
	{0x34, 0, 0},
	{0x35, 0, 0},
	{0x36, 0, 0},
	{0x37, 0, 0},
	{0x38, 0, 0},
	{0x39, 0, 0},
	{0x3a, 0, 0},
	{0x3b, 0, 0},
	{0x3c, 0, 0},
	{0x3d, 0, 0},
	{0x3e, 0, 0},
	{0x3f, 0, 0},
	{0x40, 0, 0},
	{0x41, 0, 0},
	{0x42, 0, 0},
	{0x43, 0, 0},
	{0x44, 0, 0},
	{0x45, 0, 0},
	{0x46, 0, 0},
	{0x47, 0, 0},
	{0x48, 0, 0},
	{0x49, 0, 0},
	{0x4a, 0, 0},
	{0x4b, 0, 0},
	{0x4c, 0, 0},
	{0x4d, 0, 0},
	{0x4e, 0, 0},
	{0x4f, 0, 0},
	{0x50, 0, 0}
};

/******************************************************/

#endif