
// ***** 0. Documentation Section *****
// Runs on TM4C123
// Basic embedded C program to toggle an external LED
// while a button is pressed and turn the LED on when the
// button is released. 
// Author: Erico Cruz
// Date: Feb. 18, 2014

// ***** 1. Pre-processor Directives Section *****
#include "registers.h"			// TM4C123

// ***** 2. Global Declarations Section *****
unsigned long inputSwitch;		// Input from PE0
unsigned long outputLed;		// Output to PE1
unsigned long data;

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); 				// Disable interrupts
void EnableInterrupts(void);  				// Enable interrupts
void Delay100ms(unsigned long time); 			// Delay100ms based on 80Mhz MCU freq.
void PortE_Init(void);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic

int main(void){ 
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************

  PortE_Init();
	
  while(1){
		
		inputSwitch = GPIO_PORTE_DATA_R&0x01; // read PE0 into Sw1
		if (inputSwitch == 0x01) {
			Delay100ms(1);						// Toggle bit frequency 5Hz
			outputLed ^= outputLed; 
			data = GPIO_PORTE_DATA_R;
		} 
		else {
			outputLed = 0x02;
			data = GPIO_PORTE_DATA_R;
		}
		GPIO_PORTE_DATA_R = inputSwitch | outputLed;
  }
  
}

void PortE_Init(void) {
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x10;	// 1) activate clock for Port E
	delay = SYSCTL_RCGC2_R; // allow time for clock to start
	
	//GPIO_PORTF_LOCK_R = 0x4C4F434B; // 2) unlock GPIO Port F
  //GPIO_PORTF_CR_R = 0x1F; // allow changes to PF4-0
  // only PF0 needs to be unlocked, other bits can't be locked
	
	GPIO_PORTE_AMSEL_R 	&= ~0x03; 		// 3) disable analog on PE0 and PE1
  	GPIO_PORTE_PCTL_R 	&= ~0x000000FF; 	// 4) PCTL GPIO on PF2 4
  	GPIO_PORTE_DIR_R 	&= ~0x01; 		// 5) PE0 input(0)
  	GPIO_PORTE_DIR_R 	|= 0x02; 		// 5) PE1 output(1)
  	GPIO_PORTE_AFSEL_R 	&= ~0x03; 		// 6) disable alt funct on PE0 and PE1
  	//GPIO_PORTE_PUR_R 	|= 0x10; 		// enable pull-up on PF0 and PF4 no needed
  	GPIO_PORTE_DEN_R 	|= 0x03; 		// 7) enable digital I/O on PF2, 4
  	GPIO_PORTE_DATA_R 	|= 0x02; 		// The system starts with the LED ON (make PE1 =1). 

}

void Delay100ms(unsigned long time){
	unsigned long i;
	while(time > 0){
		i = 1333333; 		// 100ms based on 80Mhz clock. Not an accurate timer using SysTick
		while(i > 0){
			i = i - 1;
		}
		time = time - 1; 	// decrements every 100 ms
	}
}
