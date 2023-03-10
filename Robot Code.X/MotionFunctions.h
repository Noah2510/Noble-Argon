/* 
 * File:   
 * Author: noahf
 * Comments:
 * Revision history: 
 */

//#ifndef XC_HEADER_TEMPLATE_H
//#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  

// Some of these are just here in case we want to use later
#define pi 3.14159265
#define right_dir_pin _LATB13
#define left_dir_pin _LATB9
#define left_motor_T OC3RS 
#define left_motor_dc OC3R
#define right_motor_T OC2RS
#define right_motor_dc OC2R
#define right 1
#define left 0
#define middle 3
#define med_pwm 5999
#define fast_pwm 749
#define slow_pwm 2999
#define right_QRD ADC1BUF0
#define left_QRD ADC1BUF1
#define right_QRD_dig _RA0
#define left_QRD_dig _RA1
#define mid_QRD_dig _RB15
#define task_QRD _RB7

unsigned int steps = 0;
int previous_state = 0;
int task_counter = 0;

void Motion_Setup(void) {
    
    // Configure direction pins as digital outputs
    _ANSB13 = 0; // right servo
    _TRISB13 = 0;
    
    _TRISB9 = 0; // left servo
    
    _TRISB7 = 1; // set task detection pin as DI
    
    right_dir_pin = 1; // start both servos in the same direction
    left_dir_pin = 1; 
    
    _TRISA0 = 1; // Set up QRD pins for digital input
    _TRISA1 = 1;
    _TRISB15 = 1;
    _ANSB15 = 0;
    _ANSA0 = 0;
    _ANSA1 = 0;
    
    // Configure PWM pins 
    //right motor
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON1bits.OCM = 0b110;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2CON2bits.OCTRIG = 0;
    OC2RS = fast_pwm; 
    OC2R = 0; 
    
    //Left motor
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 0b111;
    OC3CON1bits.OCM = 0b110;
    OC3CON2bits.SYNCSEL = 0b11111;
    OC3CON2bits.OCTRIG = 0;
    OC3RS = fast_pwm; 
    OC3R = 0; 
    
    // Configure OC2 interrupt AFTER pwm is configured
    _OC2IP = 4; // interrupt priority
    _OC2IE = 1; // Enable interrupt
    _OC2IF = 0; // clear flag
}

void Analog_Setup(void) {
    _ADON = 0;    // Disable A/D module during configuration
    
    // AD1CON1
    _MODE12 = 1;  // 12-bit resolution
    _FORM = 0;    // unsigned integer output
    _SSRC = 7;    // auto convert
    _ASAM = 1;    // auto sample

    // AD1CON2
    _PVCFG = 0;   // use VDD as positive reference
    _NVCFG = 0;   // use VSS as negative reference
    _BUFREGEN = 1;// store results in buffer corresponding to channel number
    _CSCNA = 1;   // scanning mode
    _SMPI = 1;    // begin new sampling sequence after every sample
    _ALTS = 0;    // sample MUXA only

    // AD1CON3
    _ADRC = 0;    // use system clock
    _SAMC = 1;    // sample every A/D period
    _ADCS = 0x3F; // TAD = 64*TCY

    //AD1CSSL = 1; 
    _CSS0 = 1;
    _CSS1 = 1;
//    _CH0NA = 0;  // Vref is ground
//    _CH0SA = 0;  // AN0
//    _CH0SB = 1;  // AN1
//    _CH0NB = 0; // Vref is ground
    // ?????

    _ADON = 1;    // enable module
}

void INT_Setup(void)
{
    _INT0IP = 4; // set priority
    _INT0IE = 1; // enable interrupt
    _INT0IF = 0; // clear flag
    _INT0EP = 1; // set edge detect polarity to positive edge
}

void Forward(void) { // Enter the distance in mm
    
    steps = 0;
    right_dir_pin = 1;
    left_dir_pin = 1;
    right_motor_T = fast_pwm;
    right_motor_dc = right_motor_T/2;
    left_motor_T = fast_pwm;
    left_motor_dc = left_motor_T/2;

}

void Backward(void) { // Enter distance in mm
    
    right_dir_pin = 0;
    left_dir_pin = 0;
    
    OC2RS = fast_pwm;
    OC2R = 400;
    OC3RS = fast_pwm;
    OC3R = 400;
}

void STOP(void) {
    OC2RS = 0;
    OC2R = 0;
    OC3RS = 0;
    OC3R = 0;
}

void Turn_Right(void) {
    
    right_dir_pin = 0;
    OC2RS = fast_pwm;
    OC2R = 400;
    OC3RS = fast_pwm;
    OC3R = 400;
}

void Turn_Left(void) {
    
    left_dir_pin = 0;
    OC2RS = fast_pwm;
    OC2R = 400;
    OC3RS = fast_pwm;
    OC3R = 400;
}

// Turns right to get back to line
void Adj_Right(void) {
    // speed up left motor
    
    left_motor_T = fast_pwm/2;
    left_motor_dc = left_motor_T/2;
    right_motor_T = slow_pwm;
    right_motor_dc = right_motor_T/2;
}

// Turns left to get back to line
void Adj_Left(void) {
    // speed up right motor
    left_motor_T = slow_pwm;
    left_motor_dc = left_motor_T/2;
    right_motor_T = fast_pwm/2;
    right_motor_dc = right_motor_T/2;
}

// Searches for line after getting off
void Search4Line(void) {
    
    steps = 0;
    
    if (previous_state == left)
    {
        Turn_Right();
        
        while(1)
        {
            if (right_QRD_dig == 1)
            {
                Forward();
                break;
            }
        }
    }
    
    if (previous_state == right)
    {
        Turn_Left();
        
        while(1)
        {
            if (left_QRD_dig == 1)
            {
                Forward();
                break;
            }
        }
    }
}

int Count_Tasks(void){
    
    Forward();
    
    while(1)
    {
        if(steps > 2400) // # of steps to get past all lines
        {
            break;
        }
    }
    
    return task_counter;
}