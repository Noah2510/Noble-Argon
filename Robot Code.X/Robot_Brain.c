/*
 * File:   Robot_Brain.c
 * Author: noahf
 *
 * Created on March 9, 2023, 12:04 PM
 */

// Testing for blake's github
#include "xc.h"
#include "MotionFunctions.h"
#pragma config FNOSC = FRCDIV // 8 MHz w post scaler

void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void) 
{   
    steps = steps + 1;
    _OC2IF = 0; // clear flag
} 

unsigned int task_counter = 0;
void _ISR _CNInterrupt(void)
{
    _CN1F = 0; // clear interrupt flag
    task_counter++;
}

enum { STRAIGHT, ADJ_R, ADJ_L, OFFLINE } state;

int main(void) {
    
    //_RCDIV = 0b010; // 2 MHz after 1/4 post scaler
    
    Motion_Setup();
    //Analog_Setup();
    CN_Setup();
    Forward();
    
    // set threshold for QRD to detect line (Analog only))
    //int threshold = 1860; // 1.5V/3.3 * 4095
    
    state = STRAIGHT;
    
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
                
                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
                {
                    previous_state = middle;
                    STOP();
                    Search4Line();
                    state = STRAIGHT;
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
                
                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
                {
                    previous_state = left;
                    STOP();
                    Search4Line();
                    state = STRAIGHT;
                }
                
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
                
                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
                {
                    previous_state = right;
                    STOP();
                    Search4Line();
                    state = STRAIGHT;
                }
                
                break;
        }
    }
    return 0;
}

