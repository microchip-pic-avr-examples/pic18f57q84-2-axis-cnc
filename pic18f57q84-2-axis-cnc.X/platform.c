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

#include "platform.h"
#include "config_macros.h"
#include "master_clock.h"
#include "acceleration.h"

/******************************************************/

void null_cb(){
}

/******************************************************/

CbFuncPtr counter_cb = &null_cb;
CbFuncPtr acceleration_cb = &null_cb;

/******************************************************/

Stepper y_axis;
Stepper x_axis;

Pin actuator;

/******************************************************/

uint16_t distance_to_travel = 0;
uint16_t target_speed = 0;

volatile bool active = false;

/******************************************************/

bool Platform_IsTravelling(void){
    return active;
}

void Platform_Initialize(Stepper x_stepper, Stepper y_stepper, Pin actuator_pin) {
    Pin_MakeOutput(actuator_pin);
    x_axis = x_stepper;
    y_axis = y_stepper;
    actuator = actuator_pin;
    Stepper_SetNCO(y_axis, MASTER_NCO_SOURCE);
    Stepper_SetNCO(x_axis, MASTER_NCO_SOURCE);
}

StepperError Platform_Home() {
    
    StepperError error;

    error = Stepper_Home(&x_axis);
    if (error != NO_ERROR) {
        return error;
    }

    error = Stepper_Home(&y_axis);
    if (error != NO_ERROR) {
        return error;
    }
    
    return error;
}

void Platform_SetDirection(uint8_t dir_info){
    if(dir_info & 0b1){
        Stepper_SetForwards(x_axis);
    } else {
        Stepper_SetBackwards(x_axis);
    }
    
    if(dir_info & 0b10){
        Stepper_SetForwards(y_axis);
    } else {
        Stepper_SetBackwards(y_axis);
    }
}

void Platform_SetIncrements(uint24_t x, uint24_t y){
    Stepper_SetInc(x_axis, x);
    Stepper_SetInc(y_axis, y);
}

void Platform_DistanceISR(){
    (*counter_cb)();
    Counter_ClearISR();
}

void Platform_AccelerationISR(){
    (*acceleration_cb)();
    Acceleration_ClearISR();
}

// Interrupt because travel is complete
void CounterEndIsr(){
    MasterClock_Disable();
    active = false;
}

// Interrupt because midpoint was reached
void CounterHalfwayIsr(){
    Counter_SetPeriod(distance_to_travel);
    counter_cb = &CounterEndIsr;
    acceleration_cb = &null_cb;
    
    Acceleration_Reverse();
}

// Interrupt because it needs to start slowing down
void CounterSlowIsr(){
    Counter_SetPeriod(distance_to_travel);
    Acceleration_RampDown(target_speed);
    counter_cb = &CounterEndIsr;
}

// Interrupt because full speed reached
void RampFullIsr(){
    uint16_t time_to_accelerate = Counter_GetValue();
    uint16_t when_to_brake = distance_to_travel - time_to_accelerate;
    
    Counter_SetPeriod(when_to_brake);
    
    counter_cb = &CounterSlowIsr;
    acceleration_cb = &null_cb;
}

void Platform_TravelFixedSpeed(uint16_t distance, uint32_t inc){
    active = true;
    counter_cb = &CounterEndIsr;
    Counter_Start(distance);
    MasterClock_SetInc(inc);
    MasterClock_Enable();
}

void Platform_TravelRampedSpeed(uint16_t distance, uint16_t max){
    active = true;
    distance_to_travel = distance;
    target_speed = max;
    uint16_t halfway_point = distance / 2;
    counter_cb = &CounterHalfwayIsr;
    acceleration_cb = &RampFullIsr;
    Counter_Start(halfway_point);
    Acceleration_RampUp(max);
    MasterClock_Enable();
}


void Platform_RaiseActuator(void){
    Pin_SetLow(actuator);
}

void Platform_LowerActuator(void){
    Pin_SetHigh(actuator);
}

/******************************************************/
