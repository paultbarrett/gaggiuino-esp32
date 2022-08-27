#ifdef ESP32

#include "Arduino.h"
#include "SimpleFTPServer.h"

void wifiInit();
void wifiCheck();
void wifiReconnect();
void networkServicesInit();
void telnetInit();
//void mDNSInit();
void ftpInit();
void handleTelnetClient();
void networkServicesHandle();

void _ftpCallback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace);
void _ftpTransferCallback(FtpTransferOperation ftpOperation, const char *name, unsigned int transferredSize);

// Global class
extern WiFiClient telnetClient;

#endif