#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// Connecting to wifi
const char *ssid     = "Kobberfabrikkens Net";
const char *password = "kobberfabrikken";

const long utcOffsetInSeconds = 3600;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
 
// Initializing the pins
int MotorPin = 2;
int UvPanelRelayOut = 4;
int SensorIn = A0;
int SensorValue;
int OnTime[2] = {16,35};
int OffTime[2] = {16,36};


 
 
int PumpTurnOnValue = 0;
 
void setup() {
 
  // Starting serial monitor
  Serial.begin(9600);
 
  //Controle pins for waterpump 
  pinMode(MotorPin, OUTPUT);
 
  pinMode(UvPanelRelayOut, OUTPUT);
 
  // Input pin for Moisture sensor
  pinMode(SensorIn, INPUT);

  WiFi.begin(ssid, password);

  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
  digitalWrite(LED_BUILTIN, HIGH);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  timeClient.begin();
  timeClient.update();
  Serial.print(daysOfTheWeek[timeClient.getDay()]);
  Serial.print(", ");
  Serial.print(timeClient.getHours());
  Serial.print(":");
  Serial.print(timeClient.getMinutes());
  Serial.print(":");
  Serial.println(timeClient.getSeconds());

}
 
void loop() {
 
  // Mapping the value from the moisture sensor to value between 0 and 100
  SensorValue = map(analogRead(SensorIn), 600, 0, 0, 100);
  Serial.println(SensorValue);
 
  // Turning the pump on if the value drops below 50 percent
  if (SensorValue < PumpTurnOnValue) {

    digitalWrite(MotorPin, HIGH);
    Serial.println("tør");
  
  } else {
  
    digitalWrite(MotorPin, LOW);
    
  }
  
  timeClient.update();

  if(timeClient.getHours() >= OnTime[0] && timeClient.getMinutes() >= OnTime[1] && timeClient.getHours() <= OffTime[0] && timeClient.getMinutes() <= OffTime[1]){
  
    digitalWrite(UvPanelRelayOut, HIGH);
    Serial.println("Tænder uv-panel");
  
  } 
  
  else {
  
    digitalWrite(UvPanelRelayOut, LOW);
  
  }
}
