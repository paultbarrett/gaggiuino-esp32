#ifndef PERIPHERALS_H
#define PERIPHERALS_H

#include "pindef.h"
#include "peripherals.h"
#include "front_panel.h"
#include "PCF8574.h"
#include <Wire.h>
#include <Arduino.h>

// Non default I2C pins
#if defined(I2C_SDA) || defined(I2C_SCL)
TwoWire I2C_0 = TwoWire(0);
#endif

void i2cInit()
{
  I2C_0.begin(I2C_SDA, I2C_SCL);
}

static inline void pinInit(void) {
  #if defined(LEGO_VALVE_RELAY)
    pinMode(valvePin, OUTPUT_OPEN_DRAIN);
  #else
    pinMode(valvePin, OUTPUT);
  #endif
  pinMode(relayPin, OUTPUT);
  #ifndef PCF8574_FRONTPANEL
  pinMode(brewPin,  INPUT_PULLUP);
  pinMode(steamPin, INPUT_PULLUP);
  #endif
  pinMode(HX711_dout_1, INPUT_PULLUP);
  pinMode(HX711_dout_2, INPUT_PULLUP);
}

// Actuating the heater element
static inline void setBoilerOn(void) {
  digitalWrite(relayPin, HIGH);  // boilerPin -> HIGH
}

static inline void setBoilerOff(void) {
  digitalWrite(relayPin, LOW);  // boilerPin -> LOW
}

//Function to get the state of the brew switch button
//returns true or false based on the read P(power) value
static inline bool brewState(void) {  //Monitors the current flowing through the ACS712 circuit and returns a value depending on the power value (P) the system draws
  #ifdef PCF8574_FRONTPANEL
  return digitalReadFP(brewPin) == LOW;
  #else
  return digitalRead(brewPin) == LOW; // pin will be low when switch is ON.
  #endif
}

// Returns HIGH when switch is OFF and LOW when ON
// pin will be high when switch is ON.
static inline bool steamState(void) {
  #ifdef PCF8574_FRONTPANEL
  return digitalReadFP(steamPin) == LOW;
  #else
  return digitalRead(steamPin) == LOW; // pin will be low when switch is ON.
  #endif
}

static inline void openValve(void) {
  #if defined(LEGO_VALVE_RELAY)
    digitalWrite(valvePin, LOW);
  #else
    digitalWrite(valvePin, HIGH);
  #endif
}

static inline void closeValve(void) {
  #if defined(LEGO_VALVE_RELAY)
    digitalWrite(valvePin, HIGH);
  #else
    digitalWrite(valvePin, LOW);
  #endif
}

#endif
