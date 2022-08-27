#ifndef PRESURE_SENSSOR_H
#define PRESURE_SENSSOR_H

#include <Arduino.h>
#include "Wire.h"

void    pressureSensorInit(void);
float   getPressure(void);
bool    isPressureFalling(void);
int8_t  getAdsError(void);

extern TwoWire I2C_0;
#endif
