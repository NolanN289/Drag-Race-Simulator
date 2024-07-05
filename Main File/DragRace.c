// DragRace.c
// file for CECS346 Project 2
// By Nolan Nguyen
// 3/21/2024

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types
#include <stdbool.h> // provides boolean data type
#include "PLL.h"
#include "SysTick.h"
#include "Sensors_Lights.h"

// Function Prototypes
// External functions from startup.s
extern void DisableInterrupts(void); // Disable interrupts
extern void EnableInterrupts(void);  // Enable interrupts
extern void WaitForInterrupt(void);  // Go to low power mode while waiting for the next interrupt

// Functions defined in this file.
void System_Init(void);
void GPIOPortA_Handler(void);
void GPIOPortE_Handler(void);
void SysTick_Handler(void);
void Debounce(void);

//TODO: Bit addresses for two sensors, 8 color lights, and one reset button 
#define SENSORS 								(*((volatile unsigned long *)    0x40004030      )) // bit addresses for 2 sensors 
#define LIGHTSPORTB             (*((volatile unsigned long *)    0x4000503C   	 )) // bit addresses for 8 Race Lights 
#define LIGHTSPORTC							(*((volatile unsigned long *)    0x400063C0 	   ))
#define RESET                   (*((volatile unsigned long *)    0x40024020      )) // bit address for one reset button
 
// FSM 
struct State { 
	uint8_t Out;
	uint8_t Time;     // multiple of 0.5 second
	uint8_t Next[4];
};

typedef const struct State STyp;

// Constant definitions for FSM outputs
 #define START             0xFF

// TODO: define reload value for half second
#define HALF_SEC   8000000

//TODO: assign a value to all states in Drag Race FSM
enum DragRace_states { 
	Init, Wait, Y1, Y2, Go,
	FalseSL, FalseSR, FalseB,
	WinLeft, WinRight, WinBoth
};

#define Init			0
#define Wait			1
#define Y1				2
#define Y2				3
#define Go 				4
#define FalseSL		5
#define FalseSR		6
#define FalseB		7
#define WinLeft		8
#define WinRight	9
#define WinBoth		10

//TODO: Define Drag Race FSM
STyp DragRace_FSM[11] = {
	{START, 2, {Wait, Wait, Wait, Wait}},	          					
	{0x00, 1, {Wait, Wait, Wait, Y1}},																		
	{0xC0, 1, {FalseB, FalseSL, FalseSR, Y2}},																						
	{0x30, 1, {FalseB, FalseSL, FalseSR, Go}},			
	{0x0C, 1, {WinBoth, WinLeft, WinRight, Go}},		
	{0x02, 2, {Wait, Wait, Wait, Wait}},						
	{0x01, 2, {Wait, Wait, Wait, Wait}},						
	{0x03, 2, {Wait, Wait, Wait, Wait}},						
	{0x08, 2, {Wait, Wait, Wait, Wait}},						
	{0x04, 2, {Wait, Wait, Wait, Wait}},						
	{0x0C, 2, {Wait, Wait, Wait, Wait}}
};

uint8_t S;  // current state index
uint8_t Input;
bool timesup;
bool reset;  // flag to reset the system, set by the reset button located at breadboard, not the launchpad reset button.
	
	
int main(void){
	System_Init();
		
  while(1){ 
		while (!reset) {
			// TO Do: take care of FSM outputs and time in state.
			LIGHTSPORTB = DragRace_FSM[S].Out;
			LIGHTSPORTC = DragRace_FSM[S].Out;
			SysTick_Start(DragRace_FSM[S].Time*HALF_SEC);
			while((!timesup)&&(!reset)){
			  WaitForInterrupt();
			}
			timesup=false;
			S = DragRace_FSM[S].Next[Input];
		}
		reset = false; 
		S = Init;
		
  }
}

// Initialize the following hardware modules: PLL, Sensors, Reset button, Lights, SysTick timer
// Initialize the following golbal variables: timesup for FSM state time, reset for reset the whole system
void System_Init(void) {
	DisableInterrupts();
	PLL_Init();
  Sensors_Init(); 
	Reset_Init(); 
	LightsPortB_Init();
	LightsPortC_Init();
	SysTick_Init(); 
  // TODO: assign Initial state, reset global variables: timesup and reset 
   S = Init ; 
	timesup = false;
	reset = false;
	Input = SENSORS >> 2;
	EnableInterrupts();
}

void GPIOPortA_Handler(void) {
	Debounce(); //debounce the button 
	
	if (GPIO_PORTA_RIS_R & 0x04) { // PA2
		if (LIGHTSPORTB == 0xC0 || LIGHTSPORTB == 0x30) {
			S = FalseSR;
	GPIO_PORTA_ICR_R = 0x04;  // clear flag
	Input = SENSORS >> 2;
	}
	else{
		GPIO_PORTA_ICR_R = 0x04;  // clear flag
		Input = SENSORS >> 2;
		}
	}
	Debounce();
	if (GPIO_PORTA_RIS_R & 0x08) { // PA3
		if (LIGHTSPORTB == 0xC0 || LIGHTSPORTB == 0x30) {
			S = FalseSL;
			GPIO_PORTA_ICR_R = 0x08; // clear flag
			Input = SENSORS >> 2;
		}
		else{
			GPIO_PORTA_ICR_R = 0x08; // clear flag
			Input = SENSORS >> 2;
		}
	}
	Debounce();
	if (GPIO_PORTA_RIS_R & 0x0C) { // PA2 & PA3
		if (LIGHTSPORTB == 0xC0 || LIGHTSPORTB == 0x30) {
			S = FalseB;
			GPIO_PORTA_ICR_R = 0x0C; // clear flag
			Input = SENSORS >> 2;
		}
		else{
			GPIO_PORTA_ICR_R = 0x0C; // clear flag
			Input = SENSORS >> 2;
		}
	}
}

// Interrupt handler for reset button
void GPIOPortE_Handler(void) {
	if (GPIO_PORTE_RIS_R & 0x08) { // PE3
		GPIO_PORTE_ICR_R |= 0x08; // clear flag
		reset = true;
	}
	else {
		GPIO_PORTE_ICR_R = 0x08;  // clear flag
		
	}
}
// Systick interrupt handler: stop systick timer and take care of timesup 
void SysTick_Handler(void) {
	SysTick_Stop();
	timesup = true;
}

void Debounce(void) {
	unsigned long j;
	for (j = 0; j < 199999; j++) {
	}
}
