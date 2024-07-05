// Sensors_Lights.c
// Peripheral Initialization file for CECS346 Project 2
// By Nolan Nguyen

// On the TM4C Microcontroller

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include "Sensors_Lights.h"


/*
Left Button - PA2
Right Button - PA3

Reset Button - PE3

Right Red - PB0
Left Red - PB1
Right Green - PB2
Left Green - PB3
Right Yellow 2 - PC4
Left Yellow 2 - PC5
Right Yellow 1 - PC6
Left Yellow 1 - PC7
*/

// Initialize the two sensors, enable both edge edge-triggered interrupt for both sensors
void Sensors_Init(void){
	SYSCTL_RCGC2_R |= 0x00000001; // Activate clock for Port A
   while ((SYSCTL_PRGPIO_R & 0x00000001) == 0) {}; // Wait for clock to start
   GPIO_PORTA_DIR_R &= ~0x0C; // Make PA2 and PA3 inputs
   GPIO_PORTA_DEN_R |= 0x0C; // Enable digital I/O on PA2 and PA3
   GPIO_PORTA_AFSEL_R &= ~0x0C; // Disable alternate functions on PA2 and PA3
   GPIO_PORTA_PCTL_R &= ~0x0000FF00; // Clear PCTL for PA2 and PA3
		 
	//Port A interrupt
   GPIO_PORTA_IS_R &= ~0x0C; // Set PA2 and PA3 to edge-sensitive
   GPIO_PORTA_IBE_R |= 0x0C; // Enable both edges for PA2 and PA3
   GPIO_PORTA_ICR_R = 0x0C; // Clear any prior interrupt on PA2 and PA3
   GPIO_PORTA_IM_R |= 0x0C; // Arm interrupt on PA2 and PA3
   NVIC_PRI0_R = (NVIC_PRI0_R & 0xFFFF00FF) | 0x00002000; // Set priority to 2 for Port A interrupts
   NVIC_EN0_R = 0x00000001; // Enable interrupt 0 in NVIC
}

// Initialize the reset button: use level triggered interrupt
void Reset_Init(void){
	SYSCTL_RCGC2_R |= 0x00000010;
	while ((SYSCTL_PRGPIO_R & 0x00000010) == 0) {}; // Allow time for clock to start	// activate clock for Port E
	GPIO_PORTE_DIR_R &= ~0x08;															// make PE3 inputs
	GPIO_PORTE_DEN_R |= 0x08;																// enable digital I/O on PE3
	GPIO_PORTE_AMSEL_R = 0;																	// disable analog function for PE3
	GPIO_PORTE_PCTL_R &= ~0x0000F000;												// GPIO clear bit PCTL for PE3
	GPIO_PORTE_AFSEL_R &= ~0x08;														// no alternate function for PE3
	
	// Port E Interrupt
	GPIO_PORTE_IS_R |= 0x08;																// PE3 is level sensitive
	GPIO_PORTE_IEV_R |= 0x08;
	GPIO_PORTE_ICR_R |= 0x08;																// clear flag
	GPIO_PORTE_IM_R |= 0x08;																// arm interrupt on PE3
	NVIC_PRI1_R = (NVIC_PRI1_R & 0xFFFFFF00) | 0x00000020;	// priority 1
	NVIC_EN0_R = 0x00000010;																// enable interrupt 4 in NVIC
}

// Initialize 8 LEDs
void LightsPortB_Init(void){
	 SYSCTL_RCGC2_R |= 0x00000002; // Activate clock for Port B
   while ((SYSCTL_PRGPIO_R & 0x00000002) == 0) {}; // Wait for clock to start
   GPIO_PORTB_AMSEL_R &= ~0x0F; // Disable analog function for PB3-PB0
   GPIO_PORTB_PCTL_R &= ~0x0000FFFF; // GPIO clear bit PCTL for PB3-PB0
   GPIO_PORTB_DIR_R |= 0x0F; // PB2-PB0 outputs
   GPIO_PORTB_AFSEL_R &= ~0x0F; // No alternate function for PB3-PB0
   GPIO_PORTB_DEN_R |= 0x0F; // Enable digital pins PB3-PB0
}

void LightsPortC_Init(void){
	SYSCTL_RCGC2_R |= 0x00000004;														// activate clock for Port C
	while ((SYSCTL_PRGPIO_R & 0x00000004) == 0) {};
	GPIO_PORTC_AMSEL_R &= ~0xF0;														// disable analog function for PC7 - PC4
	GPIO_PORTC_PCTL_R &= ~0xFFFF0000;												// GPIO clear bit PCTL for PC7 - PC4
	GPIO_PORTC_DIR_R |= 0xF0;																// PC7 - PC4 output
	GPIO_PORTC_AFSEL_R &= ~0xF0;														// no alternate function for PC7 - PC4
	GPIO_PORTC_DEN_R |= 0xF0;																// enable digital pins PC7 - PC4
}