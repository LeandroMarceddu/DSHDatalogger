// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

#define cardSelect 4;

File logfile;
RTC_DS3231 rtc;

const float calibZero = 0.2;
float V, P;

void setup () {
  Serial.begin(57600);
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  pinMode(13, OUTPUT);

  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
}

void loop() {
  DateTime now = rtc.now();
  V = analogRead(0);
  

}

void error(uint8_t errno) {
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
