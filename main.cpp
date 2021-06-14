#include <Arduino.h>
// #include "Enums.cpp"
// #include "Vector2.cpp"
#define ENA 6
#define ENB 5
#define IN4 7
#define IN3 8
#define IN2 9
#define IN1 11

const int input_l = 49;
const int input_r = 50;
const int input_f = 51;
const int input_b = 52;

int index;

const int startToken = 123;
const int endToken = 125;

const int tokenLocation = 6;

enum Direction
{
  forward,
  backward,
  right,
  left,
  neutral
};

Direction direction = neutral;

void Forward(bool debug=false)
{
  digitalWrite(IN1, HIGH); //set IN1 hight level
  digitalWrite(IN2, LOW); //set IN2 low level
  digitalWrite(IN3, LOW); //set IN3 low level
  digitalWrite(IN4, HIGH); //set IN4 hight level
}

void Backward(bool debug=false)
{
  digitalWrite(IN1, LOW); //set IN1 hight level
  digitalWrite(IN2, HIGH); //set IN2 low level
  digitalWrite(IN3, HIGH); //set IN3 low level
  digitalWrite(IN4, LOW); //set IN4 hight level
}

void Right(bool debug=false)
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void Left(bool debug=false)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Stop(bool debug=false)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void RawInput()
{
    char result;
    int input = Serial.read();
    Serial.println(' ' + input);
    // Serial.print(' ');

    // buttons
//    if (input == 102) // ascii code for f
//    {
//        MotorTest();
//        Serial.println("Called MotorTest()");
// 
//    }

    // analog stick
    index++;
    if (index == tokenLocation)
    {
        switch(input)
        {
            case input_f : direction = forward; break;
            case input_b : direction = backward; break;
            case input_l : direction = right; break;
            case input_r : direction = left; break;
            default : direction = neutral; break;
        }
    }

    if (input == startToken)
    {
        index = 0;
    }
}

void Move(bool debug=false)
{
  switch (direction)
  {
    case forward : Forward(debug); break;
    case backward : Backward(debug); break;
    case left : Left(debug); break;
    case right : Right(debug); break;
    default : Stop(debug); break;
  }
}

//void GetInput(bool debug=false)
//{
//  int rawInput = ;
//  char input = char(rawInput);
//
//  switch (input)
//  {
//    case 'w' : direction = forward; break;
//    case 'a' : direction = left; break;
//    case 's' : direction = backward; break;
//    case 'd' : direction = left; break;
//    default : break;
//  }
//}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  digitalWrite(ENA, HIGH); //enable L298n A channel
  digitalWrite(ENB, HIGH); //enable L298n B channel
}

void loop() {
  // put your main code here, to run repeatedly:
  RawInput();
  Move();
}