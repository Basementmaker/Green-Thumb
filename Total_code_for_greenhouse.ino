// DHT-sensor-library - Version: Latest 
#include <DHT.h>
#include <DHT_U.h>

// Ethernet2 - Version: Latest 
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet2.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp2.h>
#include <util.h>

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#define DHTTYPE DHT11
#define DHTPIN  14
#include <CayenneEthernetW5500.h>


//This is the output pin on the Arduino we are using for the solenoid
int solenoidPin = 4;

// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "dkdtm3ta8x";

//setup for the DHT11 Sensor
float h, t, hif;
bool Humidity = true;
bool Temperature = true;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  pinMode(solenoidPin, OUTPUT);  //Sets the pin as an output
  Serial.begin(9600);
  Serial.print("Setup");
  Cayenne.begin(token);

  Temperature = true;
}

void loop()
{
  //Run Cayenne Functions
  Cayenne.run();
}

//pushes humidity

CAYENNE_OUT(V1)
{
  float h = dht.readHumidity();
  Cayenne.virtualWrite(V1, h); //virtual pin
}


//pushes tempature
CAYENNE_OUT(V2)
{
  //Check if read failed and try until success
  do {
    //Read temperature as Fahrenheit
    t = dht.readTemperature(true);

    delay(100);
  } while  (isnan(t));

  Serial.print("Temperature: ");
  Serial.println(t);
  Serial.println(" ");

//Set Temperature to true so we know when to sleep
  Temperature = true;

  //Write to Cayenne Dashboard
  Cayenne.virtualWrite(V2, t);
}

//recives solonoid control

CAYENNE_IN(1)
{
  int currentValue = getValue.asInt();
  if (currentValue == 1)                  // if switch is on
  {
  digitalWrite(solenoidPin, HIGH);        //turns on solonoid
  Serial.println("Solenoid HIGH");
  Serial.println(" ");
  }
  else
  {
  digitalWrite(solenoidPin, LOW);        //turns off solonoid
  Serial.println("Solenoid LOW");
  Serial.println(" ");
  }
}
