/*
*   Program name: 
        Robot Car
*   Description:
        The program is made for PIC16F648 used in robot car input with sensors and output with stepper motors.
*   Configuration:
        Microcontroller: PIC16F648
*   Notes: -
*/

/*********************  Input/Output Pin Definitions    *************************
 *  Port A Pin Definitions
 *  Bit 0: Sensor1
 *  Bit 1: Sensor2
 *  Bit 2: Sensor3
 *  Bit 3: Sensor4
 *  Bit 4: Sensor5
 *  Bit 5:
 *  Bit 6:
 *  Bit 7: Power Button
 *
 *  Port B Pin Definitions
 *  Bit 0-3: Left Motor
 *  Bit 4-7: Right Motor
 ********************************************************************************/


   
#include <16F648A.h>
#use delay(clock=4000000)
#fuses NOWDT, INTRC, NOPUT, NOPROTECT, NOLVP, NOMCLR


static int const LOOKUPTABLE[32]={0xA,0xA,0xA,0xA,0xA,0xA,0xA,0xA,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x3,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0x5,0xC,0xC,0xC,0xC,0xC,0xC,0xC,0xC};         //look up table for 8x spped.
static int index1 = 0;                   // index means the elements of LOOKUPTABLE
static int index2 = 0;



struct Motor_pin_map {                  // Definition of PIC pins
    int Sensor:5;                  // Input Sensors
    int unused:2;
    boolean Power;
    int Motor:8;                       //Outputs for both 1st and 2nd motor
}MotorPort, MotorPortDirection;

#byte MotorPort = 0x05                  // 0x05 for PORTA, 0x06 for PORTB
#byte MotorPortDirection = 0x85         // 0x85 for TRISA, 0x86 for TRISB



void initMain() {                       // Initialize variable of input and output
    MotorPortDirection.Sensor = 0b11111;   // 1 for TRIS means input
    MotorPortDirection.Power = 0b1;
    MotorPortDirection.Motor = 0x00;    // 0 for TRIS means output
}



// main
void main() 
{
    initMain();
    
    int right1, left1, right2, left2;

    while(1) {
        
        switch (MotorPort.Sensor) {            // 0 for in black line, 1 for out of black line
            case 0b00100:
                index1 = (index1 + 8) % 32;     // index1 for right motor, index2 for left motor
                index2 = (index2 + 8) % 32;
                break;
            case 0b01100:
                index1 = (index1 + 8) % 32;
                index2 = (index2 + 4) % 32;
                break;
            case 0b00110:
                index1 = (index1 + 4) % 32;
                index2 = (index2 + 8) % 32;
                break;
            case 0b11100:
                index1 = (index1 + 8) % 32;
                index2 = (index2 + 2) % 32;
                break;
            case 0b00111:
                index1 = (index1 + 2) % 32;
                index2 = (index2 + 8) % 32;
                break;
            default:
                left2 = (MotorPort.Sensor & 0b10000) >> 4;
                left1 = (MotorPort.Sensor & 0b01000) >> 3;
                right1 = (MotorPort.Sensor & 0b00010) >> 1;
                right2 = (MotorPort.Sensor & 0b00001);
                index2 = ((right2 - left2) * 4 + (right1 - left1) + 4 + index2) % 32;
                index1 = ((left2 - right2) * 4 + (left1 - right1) + 4 + index1) % 32;
                break;
        }
        MotorPort.Motor = LOOKUPTABLE[index1] + (LOOKUPTABLE[index2] << 4);       // output is left + right.
        Delay_ms(5);
        
    }
}   // End main()
