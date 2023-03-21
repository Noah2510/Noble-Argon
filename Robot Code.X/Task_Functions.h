// These are functions for completing different tasks

#include "MotionFunctions.h"

void Ball_Collection(void) {
    
    enum {TURN_L, BACK_UP, RETRIEVE, FORWARD, BACK2LINE} state;
    int x = 1;
    
    while(x)
    {
        Turn_Left();
        state = TURN_L;
        
        switch(state)
        {
            
            case TURN_L:
                
                if(steps >= 1005)
                {
                    Backward();
                    steps = 0;
                    state = BACK_UP;
                }
                
                break;
                
            case BACK_UP:
                
                if(steps > 2000)
                {
                    STOP();
                    steps = 0;
                    TMR1 = 0;
                    T1CONbits.TON = 1;
                    state = RETRIEVE;
                }
                
                break;
                
            case RETRIEVE:
                
                if(TMR1 >= 2000)
                {
                    Forward();
                    TMR1 = 0;
                    T1CONbits.TON = 0;
                    state = FORWARD;
                }
                
                break;
                
            case FORWARD:
                
                if(left_QRD_dig == 1 && right_QRD_dig == 1)
                {
                    Adj_Right();
                    state = BACK2LINE;
                }
                
                break;
                
            case BACK2LINE:
                
                if (left_QRD_dig !=1 && right_QRD_dig == 1)
                {
                    x = 0;
                }
                
                break;
        }
    }
}
