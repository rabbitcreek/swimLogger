#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <SPI.h>
#include <SD.h>
#define cardSelect 4

File logfile;
/*
   This sample sketch demonstrates the normal use of a TinyGPS++ (TinyGPSPlus) object.
   It requires the use of SoftwareSerial, and assumes that you have a
   4800-baud serial GPS device hooked up on pins 4(rx) and 3(tx).
*/
static const int RXPin = 10, TXPin = 11;
static const uint32_t GPSBaud = 9600;
float timerNow = 0.0;
// The TinyGPS++ object
TinyGPSPlus gps;

// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);
#define ONE_WIRE_BUS 9

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
void setup()
{
  Serial.begin(9600);
  ss.begin(GPSBaud);
sensors.begin();
if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    
  }
  char filename[15];
  strcpy(filename, "/ANALOG00.TXT");
  for (uint8_t i = 0; i < 100; i++) {
    filename[7] = '0' + i/10;
    filename[8] = '0' + i%10;
    // create if does not exist, do not open existing, write, sync after write
    if (! SD.exists(filename)) {
      break;
    }
  }

  logfile = SD.open(filename, FILE_WRITE);
  if( ! logfile ) {
    Serial.print("Couldnt create "); 
    Serial.println(filename);
    
  }
  Serial.print("Writing to "); 
  Serial.println(filename);

  Serial.println(F("DeviceExample.ino"));
  Serial.println(F("A simple demonstration of TinyGPS++ with an attached GPS module"));
  Serial.print(F("Testing TinyGPS++ library v. ")); Serial.println(TinyGPSPlus::libraryVersion());
  Serial.println(F("by Mikal Hart"));
  Serial.println();
  timerNow = millis();
}

void loop()
{
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
  if( millis() - timerNow > 10000 ){
    timerNow = millis();
    displayInfo();
    displayTemp();
  }
 
}

void displayInfo()
{
  Serial.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    logfile.print(" Lat = ");
    logfile.print(gps.location.lat(),6);
    logfile.print(" Lng = ");
    logfile.print(gps.location.lng(),6);
    logfile.flush();
    Serial.print(gps.location.lat(), 6);
    Serial.print(F(","));
    Serial.print(gps.location.lng(), 6);
    Serial.print(" Speed(mph): ");
    logfile.print(" Speed(mph): ");
    logfile.print(gps.speed.mph());
    Serial.print (gps.speed.mph());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F("  Date/Time: "));
  if (gps.date.isValid())
  {
    logfile.print(" Date: ");
    logfile.print(gps.date.month());
    Serial.print(gps.date.month());
    logfile.print("/");
    Serial.print(F("/"));
    logfile.print(gps.date.day());
    Serial.print(gps.date.day());
    logfile.print("/");
    Serial.print(F("/"));
    logfile.print(gps.date.year());
    Serial.print(gps.date.year());
    logfile.flush();
    
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.print(F(" "));
  if (gps.time.isValid())
  {
    if (gps.time.hour() < 10) Serial.print(F("0"));
    Serial.print(gps.time.hour());
    Serial.print(F(":"));
    if (gps.time.minute() < 10) Serial.print(F("0"));
    Serial.print(gps.time.minute());
    Serial.print(F(":"));
    if (gps.time.second() < 10) Serial.print(F("0"));
    Serial.print(gps.time.second());
    Serial.print(F("."));
    if (gps.time.centisecond() < 10) Serial.print(F("0"));
    Serial.print(gps.time.centisecond());
  }
  else
  {
    Serial.print(F("INVALID"));
  }

  Serial.println();
}
void displayTemp(){
   sensors.requestTemperatures(); 
  Serial.print("Celsius temperature: ");
  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire
  Serial.print(sensors.getTempCByIndex(0)); 
  Serial.print(" - Fahrenheit temperature: ");
  Serial.println(sensors.getTempFByIndex(0));
  logfile.print(" TempF = "); 
  logfile.println(sensors.getTempFByIndex(0));
  logfile.flush();
}

