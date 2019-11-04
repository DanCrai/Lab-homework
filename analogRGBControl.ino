const int pinLEDRED = 11;
const int pinLEDGREEN = 10;
const int pinLEDBLUE = 9;
const int pinPOTRED = A0;
const int pinPOTGREEN = A1;
const int pinPOTBLUE = A2;

int valuePOTRED = 0;
int valuePOTGREEN = 0;
int valuePOTBLUE = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(pinLEDRED,OUTPUT);
  pinMode(pinLEDGREEN,OUTPUT);
  pinMode(pinLEDBLUE,OUTPUT);
  pinMode(pinPOTRED,INPUT);
  pinMode(pinPOTGREEN,INPUT);
  pinMode(pinPOTBLUE,INPUT);
  Serial.begin(9600);
}
void colorLED(int r, int g, int b)
{
  analogWrite(pinLEDRED,r);
  analogWrite(pinLEDGREEN,g);
  analogWrite(pinLEDBLUE,b);
}

void loop() {
  // put your main code here, to run repeatedly:
  //red potentionmetre here
  valuePOTRED = analogRead(pinPOTRED);
  valuePOTRED = map(valuePOTRED, 0, 1023, 0, 255);
  Serial.println(valuePOTRED);
  //green potentiometre here
  valuePOTGREEN = analogRead(pinPOTGREEN);
  valuePOTGREEN = map(valuePOTGREEN, 0, 1023, 0, 255);
  Serial.println(valuePOTGREEN);
  //blue potentiometre here
  valuePOTBLUE = analogRead(pinPOTBLUE);
  valuePOTBLUE = map(valuePOTBLUE, 0, 1023, 0, 255);
  Serial.println(valuePOTBLUE);
  colorLED(valuePOTRED,valuePOTGREEN,valuePOTBLUE);
  //colorLED(0,0,0);
}
