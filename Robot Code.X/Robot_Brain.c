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
void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void)
{
    _INT0IF = 0; // clear flag
    task_counter++;
}
enum { STRAIGHT, ADJ_R, ADJ_L} state;

int main(void) {
    
    Motion_Setup();
    INT_Setup();
    Timer_Setup();
    Forward();
    //Turn_Left();
    
    state = STRAIGHT;
    
    while(1){
        
        switch(state)
        {
            case STRAIGHT:
                
                if (left_QRD_dig == 1 && right_QRD_dig != 1)
                {
                    //steps = 0;
                    //previous_state = middle;
                    //_LATA2 = 1;
                    Adj_Left();
                    state = ADJ_L;
                }
                
                else if (right_QRD_dig == 1 && left_QRD_dig != 1)
                {
                    //steps = 0;
                    //previous_state = middle;
                    //_LATA2 = 1;
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
                
                else if (task_QRD == 1 && task_counter < 1)
                {
                    _TON = 1; // start timer
                    task_counter = 0;
                    _LATA2 = 1;
                    //task_counter = Count_Tasks();
                    //state = PHASE_DETECTED;
                }
                
                else if (TMR1 >= 35000 && task_counter == 1)
                {
                    task_counter = 0;
                    _TON = 0;
                    TMR1 = 0;
                }
                
                else if (TMR1 >= 35000 && task_counter == 2)
                {
                    STOP();
                }
                
                break;
                
            case ADJ_L:
                //_LATA2 = 1;
                if (left_QRD_dig != 1 && right_QRD_dig != 1)
                {
                    //steps = 0;
                    //previous_state = left;
                    Forward();
                    state = STRAIGHT;
                }
                
                else if (right_QRD_dig == 1 && left_QRD_dig != 1)
                {
                    //steps = 0;
                    //previous_state = left;
                    Adj_Right();
                    state = ADJ_R;
                }
                
                else if (task_QRD == 1 && task_counter < 1)
                {
                    _TON = 1; // start timer
                    task_counter = 0;
                    _LATA2 = 1;
                    //task_counter = Count_Tasks();
                    //state = PHASE_DETECTED;
                }
                
                else if (TMR1 >= 35000 && task_counter == 1)
                {
                    task_counter = 0;
                    _TON = 0;
                    TMR1 = 0;
                }
                
                else if (TMR1 >= 35000 && task_counter >= 2)
                {
                    STOP();
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
                    //previous_state = right;
                    Forward();
                    state = STRAIGHT;
                }
                
                else if (left_QRD_dig == 1 && right_QRD_dig != 1)
                {
                    //steps = 0;
                    //previous_state = right;
                    Adj_Left();
                    state = ADJ_L;
                }
                
                else if (task_QRD == 1 && task_counter < 1)
                {
                    _TON = 1; // start timer
                    task_counter = 0;
                    _LATA2 = 1;
                    //task_counter = Count_Tasks();
                    //state = PHASE_DETECTED;
                }
                
                else if (TMR1 >= 35000 && task_counter == 2)
                {
                    STOP();
                }
                
                else if (TMR1 >= 46875 && task_counter == 1)
                {
                    task_counter = 0;
                    _TON = 0;
                    TMR1 = 0;
                }
                
//                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
//                {
//                    previous_state = right;
//                    STOP();
//                    Search4Line();
//                    state = STRAIGHT;
//                }
                
                break;
        }
    }
    return 0;
}

