#ifdef ESP32

#include "Arduino.h"

#include "SimpleFTPServer.h"

void networkServicesInit();
//void arduinoOTAInit(); 
//void httpOTAInit();
void telnetInit();
//void mDNSInit();
void ftpInit();
void handleTelnetClient();
void networkServicesHandle();

void _ftpCallback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace);
void _ftpTransferCallback(FtpTransferOperation ftpOperation, const char *name, unsigned int transferredSize);

extern char configUser[];
extern char configPassword[];
extern String currentIP;

// Global class
extern WiFiClient telnetClient;

#endif