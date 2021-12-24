#include "RTClib.h"
#include <SPI.h>
#include <SD.h>

#define cardSelect 4

File logfile;
RTC_DS3231 rtc;

const float calibZero = 0.32;
float V, P;

unsigned long pushFlush = 0;

void setup () {
  Serial.begin(9600);
  // Setup RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    //error(2);
  }
  // Setup errorled
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(0, INPUT);

  // Setup SD
  if (!SD.begin(cardSelect)) {
    Serial.println("Card init. failed!");
    error(2);
  }
  logfile = SD.open("logging.csv", FILE_WRITE);
  if (!logfile) {
    Serial.print("Couldn't create/open logfile: ");
    Serial.println(logfile);
    error(2);
  }

  // Setup analog signals
  analogReference(AR_DEFAULT);
  analogReadResolution(12);

  pushFlush = millis();
}

void loop() {
  DateTime now = rtc.now();
  V = (analogRead(0) * 3.3 / 4095) - calibZero;
  P = V * 16 / 3; //in bar

  Serial.print(now.timestamp());
  Serial.print(",");
  Serial.print(V);
  Serial.print(",");
  Serial.println(P);
  
  logfile.print(now.timestamp());
  logfile.print(",");
  logfile.print(V);
  logfile.print(",");
  logfile.println(P);
  if ((millis() - pushFlush) > 8000)
  {
    pushFlush = millis();
    logfile.close();
    Serial.println("pushFlush");
    logfile = SD.open("logging.csv", FILE_WRITE);
    if (!logfile) {
      Serial.print("Couldn't create/open logfile: ");
      Serial.println(logfile);
      error(2);
    }
  }
  delay(500);
}

void error(uint8_t errno) {
  //2 = SDcard error
  while (1) {
    uint8_t i;
    for (i = 0; i < errno; i++) {
      digitalWrite(13, HIGH);
      delay(100);
      digitalWrite(13, LOW);
      delay(100);
    }
    for (i = errno; i < 10; i++) {
      delay(200);
    }
  }
}
