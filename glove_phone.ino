#include <SoftwareSerial.h>
#include "Adafruit_FONA.h"

#define FONA_RX 8
#define FONA_TX 9
#define FONA_RST 2

#define FONA_RI_INTERRUPT  0

SoftwareSerial fonaSS = SoftwareSerial(FONA_TX, FONA_RX);

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);

const int callIndexFingerBtn = 4;
const int indexVib = 5;
const int callMiddleFingerBtn = 6;
const int middleVib = 7;
const int callRingFingerBtn = 10;
const int ringVib = 11;
const int endCallBtn = 12;


int indexFingerBtnCounter = 0;
int callIndexFingerBtnState = 0;
int lastCallIndexFingerBtnState = 0;
char indexNum[] = ""; //enter fav number 1, prefix with 1 for us numbers

int middleFingerBtnCounter = 0;
int callMiddleFingerBtnState = 0;
int lastCallMiddleFingerBtnState = 0;
char middleNum[] = "";//enter fav number 2, prefix with 1 for us numbers

int ringFingerBtnCounter = 0;
int callRingFingerBtnState = 0;
int lastCallRingFingerBtnState = 0;
char ringNum[] = "";//enter fav number 3, prefix with 1 for us numbers

int endCallCounter = 0;
int endCallBtnState = 0;
int lastEndCallBtnState = 0;

char glcall[32] = "";

boolean incomingCall = false;
long present = millis();
long past = 0;
long interval = 1000;

boolean turnVibOn = false;

void setup() {
  while (!Serial);

  pinMode(callIndexFingerBtn, INPUT);
  pinMode(callMiddleFingerBtn, INPUT);
  pinMode(callRingFingerBtn, INPUT);
  pinMode(endCallBtn, INPUT);

  pinMode(indexVib, OUTPUT);
  pinMode(middleVib, OUTPUT);
  pinMode(ringVib, OUTPUT);

  Serial.begin(115200);

  fonaSS.begin(4800); // if you're using software serial
//  
//  if(! fona.setAudio(FONA_EXTAUDIO)) {
//        Serial.println(F("Failed"));
//      } else {
//        Serial.println(F("OK!"));
//      }

     // fona.setMicVolume(FONA_EXTAUDIO, 10);
     
  // See if the FONA is responding
  if (! fona.begin(fonaSS)) {           
    Serial.println(F("Couldn't find FONA"));
    while (1);
  }

  //Enable caller ID
  if (fona.callerIdNotification(true, FONA_RI_INTERRUPT)) {
    Serial.println(F("Caller id notification enabled"));
  } else {
    Serial.println(F("Caller id notification disabled"));
  }

}

void loop() {
  present = millis();
  char phone[32] = {0};

  if (fona.incomingCallNumber(phone)) {
    strcpy(glcall, phone);
    turnVibOn = true;
  }


  // RECEIVE INDEX FINGER
  
  callIndexFingerBtnState = digitalRead(callIndexFingerBtn);

  if (strcmp(glcall, indexNum) == 0) {
    if (turnVibOn) {
      digitalWrite(indexVib, HIGH);
    }

    if (callIndexFingerBtnState != lastCallIndexFingerBtnState) {
      if (callIndexFingerBtnState == HIGH) {
        turnVibOn = false;
        digitalWrite(indexVib, LOW);
        if (! fona.pickUp()) {
          Serial.println(F("Cant answer index finger"));
        } else {
          Serial.println(F("answered index finger"));
        }
      } else {
        Serial.println("released btn to answer index finger");
      }
    }
  }

  //CALL INDEX FINGER

  if (strcmp(glcall, "") == 0) {
    if (callIndexFingerBtnState != lastCallIndexFingerBtnState) {
      if (callIndexFingerBtnState == HIGH) {
        indexFingerBtnCounter++;
        past = present;
      } else {
        Serial.println("released btn to call index finger");
      }
    }
  }
  
  if (present - past > interval){
  indexFingerBtnCounter = 0;
  }

  lastCallIndexFingerBtnState = callIndexFingerBtnState;
  

  if (indexFingerBtnCounter >= 2) {
    indexFingerBtnCounter = 0;
    if (!fona.callPhone(indexNum)) {
      Serial.println(F("cant call index finger"));
    } else {
      Serial.println(F("calling index finger"));
    }
  }

  // RECEIVE MIDDLE FINGER

  callMiddleFingerBtnState = digitalRead(callMiddleFingerBtn);

  if (strcmp(glcall, middleNum) == 0) {
    if (turnVibOn) {
      digitalWrite(middleVib, HIGH);
    }
    if (callMiddleFingerBtnState != lastCallMiddleFingerBtnState) {
      if (callMiddleFingerBtnState == HIGH) {
        digitalWrite(middleVib, LOW);
        turnVibOn = false;
        if (! fona.pickUp()) {
          Serial.println(F("Cant answer Benny"));
        } else {
          Serial.println(F("answered benny"));
        }
      } else {
        Serial.println("released btn to answer benny");
      }
    }
  }

  // CALL MIDDLE FINGER

  if (strcmp(glcall, "") == 0) {
    if (callMiddleFingerBtnState != lastCallMiddleFingerBtnState) {
      if (callMiddleFingerBtnState == HIGH) {
        middleFingerBtnCounter++;
        past = present;
      } else {
        Serial.println("released btn to call middle finger");
      }

    }
  }
  
   if (present - past > interval){
      middleFingerBtnCounter = 0;
    }
  lastCallMiddleFingerBtnState = callMiddleFingerBtnState;

  if (middleFingerBtnCounter >=2) {
    middleFingerBtnCounter = 0;
    if (!fona.callPhone(middleNum)) {
      Serial.println(F("cant call middle finger"));
    } else {
      Serial.println(F("calling middle finger"));
    }
  }

  // RECEIVE RING FINGER

  callRingFingerBtnState = digitalRead(callRingFingerBtn);

  if (strcmp(glcall, ringNum) == 0) {
    if (turnVibOn) {
      digitalWrite(ringVib, HIGH);
    }
    if (callRingFingerBtnState != lastCallRingFingerBtnState) {
      if (callRingFingerBtnState == HIGH) {
        turnVibOn = false;
        digitalWrite(ringVib, LOW);
        if (! fona.pickUp()) {
          Serial.println(F("Cant answer ring finger"));
        } else {
          Serial.println(F("answered ring finger"));
        }
      } else {
        Serial.println("released btn to answer ring");
      }
    }
  }


  //CALL RING FINGER

  if (strcmp(glcall, "") == 0) {
    if (callRingFingerBtnState != lastCallRingFingerBtnState) {
      if (callRingFingerBtnState == HIGH) {
        ringFingerBtnCounter++;
        past = present;
        //        if (!fona.callPhone(ringNum)) {
        //          Serial.println(F("cant call ring finger"));
        //        } else {
        //          Serial.println(F("calling ring finger"));
        //        }
      } else {
        Serial.println("released btn to call ring");
      }

    }
  }
  if (present - past > interval){
      ringFingerBtnCounter = 0;
    }

  lastCallRingFingerBtnState = callRingFingerBtnState;

  if (ringFingerBtnCounter >= 2 ) {
    ringFingerBtnCounter = 0;
    if (!fona.callPhone(ringNum)) {
      Serial.println(F("cant call ring finger"));
    } else {
      Serial.println(F("calling ring finger"));
    }
  }


  // END CALL

  endCallBtnState = digitalRead(endCallBtn);

  if (endCallBtnState != lastEndCallBtnState) {
    if (endCallBtnState == HIGH) {
      digitalWrite(indexVib, LOW);
      digitalWrite(middleVib, LOW);
      digitalWrite(ringVib, LOW);
      if (! fona.hangUp()) {
        Serial.println(F("Failed to end call"));
      } else {
        Serial.println(F("Ended call"));
        strcpy(glcall, "");
      }
    } else {
      Serial.println("released btn to end call");
    }

  }

  lastEndCallBtnState = endCallBtnState;

  // flush input
  flushSerial();

}

void flushSerial() {
  while (Serial.available())
    Serial.read();
}


