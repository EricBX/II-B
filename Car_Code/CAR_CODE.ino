#define RIGHT_BACKWARD 13
#define RIGHT_FORWARD 12
#define LEFT_BACKWARD 10
#define LEFT_FORWARD 9
#define STEER 14
int duty = 1150;
int CYCLE = 4;
int low = 200;
int run = 165;
int fast = 50;
char command;
void Forward();
void FastForward();
void TurnLeft();
void TurnRight();
void LeftForward();
void RightForward();
void Backward();
void Stop();
void Steer(float ratio);

void setup()
{
  Serial.begin(9600);
  pinMode(LEFT_BACKWARD,OUTPUT);
  pinMode(RIGHT_BACKWARD,OUTPUT); 
  pinMode(LEFT_FORWARD,OUTPUT);
  pinMode(RIGHT_FORWARD,OUTPUT);
  pinMode(STEER,OUTPUT);
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);
  //digitalWrite(STEER,HIGH);
  
}

void loop()
{
  if (Serial.available()>0)
  {
    command=Serial.read();
    if (command=='F')
    {
      Serial.println("Forward");
      Forward();
      Serial.read();
    }
    else if (command=='B')
    {
      Serial.println("Backward");
      Backward();
      Serial.read();
    }
    else if (command=='L')
    {
      Serial.println("TurnLeft");
      TurnLeft();
      Serial.read();
    }
    else if (command=='R')
    {
      Serial.println("TurnRight");
      TurnRight();
      Serial.read();
    }
    else if (command=='E')
    {
      Serial.println("RightForward");
      RightForward();
      Serial.read();
    }
    else if (command=='Q')
    {
      Serial.println("LeftForward");
      LeftForward();
      Serial.read();
    }
    else if (command=='W')
    {
      Serial.println("FastFoward");
      FastForward();
      Serial.read();
    }
    else if (command=='S')
    {
      Serial.println("Stop");
      Stop();
      Serial.read();
    }
  }
}


void Forward()
{
  analogWrite(RIGHT_FORWARD,run);
  digitalWrite(RIGHT_BACKWARD,HIGH);

  analogWrite(LEFT_FORWARD,run);
  digitalWrite(LEFT_BACKWARD,HIGH);  
  Steer(1.0);
}

void Backward()
{
  digitalWrite(RIGHT_FORWARD,HIGH);
  analogWrite(RIGHT_BACKWARD,low);

  digitalWrite(LEFT_FORWARD,HIGH);
  analogWrite(LEFT_BACKWARD,low);  
  Steer(1.0);
}

void TurnLeft()
{
  digitalWrite(RIGHT_FORWARD,HIGH);
  analogWrite(RIGHT_BACKWARD,run+30);

  digitalWrite(LEFT_FORWARD,HIGH);
  analogWrite(LEFT_BACKWARD,run);  
  Steer(0.85);
}

void TurnRight()
{
  digitalWrite(RIGHT_FORWARD,HIGH);
  analogWrite(RIGHT_BACKWARD,run);

  digitalWrite(LEFT_FORWARD,HIGH);
  analogWrite(LEFT_BACKWARD,run+30);  
  Steer(1.15);
}

void Stop()
{
  digitalWrite(LEFT_BACKWARD,HIGH);
  digitalWrite(RIGHT_BACKWARD,HIGH);  
  digitalWrite(LEFT_FORWARD,HIGH);
  digitalWrite(RIGHT_FORWARD,HIGH);
  Steer(1.0);
}

void Steer(float ratio)
{
  int temp = ratio*duty;
  for (int i = 0;i < CYCLE;i++)
  {
    digitalWrite(STEER,HIGH);
    delayMicroseconds(temp);
    digitalWrite(STEER,LOW);
    delayMicroseconds(10000-temp);
    delayMicroseconds(6500);
  }
}

//Following are addition functions

void FastForward()
{
  digitalWrite(RIGHT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,HIGH);

  digitalWrite(LEFT_FORWARD,LOW);
  digitalWrite(LEFT_BACKWARD,HIGH);  
  Steer(1.0);
}

void LeftForward()
{
  analogWrite(RIGHT_FORWARD,run+5);
  digitalWrite(RIGHT_BACKWARD,HIGH);

  analogWrite(LEFT_FORWARD,run+40);
  digitalWrite(LEFT_BACKWARD,HIGH);  
  Steer(1.15);
}

void RightForward()
{
  analogWrite(RIGHT_FORWARD,run+40);
  digitalWrite(RIGHT_BACKWARD,HIGH);

  analogWrite(LEFT_FORWARD,run+5);
  digitalWrite(LEFT_BACKWARD,HIGH);  
  Steer(0.85);
}
