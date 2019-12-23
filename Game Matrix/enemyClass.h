class enemy{
  private:
  int previousTime;
  int currentTime;
  int interval;
  int previousTimeToJump;
  int currentTimeToJump;
  int intervalToJump;
  bool falling;
  bool reviving;
  public:
  int deathTime;
  bool alive;
  bool active;
  bool ledSt = false;
  int *prevTime;
  int spawnTime;
  int reviveTime;
  int curPositionX;
  int curPositionY;
  bool walkingDirection;
  enemy();
  enemy(int, int, int, bool);
  ~enemy();
  void movePosition();
  void die();
  void attemptRevive();
  void changeDirection();
  bool isFalling();
  void setFalling(bool);
  bool isReviving();
  void setReviving(bool);
  void fall();
};
enemy::enemy()
{
  interval = 250;
  intervalToJump = 100;
  falling = false;
  active = true;
  alive = true;
  spawnTime = 1;
  curPositionX = -1;
  curPositionY = -1;
  walkingDirection = false;
  reviveTime = 5000;
}
enemy::enemy(int spTime, int curPosX, int curPosY, bool walkDir)
{
  interval = 1000;
  intervalToJump = 100;
  reviveTime = 5000;
  falling = false;
  active = true;
  alive = true;
  spawnTime = spTime;
  curPositionX = curPosX;
  curPositionY = curPosY;
  walkingDirection = walkDir;
}
enemy::~enemy()
{
  
}
void enemy::movePosition(){
  if(active)
  {
    currentTime = millis()%1000000;
    if(currentTime < previousTime)
    {
      previousTime -= 1000000;
    }
    if(currentTime - previousTime >= interval)
    {
      previousTime = currentTime;
      if(walkingDirection == false)
      {
        curPositionX--;
        if(curPositionX < 0)
        {
          curPositionX = 7;
          if(curPositionY == 6)
          {
            curPositionY = 0;
          }
        }
      }
      else
      {
        curPositionX++;
        if(curPositionX > 7)
        {
          curPositionX = 0;
          if(curPositionY == 6)
          {
            curPositionY = 0;
          }
        }
      }
    }
  }
}
void enemy::die(){
  alive = false;
  deathTime = millis()%1000000;
}
void enemy::attemptRevive(){
  int curTime = millis()%1000000;
  if(curTime < deathTime)
  {
    deathTime -= 1000000;
  }
  if(curTime - deathTime >= reviveTime)
  {
    alive = true;
  }
}
void enemy::changeDirection(){
  walkingDirection = !walkingDirection;
}
bool enemy::isFalling()
{
  return falling;
}
void enemy::setFalling(bool fall)
{
  falling = fall;
}
bool enemy::isReviving()
{
  return reviving;
}
void enemy::setReviving(bool revive)
{
  reviving = revive;
}
void enemy::fall()
{
  if(!falling)
  {
    falling = true;
  }
  currentTimeToJump = millis()%1000000;
  if(currentTimeToJump < previousTimeToJump)
  {
    previousTimeToJump -= 1000000;
  }
  if(currentTimeToJump - previousTimeToJump >= intervalToJump)
  {
    previousTimeToJump = currentTimeToJump;
    curPositionY += 1;
  }
}
