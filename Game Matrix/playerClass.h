class Player
{
  private:
  int previousTimeToMove;
  int currentTimeToMove;
  int intervalToMove;
  int previousTimeToJump;
  int currentTimeToJump;
  int intervalToJump;
  int previousTimeDying;
  int currentTimeDying;
  int intervalDying;
  bool jumping = false;
  bool falling = false;
  bool canJump = true;
  bool dying = false;
  bool reviving = false;
  public:
  bool ledSt = false;
  int *prevTime;
  int curPositionX;
  int curPositionY;
  Player();
  Player(int, int);
  ~Player();
  void moveLeft();
  void moveRight();
  bool canPlJump();
  void setCanJump(bool);
  bool isJumping();
  void setJumping(bool);
  bool isFalling();
  void setFalling(bool);
  bool isDying();
  void setDying(bool);
  bool isReviving();
  void setReviving(bool);
  void jump();
  void fall();
  void die();
  void revive();
};

Player::Player()
{
  intervalToMove = 100;
  intervalToJump = 100;
  intervalDying = 50;
  curPositionX = 3;
  curPositionY = 6;
}
Player::Player(int curPosX, int curPosY)
{
  intervalToMove = 100;
  intervalToJump = 100;
  intervalDying = 50;
  curPositionX = curPosX;
  curPositionY = curPosY;
}
Player::~Player()
{
  
}
void Player::moveRight()
{
  currentTimeToMove = millis()%1000000;
  if(currentTimeToMove < previousTimeToMove)
  {
    previousTimeToMove -= 1000000;
  }
  if(currentTimeToMove - previousTimeToMove >= intervalToMove)
  {
    previousTimeToMove = currentTimeToMove;
    curPositionX -= 1;
    if(curPositionX < 0)
    {
      curPositionX = 7;
    }
  }
}
void Player::moveLeft()
{
  currentTimeToMove = millis()%1000000;
  if(currentTimeToMove < previousTimeToMove)
  {
    previousTimeToMove -= 1000000;
  }
  if(currentTimeToMove - previousTimeToMove >= intervalToMove)
  {
    previousTimeToMove = currentTimeToMove;
    curPositionX += 1;
    if(curPositionX > 7)
    {
      curPositionX = 0;
    }
  }
}
bool Player::canPlJump()
{
  return canJump;
}
void Player::setCanJump(bool canJ)
{
  canJump = canJ;
}
bool Player::isJumping()
{
  return jumping;
}
void Player::setJumping(bool jmp)
{
  jumping = jmp;
}
bool Player::isFalling()
{
  return falling;
}
void Player::setFalling(bool fall)
{
  falling = fall;
}
bool Player::isDying()
{
  return dying;
}
void Player::setDying(bool die)
{
  dying = die;
}
bool Player::isReviving()
{
  return reviving;
}
void Player::setReviving(bool rev)
{
  reviving = rev;
}
void Player::jump()
{
  if(!jumping)
  {
    jumping = true;
  }
  currentTimeToJump = millis()%1000000;
  if(currentTimeToJump < previousTimeToJump)
  {
    previousTimeToJump -= 1000000;
  }
  if(currentTimeToJump - previousTimeToJump >= intervalToJump)
  {
    previousTimeToJump = currentTimeToJump;
    curPositionY -= 1;
  }
}
void Player::fall()
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
void Player::die()
{
  jumping = false;
  falling = false;
  canJump = false;
  if(!dying)
  {
    dying = true;
    currentTimeDying = millis()%1000000;
    if(currentTimeDying < previousTimeDying)
    {
      previousTimeDying -= 1000000;
    }
    if(currentTimeDying - previousTimeDying >= intervalDying)
    {
      previousTimeDying = currentTimeDying;
      curPositionY -= 1;
    }
  }
  else
  {
    currentTimeDying = millis()%1000000;
    if(currentTimeDying < previousTimeDying)
    {
      previousTimeDying -= 1000000;
    }
    if(currentTimeDying - previousTimeDying >= intervalDying)
    {
      previousTimeDying = currentTimeDying;
      curPositionY += 1;
    }
  }
  if(curPositionY == 7)
  {
    dying = false;
    reviving = true;
  }
}
void Player::revive()
{
  reviving = false;
  curPositionY -= 1;
  canJump = true;
}
