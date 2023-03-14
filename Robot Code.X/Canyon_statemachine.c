#include "xc.h"
#include "MotionFunctions.h"
#pragma config FNOSC = FRCDIV // 8 MHz w post scaler
#define right_wall_detect _LATB14//Pin 17 RB14 Right wall/service detection 
#define center_wall_detect _LATB2//Pin 6 RB2 Center wall detection

//2-30 cm CCW lowers detection distance; CW increases distance 2-30 cm
//High= no obstacle 
//Low= obstacle detected within range

unsigned static int steps = 0;

unsigned static int PHASE_DETECTION=4;// global variable that counts phase strips on right side


void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void) 
{   
    steps = steps + 1;
    _OC2IF = 0; // clear flag
} 


enum {STRAIGHT,TURNING} state;


int main(void)
{
    Motion_Setup();
    Forward(); 
    state=STRAIGHT;
    while(1)
    {
        while (PHASE_DETECTION==4)
        {
            
            switch (state)
            {
                case STRAIGHT:
                    if(center_wall_detect==0)
                    {
                        STOP();
                        if(right_wall_detect==0)
                        {
                            steps=0;
                            Turn_Left();
                            state=L_TURN;
                        }
                        if (right_wall_detect==1)
                        {
                            steps=0;
                            Turn_Right();
                            state=R_TURN;
                        }
                    }
                  
                    break;
                    
                case TURNING:
                    if (steps==50)
                    {
                        steps=0;
                        Forward();
                        state=STRAIGHT;
                    }
                 
                    break;
                    
                
                        
            }
                        
                    
                    
                    
                    
                    
                    
                    
        }
            
                    
    }
    
        
           return 0; 
        
}
    
    
    
    
    
    
