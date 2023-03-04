/*
 * File:   Motion_Milestone.c
 * Author: noah2510
 *
 * Created on February 27, 2023, 5:43 PM
 */

// Testing for github 
#include "xc.h"
#include "MotionFunctions.h"
#pragma config FNOSC = FRCDIV // 8 MHz oscillator with post scaler option

unsigned int steps = 0;

void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void) 
{   
    _OC2IF = 0; // clear flag
    
    steps++; // Only counting steps on the left motor. 
} 

enum { FORWARD, TURNR } state;

int main(void) {
    
    // set post scaler
    _RCDIV = 0b010; // 2 MHz after 1/4 post scaler
    
    Motion_Setup();
    
    state = FORWARD;
    
    //Switch statement of motion inside loop 
    while(1){
        
        switch(state)
        {
            case FORWARD:
                
                if (steps >= 2600 && steps < 3400)
                {
                    Turn_Right();
                    state = TURNR;
                }
                else if (steps >= 6000 && steps < 7600)
                {
                    Turn_Right();
                    state = TURNR;
                }
                
                else if (steps > 10200)
                {
                    steps = 0;
                }
                
                break;
                
            case TURNR:
                
                if (steps >= 3400 && steps < 6000)
                {
                    Forward();
                    state = FORWARD;
                }
                
                else if (steps >= 7600 && steps < 10200)
                {
                    Forward();
                    state = FORWARD;
                }
                
                break;
        }
    }
        
    return 0;
}

