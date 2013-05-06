/*
*   Program name: 
        Half-Stepping
*   Description:
        This is a program used to turn the motor shaft clockwise or anti-clockwise under the control of an input pin.
*   Configuration:
        Microcontroller: PIC16F648
*   Notes: -
*/


   
#include <16F648A.h>
#use delay(clock=4000000)
#fuses NOWDT, INTRC, NOPUT, NOPROTECT, NOLVP, NOMCLR
#define CLOCKWISE 1
#define ANTICLOCKWISE 0


static int const LOOKUPTABLE[4]={0x02, 0x04, 0x08, 0x01};   // look up table
static int const SPEEDTABLE[4]={2.5, 5, 10, 20};                                        // 4 level of speed: 1x, 2x, 4x, 8x
static int index1 = 0;                   // index means the elements of LOOKUPTABLE
static int index2 = 0;
static int index3 = 0;
//static int speed1 = 0;
//static int speed2 = 0;


struct Motor_pin_map {                  // Definition of PIC pins
    boolean Sensor1;                   // Input
    int unused:7;
    int Motor1:4;                       //Outputs for 1st and 2nd motor
    int Motor2:4;
}MotorPort, MotorPortDirection;

#byte MotorPort = 0x05                  // 0x05 for PORTA, 0x06 for PORTB
#byte MotorPortDirection = 0x85         // 0x85 for TRISA, 0x86 for TRISB

/*
void Timer0_isr()
{
    motor2(200, CLOCKWISE);
}
*/// It is not needed in the robot


void initMain() {                       // Initialize variable of input and output
    MotorPortDirection.Sensor1 = 0b1;   // 1 for TRIS means input

    MotorPortDirection.Motor1 = 0x0;    // 0 for TRIS means output
    MotorPortDirection.Motor2 = 0x0;
}

void motorL(int speed1, int increment) {
//    int speed1;
	int i;

    for (i = 0; i < 5; i) {
        if ( increment == CLOCKWISE)
            index1 = (++index1)%4;            // read table as normal
        else if ( increment == ANTICLOCKWISE)
            index1 = (--index1)%4;            // read table as inverting 
        MotorPort.Motor1 = LOOKUPTABLE[index1];         // output by portb
        Delay_ms(speed1);                       // delay per step
    }
}

void motorR(int speed2, int increment) {
//    int speed2;
	int i;

    for (i = 0; i < 5; i) {                // endless loop
        if ( increment == CLOCKWISE)
            index2 = (++index2)%4;            // read table as normal
        else if ( increment == ANTICLOCKWISE)
            index2 = (--index2)%4;            // read table as inverting 
        MotorPort.Motor2 = LOOKUPTABLE[index2];         // output by portb
        Delay_ms(speed2);                       // delay per step
    }
}

// main
void main() 
{
       
    enable_interrupts(INT_RTCC);            // Timer0/RTCC interrupt
    enable_interrupts(GLOBAL);              // Global interrupt enable - switch all sources on together
    initMain();
/*    while(1) {
        if (MotorPort.Forward1 == 1)        // If the button connected to RA0 is pressed
            motor1(200, CLOCKWISE);              // The motor connected to RB shaft clockwise
        else if (MotorPort.Backward1 == 1)   // If pressed the button connected to RA1
            motor1(200, ANTICLOCKWISE);          // The motor connected to RB shaft anti-clockwise
        else 
            MotorPort.Motor1 = 0x0;         // If nothing input, Motor will stop
        
    }
 */

        while (1) {
            motorL(0,CLOCKWISE);
            motorR(0,ANTICLOCKWISE);

    }
}
   // End main()
