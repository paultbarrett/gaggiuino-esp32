#include "front_panel.h"
#include "PCF8574.h"
#include "log.h"

PCF8574 frontPanel(&I2C_0,PCF8574_addr);

void setFPLED(uint8_t pin, int state)
{
    if (state == 1)
    {
        frontPanel.digitalWrite(pin, HIGH);
    } else 
    {
        frontPanel.digitalWrite(pin, LOW);
    }
    
}

void frontPanelInit(void)
{
    // Init LEDs
    frontPanel.pinMode(powerLEDPin, OUTPUT, LOW);
	frontPanel.pinMode(brewLEDPin, OUTPUT, LOW);
    frontPanel.pinMode(steamLEDPin, OUTPUT,LOW);
    
    // Init Buttons
    frontPanel.pinMode(brewPin, INPUT);
    frontPanel.pinMode(steamPin, INPUT);
    
    // Turn on power LED
    setFPLED(powerLEDPin,1);

    if (frontPanel.begin())
    {
        LOG_INFO("Front Panel init");
    } else
    {
        LOG_ERROR("Front panel init failed");
    }
}

bool digitalReadFP(uint8_t pin)
{
    uint8_t val = frontPanel.digitalRead(pin,true);
    return val;
}