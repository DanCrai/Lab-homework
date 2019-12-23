#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"
#include "enemyClass.h"
#include "playerClass.h"
#include "bossClass.h"

const int RS = 13;
const int enable = 3;
const int d4 = 4;
const int d5 = 5;
const int d6 = 6;
const int d7 = 7;

LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);
LedControl lc = LedControl(12, 11, 10, 1);

int xPin = A0;
int yPin = A1;
int xValue = 0;
int yValue = 0;
int xState = 0;
int yState = 0;
int curRow = 0;
int curCol = 0;
int pinMatrix[8][8] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
};

Player *player;
int nrOfEnemies = 4;
int nrOfEnemiesAlive;
enemy *enemies[4];
boss *Boss;
bool win = false;
bool lose = false;
bool draw = true;
int nrWalls = 10;
struct wall
{
  int x1;
  int x2;
  int y1;
  int y2;
} walls[10][10];

const int pinX = A0;
const int pinY = A1;
const int butPin = 2;


struct timeCheck {
  int previousTime = 0;
  int currentTime = 0;
  int interval = 1000;
} buttonTimeCheck, levelTimeCheck, livesTimeCheck, blinkTimeCheck;


int butPressed = 0;



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
  while (aux > 9)
  {
    nrcifre++;
    cursorPosition1++;
    aux /= 10;
  }
  aux = val;
  while (nrcifre)
  {
    int ultimaCifra = aux % 10;
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
  if (buttonTimeCheck.currentTime < buttonTimeCheck.previousTime)
  {
    buttonTimeCheck.previousTime -= 1000000;
  }
  if (buttonTimeCheck.currentTime - buttonTimeCheck.previousTime > buttonTimeCheck.interval)
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
  if (levelTimeCheck.currentTime < levelTimeCheck.previousTime)
  {
    levelTimeCheck.previousTime -= 1000000;
  }
  if (levelTimeCheck.currentTime - levelTimeCheck.previousTime > levelTimeCheck.interval)
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
  if (livesTimeCheck.currentTime < livesTimeCheck.previousTime)
  {
    livesTimeCheck.previousTime -= 1000000;
  }
  if (livesTimeCheck.currentTime - livesTimeCheck.previousTime > livesTimeCheck.interval)
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
  switch (curButton)
  {
    case (newgamebut):
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("Game Menu");
        lcd.setCursor(0, 1);
        lcd.write(">New Game  Load");
        break;
      }
    case (loadgamebut):
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("Game Menu");
        lcd.setCursor(0, 1);
        lcd.write(">Load Game  High");
        break;
      }
    case (highscorebut):
      {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.write("Game Menu");
        lcd.setCursor(0, 1);
        lcd.write(">Highscore Settings");
        break;
      }
    case (settingsbut):
      {
        lcd.clear();
        lcd.setCursor(0, 0);
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
  switch (curButton)
  {
    case (newgamebut):
      {
        return loadgamebut;
        break;
      }
    case (loadgamebut):
      {
        return highscorebut;
        break;
      }
    case (highscorebut):
      {
        return settingsbut;
        break;
      }
    case (settingsbut):
      {
        return newgamebut;
        break;
      }
  }
}

menuButton previousMenuButton(menuButton curButton)
{
  switch (curButton)
  {
    case (newgamebut):
      {
        return settingsbut;
        break;
      }
    case (loadgamebut):
      {
        return newgamebut;
        break;
      }
    case (highscorebut):
      {
        return loadgamebut;
        break;
      }
    case (settingsbut):
      {
        return highscorebut;
        break;
      }
  }
}

void changeScene(menuButton curButton)
{
  switch (curButton)
  {
    case (newgamebut):
      {
        livesTimeCheck.previousTime = millis() % 1000000;
        levelTimeCheck.previousTime = millis() % 1000000;
        curLevel = startingLevel;
        curScore = 0;
        curLives = startingLives;
        setupLevel(curLevel);
        curGameState = newgame;
        createNewGame();
        break;
      }
    case (loadgamebut):
      {
        curGameState = newgame;
        break;
      }
    case (highscorebut):
      {
        curGameState = highscore;
        createHighscore();
        break;
      }
    case (settingsbut):
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
  if (score > Highscore)
  {
    Highscore = score;
    EEPROM.write(0, Highscore);
    highscoreName[0] = curName[0];
    highscoreName[1] = curName[1];
    highscoreName[2] = curName[2];
    EEPROM.write(1, highscoreName[0]);
    EEPROM.write(2, highscoreName[1]);
    EEPROM.write(3, highscoreName[2]);
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
  blinkTimeCheck.currentTime = millis() % 1000000;
  if (blinkTimeCheck.currentTime < blinkTimeCheck.previousTime)
  {
    blinkTimeCheck.previousTime -= 1000000;
  }
  if (blinkTimeCheck.currentTime - blinkTimeCheck.previousTime > blinkTimeCheck.interval && blinkTimeCheck.currentTime - blinkTimeCheck.previousTime < blinkTimeCheck.interval * 2)
  {
    lcd.setCursor(cursorPosition1, cursorPosition2);
    lcd.write(" ");
  }
  if (blinkTimeCheck.currentTime - blinkTimeCheck.previousTime >= blinkTimeCheck.interval * 2)
  {
    blinkTimeCheck.previousTime = blinkTimeCheck.currentTime;
    lcd.setCursor(cursorPosition1, cursorPosition2);
    if (settingsButton == 0)
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
  if (settingsButton == 0)
  {
    lcd.write(val1);
  }
  else
  {
    lcd.write(val1 + '0');
  }
}


void setupLevel(int lvl) {
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);
  //Serial.begin(9600);
  win = false;
  lose = false;
  if(lvl >= 10) 
    {
      endGame(curScore);
      createNewGame();
    }
  nrOfEnemiesAlive = nrOfEnemies;
  for (int i = 0; i < 8; i++)
    for (int j = 0; j < 8; j++)
    {
      pinMatrix[i][j] = 0;
    }
  for (int i = 0; i < nrWalls; i++)
  {
    for (int row = walls[lvl - 1][i].x1; row <= walls[lvl - 1][i].x2; row++)
      for (int col = walls[lvl - 1][i].y1; col <= walls[lvl - 1][i].y2; col++)
      {
        pinMatrix[col][row] = 1;
        drawWall(walls[lvl - 1][i]);
      }
  }
  if (player)
  {
    delete(player);
  }
  player = new Player();
  drawPlayer(*player);
  //if (lvl != 10)
  //{
    for (int i = 0; i < nrOfEnemies; i++)
    {
      if (enemies[i])
      {
        delete(enemies[i]);
      }
    }
    enemies[0] = new enemy(1, 7, 0, false);
    pinMatrix[enemies[0]->curPositionX][enemies[0]->curPositionY] = 2;
    drawEnemy(*enemies[0]);

    enemies[1] = new enemy(1, 5, 0, false);
    pinMatrix[enemies[1]->curPositionX][enemies[1]->curPositionY] = 2;
    drawEnemy(*enemies[1]);

    enemies[2] = new enemy(1, 0, 0, true);
    pinMatrix[enemies[2]->curPositionX][enemies[2]->curPositionY] = 2;
    drawEnemy(*enemies[2]);

    enemies[3] = new enemy(1, 2, 0, true);
    pinMatrix[enemies[3]->curPositionX][enemies[3]->curPositionY] = 2;
    drawEnemy(*enemies[3]);
  //}
  //else
  //{
  //  Boss = new boss(4, 3, 0, false);
  //  for(int i = 0; i < Boss->lengthVal; i++)
  //  {
  //    pinMatrix[Boss->curPositionX[i]][Boss->curPositionY[i]] = 2;
  //  }
  //  drawBoss(*Boss);
  //}
}

void drawWall(wall w)
{
  for (int row = w.x1; row <= w.x2; row++)
    for (int col = w.y1; col <= w.y2; col++)
    {
      lc.setLed(0, col, row, true);
    }
}
void drawPlayer(Player p)
{
  lc.setLed(0, p.curPositionX, p.curPositionY, true);
}
void drawEnemy(enemy en)
{
  lc.setLed(0, en.curPositionX, en.curPositionY, true);
}
void drawBoss(boss b)
{
  for(int i = 0; i < b.lengthVal; i++)
  {
    lc.setLed(0, b.curPositionX[i], b.curPositionY[i], true);
  }
}
void erasePlayer(Player p)
{
  lc.setLed(0, p.curPositionX, p.curPositionY, false);
}
void eraseEnemy(enemy en)
{
  lc.setLed(0, en.curPositionX, en.curPositionY, false);
}
void eraseBoss(boss b)
{
  for(int i = 0; i < b.lengthVal; i++)
  {
    lc.setLed(0, b.curPositionX[i], b.curPositionY[i], false);
  }
}
bool blinkPosition(int x, int y, int interval, int *prTime, bool ledState)
{
  //Serial.print(ledState);
  int crTime = millis() % 1000000;
  if (crTime < *prTime)
  {
    *prTime -= 1000000;
  }
  if (crTime - *prTime >= interval)
  {
    *prTime = crTime;
    lc.setLed(0, x, y, ledState);
  }
  return !ledState;
}
enemy *getEnemy(int x, int y)
{
  for (int i = 0; i < nrOfEnemies; i++)
  {
    if (enemies[i]->curPositionX == x && enemies[i]->curPositionY)
    {
      return enemies[i];
    }
  }
}


void setup() {
  //EEPROM.write(0, 0);
  lcd.begin(16, 2);
  pinMode(butPin, INPUT_PULLUP);
  Serial.begin(9600);
  levelTimeCheck.interval = 5000;
  livesTimeCheck.interval = 1000;
  Highscore = EEPROM.read(0);
  highscoreName[0] = EEPROM.read(1);
  highscoreName[1] = EEPROM.read(2);
  highscoreName[2] = EEPROM.read(3);
  createMenu(curButton);
  walls[0][0].x1 = 1;
  walls[0][0].x2 = 1;
  walls[0][0].y1 = 0;
  walls[0][0].y2 = 2;
  walls[0][1].x1 = 1;
  walls[0][1].x2 = 1;
  walls[0][1].y1 = 5;
  walls[0][1].y2 = 7;
  walls[0][2].x1 = 3;
  walls[0][2].x2 = 3;
  walls[0][2].y1 = 1;
  walls[0][2].y2 = 6;
  walls[0][3].x1 = 5;
  walls[0][3].x2 = 5;
  walls[0][3].y1 = 0;
  walls[0][3].y2 = 2;
  walls[0][4].x1 = 5;
  walls[0][4].x2 = 5;
  walls[0][4].y1 = 5;
  walls[0][4].y2 = 7;
  walls[0][5].x1 = 7;
  walls[0][5].x2 = 7;
  walls[0][5].y1 = 0;
  walls[0][5].y2 = 7;


  walls[1][0].x1 = 1;
  walls[1][0].x2 = 1;
  walls[1][0].y1 = 0;
  walls[1][0].y2 = 2;
  walls[1][1].x1 = 1;
  walls[1][1].x2 = 1;
  walls[1][1].y1 = 5;
  walls[1][1].y2 = 7;
  walls[1][2].x1 = 3;
  walls[1][2].x2 = 3;
  walls[1][2].y1 = 2;
  walls[1][2].y2 = 5;
  walls[1][3].x1 = 5;
  walls[1][3].x2 = 5;
  walls[1][3].y1 = 0;
  walls[1][3].y2 = 1;
  walls[1][4].x1 = 5;
  walls[1][4].x2 = 5;
  walls[1][4].y1 = 6;
  walls[1][4].y2 = 7;
  walls[1][5].x1 = 5;
  walls[1][5].x2 = 5;
  walls[1][5].y1 = 3;
  walls[1][5].y2 = 4;
  walls[1][6].x1 = 7;
  walls[1][6].x2 = 7;
  walls[1][6].y1 = 0;
  walls[1][6].y2 = 7;


  walls[2][0].x1 = 1;
  walls[2][0].x2 = 1;
  walls[2][0].y1 = 0;
  walls[2][0].y2 = 2;
  walls[2][1].x1 = 1;
  walls[2][1].x2 = 1;
  walls[2][1].y1 = 5;
  walls[2][1].y2 = 7;
  walls[2][2].x1 = 3;
  walls[2][2].x2 = 3;
  walls[2][2].y1 = 2;
  walls[2][2].y2 = 5;
  walls[2][3].x1 = 5;
  walls[2][3].x2 = 5;
  walls[2][3].y1 = 1;
  walls[2][3].y2 = 6;
  walls[2][4].x1 = 7;
  walls[2][4].x2 = 7;
  walls[2][4].y1 = 0;
  walls[2][4].y2 = 7;

  walls[3][0].x1 = 1;
  walls[3][0].x2 = 1;
  walls[3][0].y1 = 0;
  walls[3][0].y2 = 1;
  walls[3][1].x1 = 1;
  walls[3][1].x2 = 1;
  walls[3][1].y1 = 6;
  walls[3][1].y2 = 7;
  walls[3][2].x1 = 3;
  walls[3][2].x2 = 3;
  walls[3][2].y1 = 1;
  walls[3][2].y2 = 2;
  walls[3][3].x1 = 3;
  walls[3][3].x2 = 3;
  walls[3][3].y1 = 5;
  walls[3][3].y2 = 6;
  walls[3][4].x1 = 5;
  walls[3][4].x2 = 5;
  walls[3][4].y1 = 0;
  walls[3][4].y2 = 1;
  walls[3][5].x1 = 5;
  walls[3][5].x2 = 5;
  walls[3][5].y1 = 6;
  walls[3][5].y2 = 7;
  walls[3][6].x1 = 5;
  walls[3][6].x2 = 5;
  walls[3][6].y1 = 3;
  walls[3][6].y2 = 4;
  walls[3][7].x1 = 7;
  walls[3][7].x2 = 7;
  walls[3][7].y1 = 0;
  walls[3][7].y2 = 7;

  walls[4][0].x1 = 0;
  walls[4][0].x2 = 7;
  walls[4][0].y1 = 3;
  walls[4][0].y2 = 4;
  walls[4][1].x1 = 1;
  walls[4][1].x2 = 1;
  walls[4][1].y1 = 0;
  walls[4][1].y2 = 1;
  walls[4][2].x1 = 1;
  walls[4][2].x2 = 1;
  walls[4][2].y1 = 6;
  walls[4][2].y2 = 7;
  walls[4][3].x1 = 3;
  walls[4][3].x2 = 3;
  walls[4][3].y1 = 5;
  walls[4][3].y2 = 6;
  walls[4][4].x1 = 3;
  walls[4][4].x2 = 3;
  walls[4][4].y1 = 1;
  walls[4][4].y2 = 2;
  walls[4][5].x1 = 5;
  walls[4][5].x2 = 5;
  walls[4][5].y1 = 0;
  walls[4][5].y2 = 1;
  walls[4][6].x1 = 5;
  walls[4][6].x2 = 5;
  walls[4][6].y1 = 6;
  walls[4][6].y2 = 7;
  walls[4][7].x1 = 7;
  walls[4][7].x2 = 7;
  walls[4][7].y1 = 0;
  walls[4][7].y2 = 7;

  walls[5][0].x1 = 1;
  walls[5][0].x2 = 1;
  walls[5][0].y1 = 0;
  walls[5][0].y2 = 1;
  walls[5][1].x1 = 1;
  walls[5][1].x2 = 1;
  walls[5][1].y1 = 6;
  walls[5][1].y2 = 7;
  walls[5][2].x1 = 3;
  walls[5][2].x2 = 3;
  walls[5][2].y1 = 1;
  walls[5][2].y2 = 2;
  walls[5][3].x1 = 3;
  walls[5][3].x2 = 3;
  walls[5][3].y1 = 5;
  walls[5][3].y2 = 6;
  walls[5][4].x1 = 5;
  walls[5][4].x2 = 5;
  walls[5][4].y1 = 0;
  walls[5][4].y2 = 1;
  walls[5][5].x1 = 5;
  walls[5][5].x2 = 5;
  walls[5][5].y1 = 6;
  walls[5][5].y2 = 7;
  walls[5][6].x1 = 7;
  walls[5][6].x2 = 7;
  walls[5][6].y1 = 0;
  walls[5][6].y2 = 7;

  walls[6][0].x1 = 1;
  walls[6][0].x2 = 1;
  walls[6][0].y1 = 0;
  walls[6][0].y2 = 2;
  walls[6][1].x1 = 1;
  walls[6][1].x2 = 1;
  walls[6][1].y1 = 5;
  walls[6][1].y2 = 7;
  walls[6][2].x1 = 3;
  walls[6][2].x2 = 3;
  walls[6][2].y1 = 3;
  walls[6][2].y2 = 4;
  walls[6][3].x1 = 3;
  walls[6][3].x2 = 5;
  walls[6][3].y1 = 6;
  walls[6][3].y2 = 6;
  walls[6][4].x1 = 3;
  walls[6][4].x2 = 5;
  walls[6][4].y1 = 1;
  walls[6][4].y2 = 1;
  walls[6][5].x1 = 5;
  walls[6][5].x2 = 5;
  walls[6][5].y1 = 1;
  walls[6][5].y2 = 2;
  walls[6][6].x1 = 5;
  walls[6][6].x2 = 5;
  walls[6][6].y1 = 5;
  walls[6][6].y2 = 6;
  walls[6][7].x1 = 7;
  walls[6][7].x2 = 7;
  walls[6][7].y1 = 0;
  walls[6][7].y2 = 7;

  walls[7][0].x1 = 1;
  walls[7][0].x2 = 1;
  walls[7][0].y1 = 0;
  walls[7][0].y2 = 2;
  walls[7][1].x1 = 1;
  walls[7][1].x2 = 1;
  walls[7][1].y1 = 5;
  walls[7][1].y2 = 7;
  walls[7][2].x1 = 3;
  walls[7][2].x2 = 3;
  walls[7][2].y1 = 2;
  walls[7][2].y2 = 5;
  walls[7][3].x1 = 3;
  walls[7][3].x2 = 5;
  walls[7][3].y1 = 7;
  walls[7][3].y2 = 7;
  walls[7][4].x1 = 3;
  walls[7][4].x2 = 5;
  walls[7][4].y1 = 0;
  walls[7][4].y2 = 0;
  walls[7][5].x1 = 5;
  walls[7][5].x2 = 5;
  walls[7][5].y1 = 1;
  walls[7][5].y2 = 2;
  walls[7][6].x1 = 5;
  walls[7][6].x2 = 5;
  walls[7][6].y1 = 5;
  walls[7][6].y2 = 6;
  walls[7][7].x1 = 7;
  walls[7][7].x2 = 7;
  walls[7][7].y1 = 0;
  walls[7][7].y2 = 7;

  walls[8][0].x1 = 1;
  walls[8][0].x2 = 1;
  walls[8][0].y1 = 0;
  walls[8][0].y2 = 1;
  walls[8][1].x1 = 1;
  walls[8][1].x2 = 1;
  walls[8][1].y1 = 7;
  walls[8][1].y2 = 7;
  walls[8][2].x1 = 3;
  walls[8][2].x2 = 3;
  walls[8][2].y1 = 1;
  walls[8][2].y2 = 2;
  walls[8][3].x1 = 3;
  walls[8][3].x2 = 3;
  walls[8][3].y1 = 5;
  walls[8][3].y2 = 6;
  walls[8][4].x1 = 5;
  walls[8][4].x2 = 5;
  walls[8][4].y1 = 1;
  walls[8][4].y2 = 6;
  walls[8][5].x1 = 7;
  walls[8][5].x2 = 7;
  walls[8][5].y1 = 0;
  walls[8][5].y2 = 7;

  walls[9][0].x1 = 1;
  walls[9][0].x2 = 1;
  walls[9][0].y1 = 0;
  walls[9][0].y2 = 2;
  walls[9][1].x1 = 1;
  walls[9][1].x2 = 1;
  walls[9][1].y1 = 5;
  walls[9][1].y2 = 7;
  walls[9][2].x1 = 3;
  walls[9][2].x2 = 3;
  walls[9][2].y1 = 2;
  walls[9][2].y2 = 5;
  walls[9][3].x1 = 5;
  walls[9][3].x2 = 5;
  walls[9][3].y1 = 0;
  walls[9][3].y2 = 2;
  walls[9][4].x1 = 5;
  walls[9][4].x2 = 5;
  walls[9][4].y1 = 5;
  walls[9][4].y2 = 7;
  walls[9][5].x1 = 7;
  walls[9][5].x2 = 7;
  walls[9][5].y1 = 0;
  walls[9][5].y2 = 7;
}

void loop() {
  Serial.println(curGameState);
  switch (curGameState)
  {
    case (menu):
      {
        xValue = analogRead(pinX);
        if (xValue > 700)
        {
          if (xState != 1)
          {
            xState = 1;
            curButton = nextMenuButton(curButton);
            createMenu(curButton);
          }
        }
        else if (xValue < 300)
        {
          if (xState != 2)
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
        if (butPressed == 0 && canButtonBePressed())
        {
          changeScene(curButton);
        }
        break;
      }
    case (newgame):
      {
        if (nrOfEnemiesAlive <= 0)
        {
          win = true;
          if (curLevel < 10)
          {
            curLevel++;
            setupLevel(curLevel);
            createNewGame();
          }
          else
          {
            endGame(curScore);
          }
        }
        //Serial.println(curLives);
        if (!win && !lose) {
          for (int i = 0; i < nrOfEnemies; i++)
          {
            if (enemies[i]->active) {
              if (enemies[i]->alive) {
                if (pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY + 1] != 1)
                {
                  enemies[i]->setFalling(true);
                }
                else
                {
                  enemies[i]->setFalling(false);
                }
                if (enemies[i]->isFalling())
                {
                  pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 0;
                  eraseEnemy(*enemies[i]);
                  enemies[i]->fall();
                  drawEnemy(*enemies[i]);
                  pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 2;
                }
                if (enemies[i]->walkingDirection)
                {
                  if (enemies[i]->curPositionX >= 7)
                  {
                    if (pinMatrix[0][enemies[i]->curPositionY] == 0 || enemies[i]->curPositionY == 6)
                    {
                      pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 0;
                      eraseEnemy(*enemies[i]);
                      enemies[i]->movePosition();
                      drawEnemy(*enemies[i]);
                      pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 2;
                    }
                    else
                    {
                      enemies[i]->changeDirection();
                    }
                  }
                  else if (pinMatrix[enemies[i]->curPositionX + 1][enemies[i]->curPositionY] == 0)
                  {
                    pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 0;
                    eraseEnemy(*enemies[i]);
                    enemies[i]->movePosition();
                    drawEnemy(*enemies[i]);
                    pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 2;
                  }
                  else
                  {
                    enemies[i]->changeDirection();
                  }
                }
                else
                {
                  if (enemies[i]->curPositionX <= 0)
                  {
                    if (pinMatrix[7][enemies[i]->curPositionY] == 0 || enemies[i]->curPositionY == 6)
                    {
                      pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 0;
                      eraseEnemy(*enemies[i]);
                      enemies[i]->movePosition();
                      drawEnemy(*enemies[i]);
                      pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 2;
                    }
                    else
                    {
                      enemies[i]->changeDirection();
                    }
                  }
                  else if (pinMatrix[enemies[i]->curPositionX - 1][enemies[i]->curPositionY] == 0)
                  {
                    pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 0;
                    eraseEnemy(*enemies[i]);
                    enemies[i]->movePosition();
                    drawEnemy(*enemies[i]);
                    pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 2;
                  }
                  else
                  {
                    enemies[i]->changeDirection();
                  }
                }
              }
              else if (enemies[i]->isReviving())
              {
                enemies[i]->ledSt = blinkPosition(enemies[i]->curPositionX, enemies[i]->curPositionY, enemies[i]->reviveTime / 10, enemies[i]->prevTime, enemies[i]->ledSt);
                enemies[i]->attemptRevive();
              }
            }
          }
          if (pinMatrix[player->curPositionX][player->curPositionY] == 2)
          {
            if (getEnemy(player->curPositionX, player->curPositionY)->alive)
            {
              player->die();
              //Serial.println(player->curPositionY);
              if (decreaseLife())
              {
                curLives--;
                createNewGame();
                if (curLives <= 0)
                {
                  Serial.println(curLives);
                  delete(player);
                  for (int i = 0; i < nrOfEnemies; i++)
                  {
                    delete(enemies[i]);
                  }
                  lose = true;
                  delay(1000);
                  endGame(curScore);
                  //break;
                }
              }
              drawPlayer(*player);
            }
            else
            {
              //Serial.println("deadenemy");
              //enemy *curEnemy = getEnemy(player->curPositionX, player->curPositionY);
              //eraseEnemy(*curEnemy);
              //curEnemy->active = false;
              //delete(curEnemy);
              curScore += 40;
              if(curScore % 100 == 0)
              {
                curLives++;
              }
              nrOfEnemiesAlive--;
              createNewGame();
              for (int i = 0; i < nrOfEnemies; i++)
              {
                if (enemies[i]->curPositionX == player->curPositionX && enemies[i]->curPositionY == player->curPositionY)
                {
                  eraseEnemy(*enemies[i]);
                  pinMatrix[enemies[i]->curPositionX][enemies[i]->curPositionY] = 0;
                  enemies[i]->active = false;
                }
              }
            }
          }
          //Serial.println(player->curPositionX);
          //Serial.println(player->curPositionY);
          xValue = analogRead(xPin);
          yValue = analogRead(yPin);
          if (xValue < 300)
          {
            if (player->curPositionX == 7)
            {
              if (pinMatrix[0][player->curPositionY] != 1)
              {
                erasePlayer(*player);
                player->moveLeft();
                drawPlayer(*player);
              }
            }
            else
            {
              if (pinMatrix[player->curPositionX + 1][player->curPositionY] != 1 || player->isReviving())
              {
                if (!player->isReviving())
                  erasePlayer(*player);
                else
                  drawPlayer(*player);
                player->moveLeft();
                drawPlayer(*player);
              }
            }
          }
          else if (xValue > 700)
          {
            if (player->curPositionX == 0)
            {
              if (pinMatrix[7][player->curPositionY] != 1)
              {
                erasePlayer(*player);
                player->moveRight();
                drawPlayer(*player);
              }
            }
            else
            {
              if (pinMatrix[player->curPositionX - 1][player->curPositionY] != 1 || player->isReviving())
              {
                if (!player->isReviving())
                  erasePlayer(*player);
                else
                  drawPlayer(*player);
                player->moveRight();
                drawPlayer(*player);
              }
            }
          }
          if (yValue < 300)
          {
            if (player->canPlJump())
            {
              //Serial.println("ok");
              player->setJumping(true);
              player->setCanJump(false);
            }
          }
          if (!player->isJumping())
          {
            if (pinMatrix[player->curPositionX][player->curPositionY + 1] != 1)
            {
              player->setFalling(true);
            }
          }
          if (player->isDying())
          {
            Serial.println(player->curPositionY);
            if (pinMatrix[player->curPositionX][player->curPositionY] == 0)
            {
              erasePlayer(*player);
            }
            player->die();
            drawPlayer(*player);
          }
          else if (player->isReviving())
          {
            player->ledSt = blinkPosition(player->curPositionX, player->curPositionY, 200, player->prevTime, player->ledSt);
            if (yValue < 300)
            {
              drawPlayer(*player);
              player->revive();
              drawPlayer(*player);
            }
          }
          else if (player->isJumping())
          {
            if (pinMatrix[player->curPositionX][player->curPositionY - 1] != 1)
            {
              erasePlayer(*player);
              player->jump();
              drawPlayer(*player);
            }
            else
            {
              if (pinMatrix[player->curPositionX][player->curPositionY - 2] == 2)
              {
                enemy *curEnemy = getEnemy(player->curPositionX, player->curPositionY - 2);
                //Serial.println(curEnemy->curPositionX);
                curEnemy->die();
              }
              player->setJumping(false);
              player->setFalling(true);
            }
          }
          else if (player->isFalling())
          {
            if (pinMatrix[player->curPositionX][player->curPositionY + 1] != 1)
            {
              erasePlayer(*player);
              player->fall();
              drawPlayer(*player);
            }
            else
            {
              player->setFalling(false);
              player->setCanJump(true);
            }
          }
        }
        /*if(decreaseLife())
          {
          curLives--;
          createNewGame();
          if(curLives <= 0)
          {
            delay(1000);
            endGame(curScore);
            break;
          }
          }*/
        break;
      }
    case (endgame):
      {
        butPressed = digitalRead(butPin);
        if (butPressed == 0 && canButtonBePressed())
        {
          curGameState = menu;
          createMenu(curButton);
        }
        break;
      }
    case (highscore):
      {
        butPressed = digitalRead(butPin);
        if (butPressed == 0 && canButtonBePressed())
        {
          curGameState = menu;
          createMenu(curButton);
        }
        break;
      }
    case (settings):
      {
        switch (curSettingsState)
        {
          case (moving):
            {
              xValue = analogRead(pinX);
              if (xValue < 300)
              {
                curGameState = menu;
                createMenu(curButton);
              }
              yValue = analogRead(pinY);
              if (yValue < 300)
              {
                if (yState != 1)
                {
                  yState = 1;
                  lcd.setCursor(0, 0);
                  lcd.write(">");
                  lcd.setCursor(0, 1);
                  lcd.write(" ");
                  settingsButton = 0;
                }
              }
              else if (yValue > 700)
              {
                if (yState != 2)
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
              if (butPressed == 0 && canButtonBePressed())
              {
                curSettingsState = selecting;
              }
            }
            break;
          case (selecting):
            {
              butPressed = digitalRead(butPin);
              if (settingsButton == 0)
              {
                xValue = analogRead(pinX);
                if (xValue > 700)
                {
                  if (xState != 1)
                  {
                    unblinkPosition(7 + curLetter, 0, curName[curLetter]);
                    xState = 1;
                    curLetter++;
                    if (curLetter >= 3)
                    {
                      curLetter = 0;
                    }
                  }
                }
                else if (xValue < 300)
                {
                  if (xState != 2)
                  {
                    unblinkPosition(7 + curLetter, 0, curName[curLetter]);
                    xState = 2;
                    curLetter--;
                    if (curLetter < 0)
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
                if (butPressed == 0 && canButtonBePressed())
                {
                  unblinkPosition(7 + curLetter, 0, curName[curLetter]);
                  curSettingsState = changingValue;
                }
              }
              else
              {
                blinkPosition(13, 1, startingLevel);
                if (butPressed == 0 && canButtonBePressed())
                {
                  unblinkPosition(13, 1, startingLevel);
                  curSettingsState = changingValue;
                }
              }
              break;
            }
          case (changingValue):
            {
              butPressed = digitalRead(butPin);
              if (settingsButton == 0)
              {
                yValue = analogRead(pinY);
                if (yValue < 300)
                {
                  if (yState != 1)
                  {
                    yState = 1;
                    if (curName[curLetter] < 'z')
                    {
                      curName[curLetter]++;
                      createSettings();
                    }
                  }
                }
                else if (yValue > 700)
                {
                  if (yState != 2)
                  {
                    yState = 2;
                    if (curName[curLetter] > 'a')
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
                if (butPressed == 0 && canButtonBePressed())
                {
                  curSettingsState = moving;
                }
              }
              else
              {
                yValue = analogRead(pinY);
                if (yValue < 300)
                {
                  if (yState != 1)
                  {
                    yState = 1;
                    if (startingLevel < 9)
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
                else if (yValue > 700)
                {
                  if (yState != 2)
                  {
                    yState = 2;
                    if (startingLevel > 1)
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
                if (butPressed == 0 && canButtonBePressed())
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
