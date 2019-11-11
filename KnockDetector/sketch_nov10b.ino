#include "pitches.h"
const int speakerPin = A0;
const int buzzerPin = A1;
const int pushButton = 2;
int buttonState = 0;
int buzzerTone = 1000;
int speakerValue = 0;
int previousTime = 0;
int interval = 5000;
int previousTimeNote = 0;
bool knocked = false;
int melody[] = {
  NOTE_B3, NOTE_D4, NOTE_FS4, 0, NOTE_G4, NOTE_FS4, NOTE_D4, NOTE_B3, 0
};
int noteDurations[] = {
  2, 2, 1, 1, 1, 2, 2, 2, 1
};
int thisNote = 0;
const int treshold = 50;

void setup() {
  pinMode(speakerPin, INPUT);
  pinMode(pushButton, INPUT);
  Serial.begin(9600);
}

void loop() {
  speakerValue = analogRead(speakerPin);
  buttonState = digitalRead(pushButton);
  if(speakerValue >= treshold && knocked==false)
  {
    knocked = true;
    previousTime = millis();
    Serial.println(speakerValue);
  }
  if(knocked)
  {
    if(millis() - previousTime >= interval)
    {
      //playSong();=
      if(knocked){
        int noteDuration = 1000 / noteDurations[thisNote];
        int pauseBetweenNotes = 250 * 1.30;
        if(millis() - previousTimeNote >= pauseBetweenNotes)
        {
          tone(buzzerPin, melody[thisNote], noteDuration);
          thisNote++;
          if(thisNote == 9)
          {
            thisNote = 0;
          }
          previousTimeNote = millis();
        }
        //noTone(2);
      }
    }
    if(buttonState != 0)
    {
      knocked = false;
      noTone(buzzerPin);
    }
  }
}
