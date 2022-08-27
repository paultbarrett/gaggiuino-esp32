#include "utils.h"
#include "networkservices.h"

float mapRange(float refNumber, float refStart, float refEnd, float targetStart, float targetEnd, int decimalPrecision) {
  float deltaRef = refEnd - refStart;
  float deltaTarget = targetEnd - targetStart;

  float pct = fmax(0.0f, fmin(1.0f, abs((refNumber - refStart) / deltaRef)));
  float finalNumber = targetStart + pct * deltaTarget;

  int calcScale = (int) pow(10, decimalPrecision);
  return (float) round(finalNumber * calcScale) / calcScale;
}

void debugPrintln(String debugText)
{ // Debug output line of text to our debug targets
    String debugTimeText = "[+" + String(float(millis()) / 1000, 3) + "s] " + debugText;
    
    USART_DEBUG.begin(115200);
    USART_DEBUG.println(debugTimeText);
    USART_DEBUG.flush();

    if (telnetClient.connected())
    {
        telnetClient.println(debugTimeText);
    } 
}