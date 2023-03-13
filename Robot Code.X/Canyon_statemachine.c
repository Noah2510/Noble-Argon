#include "xc.h"
#include "MotionFunctions.h"
#pragma config FNOSC = FRCDIV // 8 MHz w post scaler


//2-30 cm CCW lowers detection distance; CW increases distance 2-30 cm
//High= no obstacle 
//Low= obstacle detected within range

#define right_wall_detect _LATB14//Pin 17 RB14 Right wall/service detection 
#define center_wall_detect _LATB2//Pin 6 RB2 Center wall detection
unsigned static int PHASE_DETECTION=4;// global variable that counts phase strips on right side



enum {STRAIGHT,TURN} state;


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
                        state=TURN;
                    }
                    break;
                case TURN:
                    if(right_wall_detect==0)
                    {
                        Turn_Left();
                        Forward();
                        state=STRAIGHT;
                    }
                    else if (right_wall_detect==1)
                    {
                        Turn_Right();
                        Forward();
                        state=STRAIGHT;
                    }
                    
               
                        
            }
                        
                    
                    
                    
                    
                    
                    
                    
        }
            
                    
    }
    
        
           return 0; 
        
}
    
    
    
    
    

\
    
