#include <Wire.h>
#include "RTClib.h"

#define MAIN_SWITCH 6
#define AC_LOAD     7    // Output to Opto Triac pin

unsigned char dimming = 99;  // Dimming level (0-100)

RTC_DS1307 uhrzeit;

void setup() {
  uhrzeit.begin();

  dimming = 99;
  
  // put your setup code here, to run once:
  pinMode(AC_LOAD, OUTPUT);// Set AC Load pin as output
  attachInterrupt(digitalPinToInterrupt(3), zero_crosss_int, RISING);
  
  pinMode(MAIN_SWITCH, OUTPUT);
  digitalWrite(MAIN_SWITCH, HIGH);
  delay(500);
  digitalWrite(MAIN_SWITCH, LOW);

}

void zero_crosss_int()  // function to be fired at the zero crossing to dim the light
{
  // Firing angle calculation : 1 full 50Hz wave =1/50=20ms 
  // Every zerocrossing : (50Hz)-> 10ms (1/2 Cycle) For 60Hz (1/2 Cycle) => 8.33ms 
  // 10ms=10000us
  
  int dimtime = (100*dimming);    // For 60Hz =>65    
  delayMicroseconds(dimtime);    // Off cycle
  digitalWrite(AC_LOAD, HIGH);   // triac firing
  delayMicroseconds(10);         // triac On propogation delay (for 60Hz use 8.33)
  digitalWrite(AC_LOAD, LOW);    // triac Off
}

void loop() {
  DateTime jetzt = uhrzeit.now();
  
  if (jetzt.hour() == 6 && jetzt.minute() == 30) {
    digitalWrite(MAIN_SWITCH, HIGH);
    
    // Sonnenaufgang!
    for (unsigned char i=85;i>1;i--) {
            dimming=i;
            delay(9000);
    }
    
    delay(600000);
    digitalWrite(MAIN_SWITCH, LOW);
  }
  
}

