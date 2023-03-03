/*
 * File:   Motion_Milestone.c
 * Author: noah2510
 *
 * Created on February 27, 2023, 5:43 PM
 */

// Testing for github 
#include "xc.h"
#include "MotionFuncions.h" // why doesn't this work?
#pragma config FNOSC = FRCDIV // 8 MHz oscillator with post scaler option

int main(void) {
    
    // set post scaler
    _RCDIV = 0b010; // 2 MHz after 1/4 post scaler
    
    Motion_Setup();
    
    while(1){
        Forward(1000);
        Turn_90(right);
        Forward(1000);
        Turn_180();
        Forward(1000);
        break; // or we could have the loop repeat
    }
        
    return 0;
}

