/*
 * File:   Line_Following.c
 * Author: noahf
 *
 * Created on March 4, 2023, 11:00 AM
 */


#include "xc.h"
#include "MotionFunctions.h"
#pragma config FNOSC = FRCDIV // 8 MHz w post scaler

enum { STRAIGHT, ADJ_R, ADJ_L } state;

int main(void) {
    
    _RCDIV = 0b010; // 2 MHz after 1/4 post scaler
    
    Motion_Setup();
    Analog_Setup();
    Forward();
    
    // set threshold for QRD to detect line
    int threshold = 400;
    
    state = STRAIGHT;
    
    while(1){
        
        switch(state)
        {
            case STRAIGHT:
                
                if (ADC1BUF0 > threshold)
                {
                    Adj_Left();
                    state = ADJ_L;
                }
                
                else if (ADC1BUF1 > threshold)
                {
                    Adj_Right();
                    state = ADJ_R;
                }
                
                break;
                
            case ADJ_L:
                
                if (ADC1BUF0 < threshold && ADC1BUF1 < threshold)
                {
                    Forward();
                    state = STRAIGHT;
                }
                
                else if (ADC1BUF0 > threshold)
                {
                    Adj_Right();
                    state = ADJ_R;
                }
                
                break;
                
            case ADJ_R:
                
                if (ADC1BUF0 < threshold && ADC1BUF1 < threshold)
                {
                    Forward();
                    state = STRAIGHT;
                }
                
                else if (ADC1BUF1 > threshold)
                {
                    Adj_Left();
                    state = ADJ_L;
                }
        }
    }
    return 0;
}
