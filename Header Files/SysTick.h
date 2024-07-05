// SysTick.h
// Runs on TM4C123
// Systick Function Prototypes
// By Nolan Nguyen


#include <stdint.h> // C99 data types

void SysTick_Init(void);

void SysTick_Start(uint32_t period);

void SysTick_Stop(void);