#include <Arduino.h>
#include "Wire.h"

void setFPLED(uint8_t pin, int state);
void frontPanelInit(void);
bool digitalReadFP(uint8_t pin);

extern TwoWire I2C_0;