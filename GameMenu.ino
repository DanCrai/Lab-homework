#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int pinX = A0;
const int pinY = A1;
const int butPin = 13;


struct timeCheck{
  int previousTime = 0;
  int currentTime = 0;
  int interval = 1000;
}buttonTimeCheck, levelTimeCheck, livesTimeCheck, blinkTimeCheck;


int xValue = 0;
int yValue = 0;
int butPressed = 0;

int xState = 0;
int yState = 0;

int Highscore = 0;
int curLevel = 0;
int startingLevel = 1;
int curName[] = {'a', 'a', 'a'};
int highscoreName[] = {'a', 'a', 'a'};
int curNameLetter1 = 'a';
int curNameLetter2 = 'a';
int curNameLetter3 = 'a';
int curLetter = 0;
int startingLives = 3;
int curLives = 0;
int curScore = 0;
int settingsButton = 0;

enum gameState
{
  menu,
  newgame,
  highscore,
  settings,
  endgame
};

enum menuButton
{
  newgamebut,
  loadgamebut,
  highscorebut,
  settingsbut
};

enum settingsState
{
  moving,
  selecting,
  changingValue
};

gameState curGameState = menu;
menuButton curButton = newgamebut;
settingsState curSettingsState = moving;



void printIntToDisplay(int val, int cursorPosition1, int cursorPosition2)
{
  int aux = val;
  int nrcifre = 1;
  while(aux > 9)
  {
    nrcifre++;
    cursorPosition1++;
    aux /= 10;
  }
  aux = val;
  while(nrcifre)
  {
    int ultimaCifra = aux%10;
    lcd.setCursor(cursorPosition1, cursorPosition2);
    cursorPosition1--;
    lcd.write(ultimaCifra + '0');
    nrcifre--;
    aux /= 10;
  }
}

bool canButtonBePressed()
{
  buttonTimeCheck.currentTime = millis() % 1000000;
  if(buttonTimeCheck.currentTime < buttonTimeCheck.previousTime)
  {
    buttonTimeCheck.previousTime -= 1000000;
  }
  if(buttonTimeCheck.currentTime - buttonTimeCheck.previousTime > buttonTimeCheck.interval)
  {
    buttonTimeCheck.previousTime = buttonTimeCheck.currentTime;
    return true;
  }
  else
  {
    return false;
  }
}

bool increaseLevel()
{
  levelTimeCheck.currentTime = millis() % 1000000;
  if(levelTimeCheck.currentTime < levelTimeCheck.previousTime)
  {
    levelTimeCheck.previousTime -= 1000000;
  }
  if(levelTimeCheck.currentTime - levelTimeCheck.previousTime > levelTimeCheck.interval)
  {
    levelTimeCheck.previousTime = levelTimeCheck.currentTime;
    return true;
  }
  else
  {
    return false;
  }
}

bool decreaseLife()
{
  livesTimeCheck.currentTime = millis() % 1000000;
  if(livesTimeCheck.currentTime < livesTimeCheck.previousTime)
  {
    livesTimeCheck.previousTime -= 1000000;
  }
  if(livesTimeCheck.currentTime - livesTimeCheck.previousTime > livesTimeCheck.interval)
  {
    livesTimeCheck.previousTime = livesTimeCheck.currentTime;
    return true;
  }
  else
  {
    return false;
  }
}

void createMenu(menuButton curButton)
{
  switch(curButton)
  {
    case(newgamebut):
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Game Menu");
      lcd.setCursor(0, 1);
      lcd.write(">New Game  Load");
      break;
    }
    case(loadgamebut):
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Game Menu");
      lcd.setCursor(0, 1);
      lcd.write(">Load Game  High");
      break;
    }
    case(highscorebut):
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Game Menu");
      lcd.setCursor(0, 1);
      lcd.write(">Highscore Settings");
      break;
    }
    case(settingsbut):
    {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.write("Game Menu");
      lcd.setCursor(0, 1);
      lcd.write(">Settings");
      break;
    }
  }
}

void createNewGame()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("lives:");
  printIntToDisplay(curLives, 6, 0);
  //lcd.setCursor(6, 0);
  //lcd.write(curLives);
  lcd.setCursor(8, 0);
  lcd.write("score:");
  printIntToDisplay(curScore, 14, 0);
  //lcd.setCursor(14, 0);
  //lcd.write(curScore);
  lcd.setCursor(0, 1);
  lcd.write("level:");
  printIntToDisplay(curLevel, 6, 1);
  //lcd.setCursor(6,1);
  //lcd.write(curLevel);
}

void createHighscore()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write("Highscore: ");
  printIntToDisplay(Highscore, 11, 0);
  lcd.setCursor(13, 0);
  lcd.write(highscoreName[0]);
  lcd.setCursor(14, 0);
  lcd.write(highscoreName[1]);
  lcd.setCursor(15, 0);
  lcd.write(highscoreName[2]);
  lcd.setCursor(0, 1);
  lcd.write(">Back to menu");
}

void createSettings()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(">Name: ");
  lcd.setCursor(7, 0);
  lcd.write(curName[0]);
  lcd.setCursor(8, 0);
  lcd.write(curName[1]);
  lcd.setCursor(9, 0);
  lcd.write(curName[2]);
  printIntToDisplay(startingLevel, 13, 1);
  lcd.setCursor(1, 1);
  lcd.write("StartLevel: ");
}


menuButton nextMenuButton(menuButton curButton)
{
  switch(curButton)
  {
    case(newgamebut):
    {
      return loadgamebut;
      break;
    }
    case(loadgamebut):
    {
      return highscorebut;
      break;
    }
    case(highscorebut):
    {
      return settingsbut;
      break;
    }
    case(settingsbut):
    {
      return newgamebut;
      break;
    }
  }
}

menuButton previousMenuButton(menuButton curButton)
{
  switch(curButton)
  {
    case(newgamebut):
    {
      return settingsbut;
      break;
    }
    case(loadgamebut):
    {
      return newgamebut;
      break;
    }
    case(highscorebut):
    {
      return loadgamebut;
      break;
    }
    case(settingsbut):
    {
      return highscorebut;
      break;
    }
  }
}

void changeScene(menuButton curButton)
{
  switch(curButton)
  {
    case(newgamebut):
    {
      livesTimeCheck.previousTime = millis()%1000000;
      levelTimeCheck.previousTime = millis()%1000000;
      curLevel = startingLevel;
      curScore = curLevel * 3;
      curLives = startingLives;
      curGameState = newgame;
      createNewGame();
      break;
    }
    case(loadgamebut):
    {
      curGameState = newgame;
      break;
    }
    case(highscorebut):
    {
      curGameState = highscore;
      createHighscore();
      break;
    }
    case(settingsbut):
    {
      settingsButton = 0;
      curGameState = settings;
      createSettings();
      break;
    }
  }
}

void endGame(int score)
{
  lcd.clear();
  if(score > Highscore)
  {
    Highscore = score;
    highscoreName[0] = curName[0];
    highscoreName[1] = curName[1];
    highscoreName[2] = curName[2];
    lcd.setCursor(0, 0);
    lcd.write("Highscore!");
    printIntToDisplay(Highscore, 12, 0);
  }
  else
  {
    lcd.setCursor(0, 0);
    lcd.write("congratiulations");
  }
  lcd.setCursor(0, 1);
  lcd.write("press to exit");
  curGameState = endgame;
}


void blinkPosition(int cursorPosition1, int cursorPosition2, int val1)
{
  blinkTimeCheck.currentTime = millis()%1000000;
  if(blinkTimeCheck.currentTime < blinkTimeCheck.previousTime)
  {
    blinkTimeCheck.previousTime -= 1000000;
  }
  if(blinkTimeCheck.currentTime - blinkTimeCheck.previousTime > blinkTimeCheck.interval && blinkTimeCheck.currentTime - blinkTimeCheck.previousTime < blinkTimeCheck.interval * 2)
  {
    lcd.setCursor(cursorPosition1, cursorPosition2);
    lcd.write(" ");
  }
  if(blinkTimeCheck.currentTime - blinkTimeCheck.previousTime >= blinkTimeCheck.interval * 2)
  {
    blinkTimeCheck.previousTime = blinkTimeCheck.currentTime;
    lcd.setCursor(cursorPosition1, cursorPosition2);
    if(settingsButton == 0)
    {
      lcd.write(val1);
    }
    else
    {
      lcd.write(val1 + '0');
    }
  }
}

void unblinkPosition(int cursorPosition1, int cursorPosition2, int val1)
{
  lcd.setCursor(cursorPosition1, cursorPosition2);
  if(settingsButton == 0)
  {
    lcd.write(val1);
  }
  else
  {
    lcd.write(val1 + '0');
  }
}


void setup() {
  lcd.begin(16, 2);
  pinMode(butPin, INPUT_PULLUP);
  Serial.begin(9600);
  levelTimeCheck.interval = 5000;
  livesTimeCheck.interval = 10000;
  createMenu(curButton);
}

void loop() {
  switch(curGameState)
  {
    case(menu):
    {
      xValue = analogRead(pinX);
      if(xValue > 700)
      {
        if(xState != 1)
        {
          xState = 1;
          curButton = nextMenuButton(curButton);
          createMenu(curButton);
        }
      }
      else if(xValue < 300)
      {
        if(xState != 2)
        {
          xState = 2;
          curButton = previousMenuButton(curButton);
          createMenu(curButton);
        }
      }
      else
      {
        xState = 0;
      }
      butPressed = digitalRead(butPin);
      if(butPressed == 0 && canButtonBePressed())
      {
        changeScene(curButton);
      }
      break;
    }
    case(newgame):
    {
      if(increaseLevel())
      {
        curLevel++;
        curScore = 3 * curLevel;
        createNewGame();
      }
      
      if(decreaseLife())
      {
        curLives--;
        createNewGame();
        if(curLives <= 0)
        {
          delay(1000);
          endGame(curScore);
          break;
        }
      }
      break;
    }
    case(endgame):
    {
      butPressed = digitalRead(butPin);
      if(butPressed == 0 && canButtonBePressed())
      {
        curGameState = menu;
        createMenu(curButton);
      }
      break;
    }
    case(highscore):
    {
      butPressed = digitalRead(butPin);
      if(butPressed == 0 && canButtonBePressed())
      {
        curGameState = menu;
        createMenu(curButton);
      }
      break;
    }
    case(settings):
    {
      switch(curSettingsState)
      {
        case(moving):
        {
          xValue = analogRead(pinX);
          if(xValue < 300)
          {
            curGameState = menu;
            createMenu(curButton);
          }
          yValue = analogRead(pinY);
          if(yValue < 300)
          {
            if(yState != 1)
            {
              yState = 1;
              lcd.setCursor(0, 0);
              lcd.write(">");
              lcd.setCursor(0, 1);
              lcd.write(" ");
              settingsButton = 0;
            }
          }
          else if(yValue > 700)
          {
            if(yState != 2)
            {
              yState = 2;
              lcd.setCursor(0, 1);
              lcd.write(">");
              lcd.setCursor(0, 0);
              lcd.write(" ");
              settingsButton = 1;
            }
          }
          else
          {
            yState = 0;
          }
          butPressed = digitalRead(butPin);
          if(butPressed == 0 && canButtonBePressed())
          {
            curSettingsState = selecting;
          }
        }
        break;
        case(selecting):
        {
          butPressed = digitalRead(butPin);
          if(settingsButton == 0)
          {
            xValue = analogRead(pinX);
            if(xValue > 700)
            {
              if(xState != 1)
              {
                unblinkPosition(7 + curLetter, 0, curName[curLetter]);
                xState = 1;
                curLetter++;
                if(curLetter >= 3)
                {
                  curLetter = 0;
                }
              }
            }
            else if(xValue < 300)
            {
              if(xState != 2)
              {
                unblinkPosition(7 + curLetter, 0, curName[curLetter]);
                xState = 2;
                curLetter--;
                if(curLetter < 0)
                {
                  curLetter = 2;
                }
              }
            }
            else
            {
              xState = 0;
            }
            blinkPosition(7 + curLetter, 0, curName[curLetter]);
            if(butPressed == 0 && canButtonBePressed())
            {
              unblinkPosition(7 + curLetter, 0, curName[curLetter]);
              curSettingsState = changingValue;
            }
          }
          else
          {
            blinkPosition(13, 1, startingLevel);
            if(butPressed == 0 && canButtonBePressed())
            {
              unblinkPosition(13, 1, startingLevel);
              curSettingsState = changingValue;
            }
          }
          break;
        }
        case(changingValue):
        {
          butPressed = digitalRead(butPin);
          if(settingsButton == 0)
          {
            yValue = analogRead(pinY);
            if(yValue < 300)
            {
              if(yState != 1)
              {
                yState = 1;
                if(curName[curLetter] < 'z')
                {
                  curName[curLetter]++;
                  createSettings();
                }
              }
            }
            else if(yValue > 700)
            {
              if(yState != 2)
              {
                yState = 2;
                if(curName[curLetter] > 'a')
                {
                  curName[curLetter]--;
                  createSettings();
                }
              }
            }
            else
            {
              yState = 0;
            }
            if(butPressed == 0 && canButtonBePressed())
            {
              curSettingsState = moving;
            }
          }
          else
          {
            yValue = analogRead(pinY);
            if(yValue < 300)
            {
              if(yState != 1)
              {
                yState = 1;
                if(startingLevel < 9)
                {
                  startingLevel++;
                  createSettings();
                  lcd.setCursor(0, 0);
                  lcd.write(" ");
                  lcd.setCursor(0, 1);
                  lcd.write(">");
                }
              }
            }
            else if(yValue > 700)
            {
              if(yState != 2)
              {
                yState = 2;
                if(startingLevel > 1)
                {
                  startingLevel--;
                  createSettings();
                  lcd.setCursor(0, 0);
                  lcd.write(" ");
                  lcd.setCursor(0, 1);
                  lcd.write(">");
                }
              }
            }
            else
            {
              yState = 0;
            }
            if(butPressed == 0 && canButtonBePressed())
            {
              curSettingsState = moving;
            }
          }
          break;
        }
      }
      
      butPressed = digitalRead(butPin);
      
      
      break;
    }
  }
}
