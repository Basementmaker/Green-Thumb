#include "arduino_secrets.h"
// Ethernet2 - Version: Latest 
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet2.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp2.h>
#include <util.h>

// Cayenne - Version: Latest 


//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#define DHTTYPE DHT11
#define DHTPIN  14
#include <CayenneEthernetW5500.h>
#include <DHT.h>




// Cayenne authentication token. This should be obtained from the Cayenne Dashboard.
char token[] = "SECRET_AUTH";
float h, t, hif;
bool Humidity = true;
bool Temperature = true;

DHT dht(DHTPIN, DHTTYPE);

void setup()
{
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
CAYENNE_OUT(V2){
  Serial.println("Entered Temperature");
  
  //Check if read failed and try until success
  do {
    //Read temperature as Fahrenheit
    t = dht.readTemperature(true);

    delay(100);
  } while  (isnan(t));

  Serial.print("Temperature: ");
  Serial.println(t);

  //Set Temperature to true so we know when to sleep
  Temperature = true;

  //Write to Cayenne Dashboard
  Cayenne.virtualWrite(V2, t);
}

