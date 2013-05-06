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
 *  Bit 4:
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


static int const FORWARD[4]={0xCA,0x53,0x35,0xAC};   // look up table
static int const TURNLEFT[4]={0xC0,0x50,0x30,0xA0};
static int const TURNRIGHT[4]={0x0A,0x03,0x05,0x0C};
static int const BACKWARD[4]={0xAC,0x35,0x53,0xCA};
static int const SPEEDTABLE[4]={2.5, 5, 10, 20};         // 4 level of speed: 1x, 2x, 4x, 8x
static int index1 = 0;                   // index means the elements of LOOKUPTABLE
static int index2 = 0;
static int counter = 0;



struct Motor_pin_map {                  // Definition of PIC pins
    boolean Sensor1;                   // Input Sensors
    boolean Sensor2;
    boolean Sensor3;
    boolean Sensor4;
    int unused:3;
    boolean Power;
    int Motor:8;                       //Outputs for both 1st and 2nd motor
}MotorPort, MotorPortDirection;

#byte MotorPort = 0x05                  // 0x05 for PORTA, 0x06 for PORTB
#byte MotorPortDirection = 0x85         // 0x85 for TRISA, 0x86 for TRISB



void initMain() {                       // Initialize variable of input and output
    MotorPortDirection.Sensor1 = 0b1;   // 1 for TRIS means input
    MotorPortDirection.Sensor2 = 0b1;
    MotorPortDirection.Sensor3 = 0b1;
    MotorPortDirection.Sensor4 = 0b1;
    MotorPortDirection.Power = 0b1;
    MotorPortDirection.Motor = 0x00;    // 0 for TRIS means output
}




// main
void main() 
{
    initMain();
    
    MotorPort.Motor = 0x00;

    while(MotorPort.Power == 1) {
        
        if ((MotorPort.Sensor1 == 1) && (MotorPort.Sensor2 == 1) && (MotorPort.Sensor3 == 1) && (MotorPort.Sensor4 == 1)) {    // Input 0000 will go back to find the line.
            while(1) {
                counter = (++counter)%4;
                MotorPort.Motor = BACKWARD[counter];
                Delay_ms(20);
                if (!(MotorPort.Sensor1 == 1) && (MotorPort.Sensor2 == 1) && (MotorPort.Sensor3 == 1) && (MotorPort.Sensor4 == 1)) break;
            }
        }
        else if ((MotorPort.Sensor1 == 1) && (MotorPort.Sensor2 == 0) && (MotorPort.Sensor3 == 0) && (MotorPort.Sensor4 == 1)) {    // Input 0110 will go straight.
            while(1) {
                counter = (++counter)%4;
                MotorPort.Motor = FORWARD[counter];
                Delay_ms(10);
                if (!(MotorPort.Sensor1 == 1) && (MotorPort.Sensor2 == 0) && (MotorPort.Sensor3 == 0) && (MotorPort.Sensor4 == 1)) break;
            }
        }
        else if ((MotorPort.Sensor1 == 1) && (MotorPort.Sensor2 == 0) && (MotorPort.Sensor3 == 0) && (MotorPort.Sensor4 == 0)) {    // Input 0111 will turn right.
            while(1) {
                counter = (++counter)%4;
                MotorPort.Motor = TURNRIGHT[counter];
                Delay_ms(15);
                if (!(MotorPort.Sensor1 == 1) && (MotorPort.Sensor2 == 0) && (MotorPort.Sensor3 == 0) && (MotorPort.Sensor4 == 0)) break;
            }
        }
        else if ((MotorPort.Sensor1 == 0) && (MotorPort.Sensor2 == 0) && (MotorPort.Sensor3 == 0) && (MotorPort.Sensor4 == 1)) {    // Input 1110 will turn left.
            while(1) {
                counter = (++counter)%4;
                MotorPort.Motor = TURNLEFT[counter];
                Delay_ms(15);
                if (!(MotorPort.Sensor1 == 0) && (MotorPort.Sensor2 == 0) && (MotorPort.Sensor3 == 0) && (MotorPort.Sensor4 == 1)) break;
            }
        }
        
    }
}   // End main()
