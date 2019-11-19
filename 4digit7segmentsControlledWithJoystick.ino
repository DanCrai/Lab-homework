const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int pinSW = A2;
const int pinX = A0; // A0 - analog pin connected to X output
const int pinY = A1; // A1 - analog pin connected to Y output
int switchValue;
int xValue = 0;
int yValue = 0;
int digitSelected = 0;

int previousDigitBlinkTime = 0;
int currentDigitBlinkTime = 0;
int blinkInterval = 100;
byte digitState = HIGH;

const int segSize = 8;

const int noOfDisplays = 4;
const int noOfDigits = 10;

int dpState = LOW;

int currentDigit = 0;
int currentXState = 0;
int currentYState = 0;
int currentButtonState = 1;

int currentNumber = 5555;
unsigned long delayCounting = 50;  // incrementing interval
unsigned long lastIncreasing = 0;

// segments array, similar to before
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
// digits array, to switch between them easily
int digits[noOfDisplays] = {
  pinD1, pinD2, pinD3, pinD4
 };   
 
byte digitMatrix[noOfDigits][segSize - 1] = {
// a  b  c  d  e  f  g
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 1, 0, 1, 1}  // 9
};


void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++)
  {
      digitalWrite(segments[i], digitMatrix[digit][i]);
  }

  // write the decimalPoint to DP pin
  digitalWrite(segments[segSize - 1], decimalPoint);
}

// activate the display no. received as param
void showDigit(int num) {
  for (int i = 0; i < noOfDisplays; i++)
  {
    digitalWrite(digits[i], HIGH);
  }
  if(num == currentDigit)
  {
    if(digitState)
      digitalWrite(digits[num], LOW);
    else
      digitalWrite(digits[num], HIGH);
  }
  else
  {
    digitalWrite(digits[num], LOW);
  }
}

void blinkDigit(int num){
  currentDigitBlinkTime = millis()%1000000;
  if(currentDigitBlinkTime < previousDigitBlinkTime)
  {
    previousDigitBlinkTime -= 1000000;
  }
  if(currentDigitBlinkTime - previousDigitBlinkTime >= blinkInterval)
  {
    previousDigitBlinkTime = currentDigitBlinkTime;
    digitState = !digitState;
  }
}
         

void setup() {
  for (int i = 0; i < segSize; i++)
  {
  pinMode(segments[i], OUTPUT);  
  }
  for (int i = 0; i < noOfDisplays; i++)
  {
  pinMode(digits[i], OUTPUT);  
  }
  //pinMode(pinSW, INPUT_PULLUP);
  Serial.begin(9600);
}

void loop() {
  switchValue = analogRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  if(switchValue == 0 && currentButtonState == 1)
  {
    currentButtonState = 0;
    digitSelected = !digitSelected;
  }
  if(switchValue != 0)
  {
    currentButtonState = 1;
  }
  Serial.println(digitSelected);
  int number;
  int digit;
  int lastDigit;

  digit = 0;
  if(digitSelected == 0)
  {
    if(xValue > 800)
    {
      if(currentXState != -1)
      {
        currentXState = -1;
        currentDigit++;
        if(currentDigit > 3)
          currentDigit = 0;
      }
    }
    else if(xValue < 300)
    {
      if(currentXState != 1)
      {
        currentDigit--;
        if(currentDigit < 0)
          currentDigit = 3;
        currentXState = 1;
      }
    }
    else
    {
      currentXState = 0;
    }
  }
  if(digitSelected == 1)
  {
    Serial.println(currentNumber);
    Serial.println(yValue);
    if(yValue < 300)
    {
      if(currentYState != -1)
      {
        currentYState = -1;
        int copyNumber = currentNumber;
        for(int i = 0; i < currentDigit; i++)
          copyNumber = copyNumber / 10;
        int selectedDigit = copyNumber % 10;
        if(selectedDigit == 0)
          selectedDigit = 9;
        else
          selectedDigit = -1;
        for(int i = 0; i < currentDigit; i++)
          selectedDigit *= 10;
        currentNumber += selectedDigit;
      }
    }
    else if(yValue > 800)
    {
      if(currentYState != 1)
      {
        currentYState = 1;
        int copyNumber = currentNumber;
        for(int i = 0; i < currentDigit; i++)
          copyNumber = copyNumber / 10;
        int selectedDigit = copyNumber % 10;
        if(selectedDigit == 9)
          selectedDigit = -9;
        else
          selectedDigit = 1;
        for(int i = 0; i < currentDigit; i++)
          selectedDigit *= 10;
        currentNumber += selectedDigit;
      }
    }
    else
    {
      currentYState = 0;
    }
  }
  int count = 0;
  //Serial.println(currentDigit);
  number = currentNumber;
  while (number != 0) {
    count++;
  lastDigit = number % 10;  // get the last digit
  showDigit(digit);
  if(digit == currentDigit)
    displayNumber(lastDigit, digitSelected);
  else
    displayNumber(lastDigit, LOW);
  blinkDigit(currentDigit);
  // increase this delay to see multiplexing in action. At about 100 it becomes obvious
  delay(5);
  digit++;            // move to next display
  number = number / 10;
  }
  for(int i = count; i < 4; i++)
  {
    lastDigit = 0;
    showDigit(digit);
  if(digit == currentDigit)
    displayNumber(lastDigit, digitSelected);
  else
    displayNumber(lastDigit, LOW);
  blinkDigit(currentDigit);
  // increase this delay to see multiplexing in action. At about 100 it becomes obvious
  delay(5);
  digit++;
  }
  // increment the number
  if (millis() - lastIncreasing >= delayCounting) {
  lastIncreasing = millis();
  }
}
