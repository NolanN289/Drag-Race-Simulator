// Sensors_Lights.h

// By Nolan Nguyen
// Initialization Protoypes

// Initialize the two sensors, enable both edge edge-triggered interrupt for both sensors
void Sensors_Init(void);

// Initialize the reset button
void Reset_Init(void);

// Initialize 8 LEDs
void LightsPortB_Init(void);
void LightsPortC_Init(void);