
#include "networkservices.h"
// #include "nextion.h"
// #include "web.h"
#include "util.h"

#include "arduino.h"
#include "WiFi.h"
#include <FS.h>
#include "LittleFS.h"
#include "SimpleFTPServer.h"
// #include "ESP8266mDNS.h"
// #include "WiFiUdp.h"
// #include "ArduinoOTA.h"

WiFiClient telnetClient;
WiFiServer telnetServer(23);
FtpServer ftpServer;

const unsigned long telnetInputMax = 128; // Size of user input buffer for user telnet session

void networkServicesInit()
{

    httpOTAInit();
    telnetInit();
    // mDNSInit();
    ftpInit();
    // arduinoOTAInit();
}

void wifiInit()
{
    
}

void httpOTAInit()
{
    if ((configPassword[0] != '\0') && (configUser[0] != '\0'))
    { // Start the webserver with our assigned password if it's been configured...
        httpOTAUpdate.setup(&webServer, "/update", configUser, configPassword);
    }
    else
    { // or without a password if not
        httpOTAUpdate.setup(&webServer, "/update");
    }
}

void telnetInit()
{
    // Start Telnet
    telnetServer.setNoDelay(true);
    telnetServer.begin();
    debugPrintln(String(F("TELNET: Server started on ")) + currentIP);
}

// void mDNSInit()
// {
//   if(!MDNS.begin(nodeName))
//   {
//     debugPrintln(String(F("MDNS: Failed to start service")));
//   } else
//   {
//     debugPrintln(String(F("MDNS: Service started")));
//   }

//   MDNS.addService(nodeName, "http", "tcp", 80);
//   MDNS.addService(nodeName, "telnet", "tcp", 23);
//   MDNS.addServiceTxt(nodeName, "app_name", "Coffee Grinder");
// }

void ftpInit()
{
    ftpServer.setCallback(_ftpCallback);
    ftpServer.setTransferCallback(_ftpTransferCallback);
    ftpServer.begin(configUser, configPassword); // default 21, 50009 for PASV)
}

void handleTelnetClient()
{ // Basic telnet client handling code from: https://gist.github.com/tablatronix/4793677ca748f5f584c95ec4a2b10303
    static unsigned long telnetInputIndex = 0;
    if (telnetServer.hasClient())
    { // client is connected
        if (!telnetClient || !telnetClient.connected())
        {
            if (telnetClient)
            {
                telnetClient.stop(); // client disconnected
            }
            telnetClient = telnetServer.available(); // ready for new client
            telnetInputIndex = 0;                    // reset input buffer index
        }
        else
        {
            telnetServer.available().stop(); // have client, block new connections
        }
    }
    // Handle client input from telnet connection.

    if (telnetClient && telnetClient.connected() && telnetClient.available())
    { // client input processing
        static char telnetInputBuffer[telnetInputMax];

        if (telnetClient.available())
        {
            char telnetInputByte = telnetClient.read(); // Read client byte
            // debugPrintln(String("telnet in: 0x") + String(telnetInputByte, HEX));
            if (telnetInputByte == 5)
            { // If the telnet client sent a bunch of control commands on connection (which end in ENQUIRY/0x05), ignore them and restart the buffer
                telnetInputIndex = 0;
            }
            else if (telnetInputByte == 13)
            { // telnet line endings should be CRLF: https://tools.ietf.org/html/rfc5198#appendix-C
              // If we get a CR just ignore it
            }
            else if (telnetInputByte == 10)
            {                                            // We've caught a LF (DEC 10), send buffer contents to the Nextion
                telnetInputBuffer[telnetInputIndex] = 0; // null terminate our char array
                nextionSendCmd(String(telnetInputBuffer));
                telnetInputIndex = 0;
            }
            else if (telnetInputIndex < telnetInputMax)
            { // If we have room left in our buffer add the current byte
                telnetInputBuffer[telnetInputIndex] = telnetInputByte;
                telnetInputIndex++;
            }
        }
    }
}

void _ftpCallback(FtpOperation ftpOperation, unsigned int freeSpace, unsigned int totalSpace)
{
    switch (ftpOperation)
    {
    case FTP_CONNECT:
        debugPrintln(F("FTP: Connected!"));
        break;
    case FTP_DISCONNECT:
        debugPrintln(F("FTP: Disconnected!"));
        break;
    case FTP_FREE_SPACE_CHANGE:
        // debugPrintln("FTP: Free space change, free %u of %u!\n", freeSpace, totalSpace);
        break;
    default:
        break;
    }
}

void _ftpTransferCallback(FtpTransferOperation ftpOperation, const char *name, unsigned int transferredSize)
{
    switch (ftpOperation)
    {
    case FTP_UPLOAD_START:
        debugPrintln(F("FTP: Upload start!"));
        break;
    case FTP_UPLOAD:
        // Serial.printf("FTP: Upload of file %s byte %u\n", name, transferredSize);
        break;
    case FTP_TRANSFER_STOP:
        debugPrintln(F("FTP: Finish transfer!"));
        break;
    case FTP_TRANSFER_ERROR:
        debugPrintln(F("FTP: Transfer error!"));
        break;
    default:
        break;
    }
}

void networkServicesHandle()
{
    ftpServer.handleFTP();
    handleTelnetClient();
    // ArduinoOTA.handle();
    // MDNS.update();
}