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
#define med_pwm 1249
#define fast_pwm 624
#define slow_pwm 1999

void Motion_Setup(void) {
    
    // Configure direction pins as digital outputs
    _ANSB13 = 0; // right servo
    _TRISB13 = 0;
    
    _TRISB9 = 0; // left servo
    
    _TRISB7 = 0;
    _LATB7 = 0;
    
    right_dir_pin = 1; // start both servos in the same direction
    left_dir_pin = 1; 
    
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

void Forward(void) { // Enter the distance in mm
    
    right_dir_pin = 1;
    left_dir_pin = 1;
    OC2RS = fast_pwm;
    OC2R = 400;
    OC3RS = fast_pwm;
    OC3R = 400;

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