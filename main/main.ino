//Importing libraries.
#include <DHT.h>;


//Defining Pins.
#define dhtPin 2

//Defining libraries.
#define dhtType DHT22


//Declaring libraries.
DHT dht(dhtPin, dhtType); 


//Declaring variables.
float tempVal;


// Define pin modes (input or output) and serial monitor for debugging. 
// Will run only once.
void setup() {

  //Initializing libraries.
  dht.begin();

  //Baud rate is 9600.
  Serial.begin(9600);

}

void loop() {
  tempVal = dht.readTemperature();
  Serial.print("Temperature Value : ");
   Serial.println(tempVal);

}
