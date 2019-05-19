#include <SoftwareSerial.h>
#include <HCSR04.h>
#include <LiquidCrystal.h>

int PIN_DRIVER_IN1 = 2;
int PIN_SENSOR_TRIG = 3;
int PIN_DRIVER_IN2 = 4;
int PIN_BLUETOOTH_RECEIVE = 5;
int PIN_BLUETOOTH_TRANSMIT = 6;
int PIN_DRIVER_IN3 = 7;
int PIN_DRIVER_IN4 = 8;
int PIN_SENSOR_FRONT = 9;
int PIN_LCD_RS = 10;
int PIN_BUZZER = 11;
int PIN_LCD_EN = 12;
int PIN_SENSOR_BACK = 13;
int PIN_LCD_D4 = A0;
int PIN_LCD_D5 = A1;
int PIN_LCD_D6 = A2;
int PIN_LCD_D7 = A3;


// sensor (trig, echo)
HCSR04 sensors (PIN_SENSOR_TRIG, new int[2] { PIN_SENSOR_FRONT, PIN_SENSOR_BACK }, 2);
SoftwareSerial Bluetooth (PIN_BLUETOOTH_RECEIVE, PIN_BLUETOOTH_TRANSMIT);
LiquidCrystal LCD (PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);

char val;

void setup() {
  pinMode(PIN_DRIVER_IN1, OUTPUT);
  pinMode(PIN_DRIVER_IN2, OUTPUT);
  pinMode(PIN_DRIVER_IN3, OUTPUT);
  pinMode(PIN_DRIVER_IN4, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  Serial.begin(9600);
  Bluetooth.begin(9600);
  LCD.begin(16, 2);
  LCD.clear();
  LCD.print("FRONT: ");
  LCD.setCursor(0, 1);
  LCD.print(" BACK: ");
}

void forward() 
{
  digitalWrite(PIN_DRIVER_IN1, HIGH);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, HIGH);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

void backward() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, HIGH);  
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, HIGH); 
}

void right() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, HIGH);
  digitalWrite(PIN_DRIVER_IN3, HIGH);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

void circleRight() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, HIGH);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

void left() { 
  digitalWrite(PIN_DRIVER_IN1, HIGH);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, HIGH); 
}

void circleLeft() {
  digitalWrite(PIN_DRIVER_IN1, HIGH);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

void honk() {
  tone(PIN_BUZZER, 500, 150);
  delay(400);
  tone(PIN_BUZZER, 500, 700);
}

void motorStop() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

void printPadding(int distance) {
  if (distance < 10)
    LCD.print("  ");
  else if (distance < 100)
    LCD.print(" ");
}

void loop()
{
  int front_dist = sensors.dist(0);
  LCD.setCursor(7, 0);
  printPadding(front_dist);
  LCD.print(front_dist);
  LCD.print(" CM  ");

  int back_dist = sensors.dist(1);
  LCD.setCursor(7, 1);
  printPadding(back_dist);
  LCD.print(back_dist);
  LCD.print(" CM  ");

  Bluetooth.listen();
  val = Bluetooth.read();
  if(val != -1) {
    Serial.print("Value received from bluetooth: ");
    Serial.println (val);
    if (val == 'w' || val == '8')
      if (front_dist > 10)
        forward();
      else
        honk();
    else if (val == 'x' || val == '2')
      if (back_dist > 10)
        backward();
      else
        honk();
    else if (val == 'a' || val == '4')
      left();
    else if (val == 'd' || val == '6')
      right();
    else if (val == 'q')
      circleLeft();
    else if (val == 'e')
      circleRight();
    else if (val == 'B') 
      honk();
    else if (val == 's')
      motorStop();
  }
}