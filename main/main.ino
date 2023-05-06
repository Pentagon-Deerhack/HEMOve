
//Defining libraries.
#define dhtType DHT22
#define TINY_GSM_MODEM_SIM900
#define SerialMon Serial

#define TOKEN "b8n8FRtZeBWcPrv7MIoj"
#define THINGSBOARD_SERVER "demo.thingsboard.io"
#define THINGSBOARD_PORT 80

//Defining Pins.
#define dhtPin 2
#define gpsRx 7
#define gpsTx 6

//Importing libraries.
#include <DHT.h>;
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <TinyGsmClient.h>
#include "ThingsBoard.h"
#include <StreamDebugger.h>

//Initializing variables.
float tempVal = 0.0;
float latVal = 0.0;
float lonVal = 0.0;

//Initialization for GSM.
const char apn[] = "web";
const char user[] = "";
const char pass[] = "";
const char server[] = "demo.thingsboard.io";

//For debugging.
#ifdef DUMP_AT_COMMANDS
StreamDebugger debugger(SerialMon, SerialMon);
TinyGsm modem(debugger);
#else
TinyGsm modem(SerialMon);
#endif

#ifdef USE_SSL
TinyGsmClientSecure client(modem);
const int port = 443;
#else
TinyGsmClient client(modem);
const int port = 80;
#endif

//Creating objects.
DHT dht(dhtPin, dhtType);
TinyGPS gps;
SoftwareSerial ss(gpsTx, gpsRx);
ThingsBoardHttp tb(client, TOKEN, server, port);

//Declaring functions.
static void smartDelay(unsigned long ms);

// Define pin modes (input or output) and serial monitor for debugging.
// Will run only once.
void setup() {

  //Initializing libraries.
  dht.begin();
  SerialMon.begin(115200);
  ss.begin(9600);

  //Setting baud rate of GSM module.
  SerialMon.write("AT+IPR=4800;at&w\r\n");
  SerialMon.end();

  delay(1000);
  
  //Begining serial monitor & GSM module at baud rate 4800.
  SerialMon.begin(4800);
  delay(3000);

  SerialMon.println(F("Initializing modem..."));
  modem.restart();
  
  String modemInfo = modem.getModemInfo();
  SerialMon.print(F("Modem: "));
  SerialMon.println(modemInfo);
}

void loop() {

  //Getting temperature data.
  tempVal = dht.readTemperature();

  //Getting latitude and longitude.
  gps.f_get_position(&latVal, &lonVal);

  //Trying to establsh connection with the network provider.
  SerialMon.print(F("Waiting for network..."));
  if (!modem.waitForNetwork()) {
    SerialMon.println(" fail");
    delay(1000);
    return;
  }
  SerialMon.println(" OK");

  //Trying to establsh connection with the server. 
  SerialMon.print(F("Connecting to "));
  SerialMon.print(apn);
  if (!modem.gprsConnect(apn, user, pass)) {
    SerialMon.println(" fail");
    delay(1000);
    return;
  }
  SerialMon.println(" OK");
  
  //Sending data to thingsboard server.
  Serial.println("Sending  data...");
  tb.sendTelemetryFloat("latitude", latVal);
  tb.sendTelemetryFloat("longitude", lonVal);
  tb.sendTelemetryFloat("temperature", tempVal);

  smartDelay(1000);
}


//Function to read gps data for 1 second.
static void smartDelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
