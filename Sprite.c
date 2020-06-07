// sprite.c
// Runs on LM4F120/TM4C123
// Jonathan Valvano
// September 26, 2013
// Example code from edX chapter 15

// http://www.spaceinvaders.de/
// sounds at http://www.classicgaming.cc/classics/spaceinvaders/sounds.php
// http://www.classicgaming.cc/classics/spaceinvaders/playguide.php
/* This example accompanies the books
   "Embedded Systems: Real Time Interfacing to Arm Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2013

   "Embedded Systems: Introduction to Arm Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2013

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */
 
 // ******* Required Hardware I/O connections*******************
// Slide pot pin 1 connected to ground
// Slide pot pin 2 connected to PE2/AIN1
// Slide pot pin 3 connected to pne side of the 1k resistor
// other side of the 1k resistor is connected to +3.3V 
// fire button connected to PE0
// special weapon fire button connected to PE1
// 8*R resistor DAC bit 0 on PB0 (least significant bit)
// 4*R resistor DAC bit 1 on PB1
// 2*R resistor DAC bit 2 on PB2
// 1*R resistor DAC bit 3 on PB3 (most significant bit)
// LED on PB4
// LED on PB5

// Blue Nokia 5110
// ---------------
// Signal        (Nokia 5110) LaunchPad pin
// Reset         (RST, pin 1) connected to PA7
// SSI0Fss       (CE,  pin 2) connected to PA3
// Data/Command  (DC,  pin 3) connected to PA6
// SSI0Tx        (Din, pin 4) connected to PA5
// SSI0Clk       (Clk, pin 5) connected to PA2
// 3.3V          (Vcc, pin 6) power
// back light    (BL,  pin 7) not connected, consists of 4 white LEDs which draw ~80mA total
// Ground        (Gnd, pin 8) ground

// Red SparkFun Nokia 5110 (LCD-10168)
// -----------------------------------
// Signal        (Nokia 5110) LaunchPad pin
// 3.3V          (VCC, pin 1) power
// Ground        (GND, pin 2) ground
// SSI0Fss       (SCE, pin 3) connected to PA3
// Reset         (RST, pin 4) connected to PA7
// Data/Command  (D/C, pin 5) connected to PA6
// SSI0Tx        (DN,  pin 6) connected to PA5
// SSI0Clk       (SCLK, pin 7) connected to PA2
// back light    (LED, pin 8) not connected, consists of 4 white LEDs which draw ~80mA total

#include "Nokia5110.h"


// *************************** Capture image dimensions out of BMP**********

#define other_racerW    ((unsigned char)other_racer[18])
#define other_racerH    ((unsigned char)other_racer[22])
#define RACERW     ((unsigned char)racer[18])
#define RACERH     ((unsigned char)racer[22])
	


typedef struct racer{
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
  const unsigned char *image[2]; // two pointers to images  
  long life;            // 0=dead, 1=alive
}racer_sprite;         

typedef struct other_racer{
  unsigned long x;      // x coordinate
  unsigned long y;      // y coordinate
	unsigned long vx;      // x coordinate
  const unsigned char *image[2]; // two pointers to images  
  long life;            // 0=dead, 1=alive
} other_racer_sprite;         

static other_racer_sprite racer2[10];
static racer_sprite racer1;
/*

void Init_racer2(void){ 
	  int i , y1 = 3;
	for(i=0;i<10;i++){
    racer2[i].x = 81;
    racer2[i].y  = 14;
		racer2[i].vx = 2;  // x--
    racer2[i].image[0] =  other_racer;
    racer2[i].image[1] =  SmallExplosion0;
    racer2[i].life = 1;
		if(i % 2 == 0 && i != 0){
              y1 = y1 - 28;	
		}
   }
 }*/
/*
void Init_racer1(void){ 
    racer1.x = 1;
    racer1.y = 1;
    racer1.image[0] =  racer;
    racer1.image[1] =  SmallExplosion0;
    racer1.life = 1;
   }
	 
	void Move_racer1(void){ int i;
  sw1 = 
  }
}
unsigned long FrameCount=0;
void Draw(void){ int i;
  Nokia5110_ClearBuffer();
  for(i=0;i<10;i++){
    if(racer2[i].life > 0){
     Nokia5110_PrintBMP(racer2[i].x, racer2[i].y, racer2[i].image[FrameCount], 0);
    }
  }
  Nokia5110_DisplayBuffer();      // draw buffer
	
  //FrameCount = 1 if collision
}
void collision(void){ 
  for(i=0;i<10;i++){
    if(racer2[i].life > 0 && racer1.life > 0 && ((racer1.x + 18 == racer2[i].x) || (racer1.y + 8 == racer2[i].y) || (racer1.y == racer2[i].y + 10))){
        racer2[i].life = 0;
			  FrameCount = 1;     //draw explotion then display that u dead
    }
  }
}
 
 
 
void Draw(void){ int i;
  Nokia5110_ClearBuffer();
  for(i=0;i<4;i++){
    if(Enemy[i].life > 0){
     Nokia5110_PrintBMP(Enemy[i].x, Enemy[i].y, Enemy[i].image, 0);
    }
  }
  Nokia5110_DisplayBuffer();      // draw buffer
}

void PortF_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000020;     // 1) F clock
  delay = SYSCTL_RCGC2_R;           // delay   
  GPIO_PORTF_LOCK_R = 0x4C4F434B;   // 2) unlock PortF PF0  
  GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0       
  GPIO_PORTF_AMSEL_R = 0x00;        // 3) disable analog function
  GPIO_PORTF_PCTL_R = 0x00000000;   // 4) GPIO clear bit PCTL  
  GPIO_PORTF_DIR_R = 0x0E;          // 5) PF4,PF0 input, PF3,PF2,PF1 output   
  GPIO_PORTF_AFSEL_R = 0x00;        // 6) no alternate function
  GPIO_PORTF_PUR_R = 0x11;          // enable pullup resistors on PF4,PF0       
  GPIO_PORTF_DEN_R = 0x1F;          // 7) enable digital pins PF4-PF0        
}

*/
