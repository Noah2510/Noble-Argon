/* 
 * File:   
 * Author: noahf
 * Comments:
 * Revision history: 
 */

#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

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

unsigned static int steps = 0;

// OC Interrupt for the left motor
void __attribute__((interrupt, no_auto_psv))_OC2Interrupt(void) 
{   
    _OC1IF = 0; // clear flag
    
    steps++; // Only counting steps on the left motor. 
} 

//OC interrupt for the right motor
void __attribute__((interrupt, no_auto_psv))_OC3Interrupt(void) 
{   
    _OC1IF = 0; // clear flag 
} 

void Motion_Setup(void) {
    // Configure OC2 interrupt
    _OC2IP = 4; // interrupt priority
    _OC2IE = 1; // Enable interrupt
    _OC2IF = 0; // clear flag
    
    // Configure OC3 interrupt
    _OC3IP = 4; // interrupt priority
    _OC3IE = 1; // Enable interrupt
    _OC3IF = 0; // clear flag
    
    // Configure direction pins as digital outputs
    _ANSB13 = 0; // right servo
    _TRISB13 = 0;
    
    _TRISB9 = 0; // left servo
    
    _LATB13 = 0; // start both servos in the same direction
    _LATB9 = 0; 
    
    // Configure PWM pins 
    //right motor
    OC2CON1 = 0;
    OC2CON2 = 0;
    OC2CON1bits.OCTSEL = 0b111;
    OC2CON1bits.OCM = 0b110;
    OC2CON2bits.SYNCSEL = 0b11111;
    OC2CON2bits.OCTRIG = 0;
    OC2RS = 0; 
    OC2R = 0; 
    
    //Left motor
    OC3CON1 = 0;
    OC3CON2 = 0;
    OC3CON1bits.OCTSEL = 0b111;
    OC3CON1bits.OCM = 0b110;
    OC3CON2bits.SYNCSEL = 0b11111;
    OC3CON2bits.OCTRIG = 0;
    OC3RS = 0; 
    OC3R = 0; 
}

void Forward(int dis) { // Enter the distance in mm
    
    // Wheel diameter is 95 mm 
    float rotations = dis/pi*95;
    int num_steps = 200 * rotations;
    
    while (steps < num_steps){
        OC2RS = 4999;
        OC2R = 3000;
        OC3RS = 4999;
        OC3R = 3000;
    }
    
    OC2RS = 0;
    OC2R = 0;
    OC3RS = 0;
    OC3R = 0;
}

void Backward(int dis) { // Enter distance in mm
    
    right_dir_pin = 1;
    left_dir_pin = 1;
    
    float rotations = dis/pi*95;
    int num_steps = 200 * rotations;
    
    while (steps < num_steps){
        
        OC2RS = 4999;
        OC2R = 3000;
        OC3RS = 4999;
        OC3R = 3000;
    }
    
    OC2RS = 0;
    OC2R = 0;
    OC3RS = 0;
    OC3R = 0;
}

void Turn_90(int dir) {
    
    steps = 0;
    
    if (dir == right){
        
        unsigned int num_steps = 0; // set # of steps for left motor
        
        while(steps < num_steps){
            
            left_dir_pin = 1; 
            
            OC2RS = 4999;
            OC2R = 3000;
            OC3RS = 4999;
            OC3R = 3000;
        }
        
        left_dir_pin = 0; // return to forward direction
    }
    else if (dir == left){
        
        unsigned int num_steps = 0; // set # of steps for left motor
        
        while(steps < num_steps) { 
            
            right_dir_pin = 1; 
            
            OC2RS = 4999;
            OC2R = 3000;
            OC3RS = 4999;
            OC3R = 3000;
        }
        
        right_dir_pin = 0; // return to forward
    }
    
    OC2RS = 0; // stop the robot
    OC2R = 0;
    OC3RS = 0;
    OC3R = 0;
}

void Turn_180(void){
    
    steps = 0;
    unsigned int num_steps = 0; // calculate number needed
    
    while (steps < num_steps) {
        
        left_dir_pin = 1;
        
        OC2RS = 4999;
        OC2R = 3000;
        OC3RS = 4999;
        OC3R = 3000;
    }
    
    OC2RS = 0; // stop the robot
    OC2R = 0;
    OC3RS = 0;
    OC3R = 0;
    
    left_dir_pin = 0; // back to forwards
}