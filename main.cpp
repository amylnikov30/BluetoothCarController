#include <Arduino.h>
// #include <Servo.h>
#define f 16736925
#define b 16754775
#define l 16720605
#define r 16761405
#define s 16712445
#define ECHO_PIN A4
#define TRIG_PIN A5
#define ENA 5  //left speed
#define ENB 6  //right speed
#define IN1 7  //left forward
#define IN2 8  //left backward
#define IN3 9  //right backward
#define IN4 11 //right forward
#define carSpeed 250.0

Servo servo;

int rightDistance = 0;
int leftDistance = 0;
int middleDistance = 0;

const int input_l = 49;
const int input_r = 50;
const int input_f = 51;
const int input_b = 52;

const int startToken = 123;
const int endToken = 125;

const int tokenLocation = 6;

int arr[23];
int index;

int pinValue;

int servoAngle;

enum FunctionMode
{
    IDLE,
    Bluetooth
};

enum MotionMode
{
    STOP,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

FunctionMode func_mode = IDLE; // holds the mode of the function

MotionMode mov_mode = STOP; // holds the movement mode

void Delayed(unsigned long t)
{
    for (unsigned long i = 0; i < t; i++)
    {
        GetBluetoothInput();
        delay(1);
    }
}

void Forward(bool debug = false)
{
    digitalWrite(IN4, HIGH);          //Right forward on
    analogWrite(ENB, carSpeed - 2.0); //Setting speed to our preset car speed(WORK IN PROGRESS)
    digitalWrite(IN1, HIGH);          //left forward on
    analogWrite(ENA, carSpeed + 5.0); //Setting speed to our preset car speed(WORK IN PROGRESS)
    digitalWrite(IN2, LOW);           //Turn off left and right back for security
    digitalWrite(IN3, LOW);

    if (debug) //debugging
    {
        Serial.println("Forward");
    }
}

void Backward(bool debug = false)
{
    digitalWrite(IN2, HIGH); //left and right back on
    digitalWrite(IN3, HIGH);
    analogWrite(ENB, carSpeed); //preset car speed
    analogWrite(ENA, carSpeed); // WORK IN PROGRESS
    digitalWrite(IN4, LOW);     //left and right forward off for security
    digitalWrite(IN1, LOW);

    if (debug)
    {
        Serial.println("Backward");
    }
}

void Left(bool debug = false)
{
    analogWrite(ENB, carSpeed);
    analogWrite(ENA, carSpeed);
    digitalWrite(IN4, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);

    if (debug)
    {
        Serial.println("Left");
    }
}

void Right(bool debug = false)
{
    analogWrite(ENB, carSpeed);
    analogWrite(ENA, carSpeed);
    digitalWrite(IN4, LOW);  // Set right forward to low
    digitalWrite(IN3, HIGH); // Set right back to high
    digitalWrite(IN1, HIGH); // Set left forward to high
    digitalWrite(IN2, LOW);  // Set left back to low

    if (debug)
    {
        Serial.println("Right");
    }
}

void Stop(bool debug = false)
{
    digitalWrite(ENB, LOW);
    digitalWrite(ENA, LOW);

    if (debug)
    {
        Serial.println("Stop");
    }
}

/*
Gets data from bluetooth chip and sets the func_mode and mov_mode accordingly
*/
void GetBluetoothInput()
{
    if (Serial.available())
    {
        Serial.println(Serial.read());
        switch (Serial.read())
        {
            case 'f':
                Serial.println("case f is getting called");
                func_mode = Bluetooth;
                mov_mode = FORWARD;
                break;
            case 'b':
                func_mode = Bluetooth;
                mov_mode = BACKWARD;
                break;
            case 'l':
                func_mode = Bluetooth;
                mov_mode = LEFT;
                break;
            case 'r':
                func_mode = Bluetooth;
                mov_mode = RIGHT;
                break;
            case 's':
                func_mode = Bluetooth;
                mov_mode = STOP;
                break;
            case 103:
                Serial.println("case 103 is called");
                func_mode = Bluetooth;
                mov_mode = FORWARD;
            default:
                break;
        }
    }
}

/*
Handles main bluetooth input
*/
void BluetoothMode()
{
    if (func_mode == Bluetooth)
    {
        switch (mov_mode)
        {
            case FORWARD:
                Forward();
                break;
            case RIGHT:
                Right();
                break;
            case LEFT:
                Left();
                break;
            case BACKWARD:
                Backward();
                break;
            case STOP:
                Stop();
                break;
            default:
                break;
        }
    }
}

void RawInput()
{
    int input = Serial.read();
    // Serial.print(' ');

    // buttons
    if (input == 102) // ascii code for f
    {
        MotorTest();
        Serial.println("Called MotorTest()");
 
    }

    // analog stick
    index++;
    if (index == tokenLocation)
    {
        switch(input)
        {
            case input_f : Serial.println("forward"); MotorTest(); break;
            case input_b : Serial.println("backward"); break;
            case input_l : Serial.println("left"); break;
            case input_r : Serial.println("right"); break;
            default : Stop(); break;
        }
    }

    if (input == startToken)
    {
        index = 0;
    }
}

void MotorTest()
{
    // digitalWrite(ENA, HIGH);
    // digitalWrite(ENB, HIGH);

    // for (int i=2; i<13; i++)
    // {
    //     digitalWrite(i, HIGH);
    // }

    // delay(500);

    // for (int i=2; i<13; i++)
    // {
    //     digitalWrite(i, LOW);
    // }

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);

    pinValue = digitalRead(IN4);

    // servoAngle += 45;

    // servo.write(servoAngle);

    delay(1000);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}


/*
Called once on program startup
*/
void setup()
{
    Serial.begin(9600);
    // servo.attach(3, 500, 2400);
    // servo.write(90);
    //  pinMode(ECHO_PIN, INPUT);
    //  pinMode(TRIG_PIN, OUTPUT);
    servo.attach(3);
    servo.write(0);
    servo.detach();
    pinMode(ENA, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
}

/*
Called every tick (almost like every frame)
*/
void loop()
{
    // GetBluetoothInput();
    // BluetoothMode();
    RawInput();
    // MotorTest();
    Serial.println(pinValue);
}