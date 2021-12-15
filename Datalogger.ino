// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

#define cardSelect 4

File logfile;
RTC_DS3231 rtc;

const float calibZero = 0.33;
float V, P;

void setup () {
  Serial.begin(57600);
  // Setup RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  // Setup errorled
  pinMode(13, OUTPUT);

  // Setup SD
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  logfile = SD.open("logging.csv", FILE_WRITE);
  if (!logfile) {
    Serial.print("Couldn't create logfile: ");
    Serial.println(logfile);
    error(2);
  }
  

  // Setup analog signals
  analogReference(AR_DEFAULT);
  analogReadResolution(12);
  
}

void loop() {
  DateTime now = rtc.now();
  V = (analogRead(0) * 3.3 / 4095) - calibZero;
  P = V * 16 / 3; //in bar
  Serial.println(V);
  Serial.println(P);

  delay(500);
}

void error(uint8_t errno) {
  //2 = SDcard error, 2 = no pressure error, 
  while(1) {
    uint8_t i;
    for (i=0; i<errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i=errno; i<10; i++) {
      delay(200);
    }
  }
}
