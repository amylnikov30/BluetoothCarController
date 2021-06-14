#include <Arduino.h>
// #include "Enums.cpp"
// #include "Vector2.cpp"
#define ENA 6
#define ENB 5
#define IN4 7
#define IN3 8
#define IN2 9
#define IN1 11

#define input_l 49
#define input_r 50
#define input_f 51
#define input_b 52

#define startToken 123
#define endToken 125

#define tokenLocation 6

int index;

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
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW); 
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
}

void Backward(bool debug=false)
{
  digitalWrite(IN1, LOW); 
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
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


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
}

void loop() {
  // put your main code here, to run repeatedly:
  RawInput();
  Move(true);
}
