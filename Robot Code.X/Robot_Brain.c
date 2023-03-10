/*
 * File:   Robot_Brain.c
 * Author: noahf
 *
 * Created on March 9, 2023, 12:04 PM
 */

#include "xc.h"
#include "MotionFunctions.h"
#pragma config FNOSC = FRCDIV // 8 MHz w post scaler

// PWM interrupt
void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void) 
{   
    steps = steps + 1;
    _OC2IF = 0; // clear flag
} 

// External Interrupt when pin 11 (task sensing qrd) goes high (senses black line)
void __attribute__((interrupt, no_auto_psv)) _INT0INterrupt(void)
{
    _INT0IF = 0; // clear flag
    task_counter++;
}
enum { STRAIGHT, ADJ_R, ADJ_L, PHASE_DETECTED } state;

int main(void) {
    
    Motion_Setup();
    INT_Setup();
    Forward();
    
    state = STRAIGHT;
    int task_counter = 0;
    
    while(1){
        
        switch(state)
        {
            case STRAIGHT:
                
                if (left_QRD_dig == 1 && right_QRD_dig != 1)
                {
                    //steps = 0;
                    previous_state = middle;
                    _LATB7 = 1;
                    Adj_Left();
                    state = ADJ_L;
                }
                
                else if (right_QRD_dig == 1 && left_QRD_dig != 1)
                {
                    //steps = 0;
                    previous_state = middle;
                    _LATB7 = 1;
                    Adj_Right();
                    state = ADJ_R;
                }
                
//                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
//                {
//                    previous_state = middle;
//                    STOP();
//                    Search4Line();
//                    state = STRAIGHT;
//                }
                
                else if (task_QRD == 1)
                {
                    task_counter = Count_Tasks();
                    state = PHASE_DETECTED;
                }
                
                break;
                
            case ADJ_L:
                
                if (left_QRD_dig != 1 && right_QRD_dig != 1)
                {
                    //steps = 0;
                    previous_state = left;
                    _LATB7 = 0;
                    Forward();
                    state = STRAIGHT;
                }
                
                else if (right_QRD_dig == 1 && left_QRD_dig != 1)
                {
                    //steps = 0;
                    previous_state = left;
                    _LATB7 = 1;
                    Adj_Right();
                    state = ADJ_R;
                }
                
                else if (task_QRD == 1)
                {
                    task_counter = Count_Tasks();
                    state = PHASE_DETECTED;
                }
                
//                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
//                {
//                    previous_state = left;
//                    STOP();
//                    Search4Line();
//                    state = STRAIGHT;
//                }
                
                break;
                
            case ADJ_R:
                
                if (right_QRD_dig != 1 && left_QRD_dig != 1)
                {
                    //steps = 0;
                    previous_state = right;
                    _LATB7 = 0;
                    Forward();
                    state = STRAIGHT;
                }
                
                else if (left_QRD_dig == 1 && right_QRD_dig != 1)
                {
                    //steps = 0;
                    previous_state = right;
                    _LATB7 = 1;
                    Adj_Left();
                    state = ADJ_L;
                }
                
                else if (task_QRD == 1)
                {
                    task_counter = Count_Tasks();
                    state = PHASE_DETECTED;
                }
                
//                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
//                {
//                    previous_state = right;
//                    STOP();
//                    Search4Line();
//                    state = STRAIGHT;
//                }
                
                break;
                
            case PHASE_DETECTED:
                
                if (task_counter == 2)
                {
                    Forward();
                    state = STRAIGHT;
                }
                
                else if (task_counter == 3)
                {
                    STOP();
                }
                
                break;
        }
    }
    return 0;
}

