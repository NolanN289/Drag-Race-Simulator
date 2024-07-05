// SysTick.c
// Runs on TM4C123
// SysTick Timer file for CECS346 Project 2
// By Nolan Nguyen


// oscilloscope or LED connected to PF2 for period measurement

#include "tm4c123gh6pm.h"
#include <stdint.h> // C99 data types

void SysTick_Init(void){
    NVIC_ST_CTRL_R = 0; // Disable SysTick during setup
    //NVIC_ST_CURRENT_R = 0; // Clear current value
    //NVIC_ST_CTRL_R = 0x07; // Enable SysTick with core clock
		NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x60000000;	// priority 3
		NVIC_ST_CTRL_R = NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN;
}

void SysTick_Start(uint32_t period) {
    NVIC_ST_RELOAD_R = period - 1; // Load the reload value
    NVIC_ST_CURRENT_R = 0; // Clear current value
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; // Enable SysTick
}

void SysTick_Stop(void) {
    NVIC_ST_CTRL_R &= ~NVIC_ST_CTRL_ENABLE; // Disable SysTick
		//NVIC_ST_CURRENT_R = 0; // Clear current value
}