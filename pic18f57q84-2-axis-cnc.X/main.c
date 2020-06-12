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

#include <xc.h>

/******************************************************/

#include "mcc_generated_files/mcc.h"
#include "config_macros.h"
#include "platform.h"
#include "counter.h"
#include "isr.h"
#include "master_clock.h"
#include "acceleration.h"
#include "comms.h"
#include "priority.h"

/******************************************************/

void InitializeDevice() {
    // Initialize the device
    SYSTEM_Initialize();
    __delay_ms(10);

    SlaveClock y_clock = SlaveClock_Initialize(NCO_Y_LOC, NCO_Y_PPS);
    Pin y_enable = Pin_Initialize(
            Y_ENABLE_PIN_PORT, Y_ENABLE_PIN_POS);
    StepPin y_step = StepPin_Initialize(
            Y_STEP_PIN_PORT, Y_STEP_PIN_POS, y_clock, Y_CLC);
    Pin y_direction = Pin_Initialize(
            Y_DIRECTION_PIN_PORT, Y_DIRECTION_PIN_POS);
    Pin y_limit = Pin_Initialize(
            Y_LIMIT_PIN_PORT, Y_LIMIT_PIN_POS);

    SlaveClock x_clock = SlaveClock_Initialize(NCO_X_LOC, NCO_X_PPS);
    Pin x_enable = Pin_Initialize(
            X_ENABLE_PIN_PORT, X_ENABLE_PIN_POS);
    StepPin x_step = StepPin_Initialize(
            X_STEP_PIN_PORT, X_STEP_PIN_POS, x_clock, X_CLC);
    Pin x_direction = Pin_Initialize(
            X_DIRECTION_PIN_PORT, X_DIRECTION_PIN_POS);
    Pin x_limit = Pin_Initialize(
            X_LIMIT_PIN_PORT, X_LIMIT_PIN_POS);
    
    Pin x_ms1 = Pin_Initialize(
            X_MS1_PIN_PORT, X_MS1_PIN_POS);
    Pin x_ms2 = Pin_Initialize(
            X_MS2_PIN_PORT, X_MS2_PIN_POS);
    Pin y_ms1 = Pin_Initialize(
            Y_MS1_PIN_PORT, Y_MS1_PIN_POS);
    Pin y_ms2 = Pin_Initialize(
            Y_MS2_PIN_PORT, Y_MS2_PIN_POS);
    
    Pin_MakeOutput(x_ms1);
    Pin_MakeOutput(x_ms2);
    Pin_MakeOutput(y_ms1);
    Pin_MakeOutput(y_ms2);
    
    Pin_SetLow(x_ms1);
    Pin_SetLow(x_ms2);
    Pin_SetLow(y_ms1);
    Pin_SetLow(y_ms2);
    
    Pin actuator = Pin_Initialize(
            ACTUATOR_PIN_PORT, ACTUATOR_PIN_POS);


    Stepper y_axis = Stepper_Initialize(Y_MAX, y_enable, y_step, y_direction, y_limit, Y_BACKWARDS, NCO_Y_CLC);
    Stepper x_axis = Stepper_Initialize(X_MAX, x_enable, x_step, x_direction, x_limit, X_BACKWARDS, NCO_X_CLC);

    MasterClock_Initialize();
    Priority_Initialize();
    Acceleration_Initialize();
    Comms_InitializeDMA();

    Platform_Initialize(x_axis, y_axis, actuator);
    
    __delay_ms(500);
    
    Interrupts_Enable();
}

/*
                         Main application
 */

void travel_command(comms_packet packet){
    uint24_t x_val = ((uint24_t)packet.x.upper << 16) + ((uint24_t)packet.x.high << 8) + ((uint24_t)packet.x.lower);
    uint24_t y_val = ((uint24_t)packet.y.upper << 16) + ((uint24_t)packet.y.high << 8) + (packet.y.lower);
    
    while(Platform_IsTravelling());
    
    Platform_SetDirection(packet.mode);
    Platform_SetIncrements(x_val, y_val);
    Platform_TravelRampedSpeed(packet.distance, packet.accel_target);
}

void run_program(void){
    Comms_EnableDMA();
    Comms_Preload();
    while(1){
        comms_packet packet = Comms_GetCurrent();
        U1TXB = packet.mode;
        if (!(packet.mode & 0xFC)){ // Standard movement
            travel_command(packet);
        } else if(packet.mode == 0xFF){ // End of sequence
            Comms_DisableDMA();
            return;
        } else if(packet.mode == 6){ // Home
            while(Platform_IsTravelling());
            Platform_Home();
        } else if (packet.mode == 7){
            while(Platform_IsTravelling());
            Platform_RaiseActuator();
            __delay_ms(500);
        } else if (packet.mode == 8){
            while(Platform_IsTravelling());
            Platform_LowerActuator();
            __delay_ms(500);
        }
        Comms_LoadNext();
    }
}

void main(void) {

    InitializeDevice();

    while (1) {
        uint8_t instruction = Comms_GetInstructions();

        switch (instruction) {
            case INITIALIZE_CONNECTION:
                Comms_SendMetadata();
                break;
            case START_PROGRAM:
                run_program();
                break;
            default:
                break;
        }
    }
}
