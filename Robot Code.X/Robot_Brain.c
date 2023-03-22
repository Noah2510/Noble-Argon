/*
 * File:   Robot_Brain.c
 * Author: noahf
 *
 * Created on March 9, 2023, 12:04 PM
 */

#include "xc.h"
#include "Motion_FunctionsN.h"
#include "Task_FunctionsN.h"
#pragma config FNOSC = FRCDIV // 8 MHz w post scaler
#pragma config ICS = PGx3

// PWM interrupt
void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void) 
{   
    steps = steps + 1;
    _OC2IF = 0; // clear flag
} 

void _ISR _CNInterrupt(void) {
    
    _CNIF = 0; // clear flag
//    if (task_QRD == 1)
//    {
//    _LATA2 = 1;
//    task_counter++;
//    }
    task_counter++;
    _LATA2 = 1;
    if (task_counter == 1){
        //_LATA2 = 1;
        TMR1 = 0;
        T1CONbits.TON = 1; // turn on timer for line counting
    }
    
    _LATA2 = 0;
}

// External Interrupt when pin 11 (task sensing qrd) goes high (senses black line)
//void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void)
//{
//    _INT0IF = 0; // clear flag
////    if (task_QRD == 1)
////    {
////    _LATA2 = 1;
////    task_counter++;
////    }
//    task_counter++;
//    _LATA2 = 1;
//    if (task_counter == 1){
//        //_LATA2 = 1;
//        TMR1 = 0;
//        T1CONbits.TON = 1; // turn on timer for line counting
//    }
//    
//    _LATA2 = 0;
//}

enum { STRAIGHT, ADJ_R, ADJ_L, TASK_COUNT, STOPPED} state;

int main(void) {
    
    Motion_Setup();
    CN_Setup();
    //INT_Setup();
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
                
                else if (TMR1 >= 20000)
                {
                    //_LATA2 = 1;
                    T1CONbits.TON = 0;
                    TMR1 = 0;
                    //STOP();
                    state = TASK_COUNT;
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
                
                else if (TMR1 >= 20000)
                {
                    //_LATA2 = 1;
                    T1CONbits.TON = 0;
                    TMR1 = 0;
                    //STOP();
                    state = TASK_COUNT;
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
                
                else if (TMR1 >= 20000)
                {
                    //_LATA2 = 1;
                    T1CONbits.TON = 0;
                    TMR1 = 0;
                    //STOP();
                    state = TASK_COUNT;
                }
                
//                else if (right_QRD_dig == 0 && left_QRD_dig == 0 && mid_QRD_dig == 0)
//                {
//                    previous_state = right;
//                    STOP();
//                    Search4Line();
//                    state = STRAIGHT;
//                }
                
                break;
                
            case TASK_COUNT:
                
                if (task_counter >= 100 && task_counter < 200)
                {
                    
                    _LATA2 = 1;
                    STOP();
                    task_counter = 0;
//                    T1CONbits.TON = 0;
//                    TMR1 = 0;
                    state = STOPPED;
                }
                
//                else if(task_counter == 2)
//                {
//                    //_LATA2 = 1;
//                    task_counter = 0;
//                    T1CONbits.TON = 0;
//                    TMR1 = 0;
//                    Ball_Collection();
//                    state = STRAIGHT;
//                }
                
                else if (task_counter >= 200 && task_counter <= 300)
                {
                    _LATA2 = 1;
                    STOP();
                    task_counter = 0;
//                    T1CONbits.TON = 0;
//                    TMR1 = 0;
                    state = STOPPED;
                }
                
//                else if (task_counter > 3)
//                {
//                    task_counter = 0;
//                    T1CONbits.TON = 0;
//                    TMR1 = 0;
//                    state = STRAIGHT;
//                    //STOP();
//                }
                
                break;
                    
            case STOPPED:
                
                break;
        }
    }
    return 0;
}

