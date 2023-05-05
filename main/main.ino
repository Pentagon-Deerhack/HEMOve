//Importing libraries.
#include <DHT.h>;
//For GPS
#include <SoftwareSerial.h>
#include <TinyGPS.h>

//Defining Pins.
#define dhtPin 2
#define gpsRx 7
#define gpsTx 6

//Defining libraries.
#define dhtType DHT22

//Declaring libraries.
DHT dht(dhtPin, dhtType);
TinyGPS gps;
SoftwareSerial ss(gpsTx, gpsRx);

//Initializing variables.
float tempVal = 0.0;
float latVal = 0.0;
float lonVal = 0.0;


//Declaring functions.
static void smartdelay(unsigned long ms);
static void print_float(float val, float invalid, int len, int prec);


// Define pin modes (input or output) and serial monitor for debugging.
// Will run only once.
void setup() {

  //Initializing libraries.
  dht.begin();

  //Baud rate is 9600.
  Serial.begin(115200);

  //Software Serial for GPS.
  ss.begin(9600);
}

void loop() {

  tempVal = dht.readTemperature();
  Serial.print("Temperature Value : ");
  Serial.println(tempVal);

  gps.f_get_position(&latVal, &lonVal);


  Serial.print("Latitude : ");
  Serial.print(latVal);

  Serial.print(" ,Longitude : ");
  Serial.println(lonVal);


  smartdelay(1000);
}



static void smartdelay(unsigned long ms) {
  unsigned long start = millis();
  do {
    while (ss.available())
      gps.encode(ss.read());
  } while (millis() - start < ms);
}
