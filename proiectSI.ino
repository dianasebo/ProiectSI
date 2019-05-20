#include <SoftwareSerial.h>
#include <HCSR04.h>
#include <LiquidCrystal.h>

//declararea pinilor folosiți
int PIN_DRIVER_IN1 = 2;
int PIN_SENSOR_TRIG = 3;
int PIN_DRIVER_IN2 = 4;
int PIN_BLUETOOTH_TRANSMIT = 5;
int PIN_BLUETOOTH_RECEIVE = 6;
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


//instanțierea senzorilor folosind biblioteca HCSR04
HCSR04 sensors (PIN_SENSOR_TRIG, new int[2] { PIN_SENSOR_FRONT, PIN_SENSOR_BACK }, 2);
//instanțierea modulului Bluetooth folosind biblioteca SoftwareSerial
SoftwareSerial Bluetooth (PIN_BLUETOOTH_RECEIVE, PIN_BLUETOOTH_TRANSMIT);
//instanțierea modulului LCD folosind biblioteca LiquidCrystal
LiquidCrystal LCD (PIN_LCD_RS, PIN_LCD_EN, PIN_LCD_D4, PIN_LCD_D5, PIN_LCD_D6, PIN_LCD_D7);


char val; //= valoarea transmisă prin aplicația Android către modulul Bluetooth

void setup() {
  //inițializarea pinilor pentru driver-ul de motoare ca ieșire
  pinMode(PIN_DRIVER_IN1, OUTPUT);
  pinMode(PIN_DRIVER_IN2, OUTPUT);
  pinMode(PIN_DRIVER_IN3, OUTPUT);
  pinMode(PIN_DRIVER_IN4, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  //începerea comunicării cu modulul Bluetooth, la baud rate de 9600
  Bluetooth.begin(9600);
  //inițializarea modulului LCD și stabilirea modului de utilizare ca având 2 linii a câte 16 coloane
  LCD.begin(16, 2);
  LCD.clear();
  //tipărirea pe ecran a mesajelor care indică senzorii
  LCD.print("FRONT: ");
  LCD.setCursor(0, 1);
  LCD.print(" BACK: ");
}

//transmiterea semnalelor corespunzătoare către motoare pentru mers înainte
void forward() 
{
  digitalWrite(PIN_DRIVER_IN1, HIGH);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, HIGH);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

//transmiterea semnalelor corespunzătoare către motoare pentru mers înapoi
void backward() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, HIGH);  
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, HIGH); 
}

//transmiterea semnalelor corespunzătoare către motoare pentru executarea unui viraj la dreapta (roata din stânga merge în față iar cea din dreapta, în spate)
void right() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, HIGH);
  digitalWrite(PIN_DRIVER_IN3, HIGH);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

//transmiterea semnalelor corespunzătoare către motoare pentru executarea unei mișcări circulare la dreapta (roata din dreapta stă pe loc iar cea din stânga merge în față)
void circleRight() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, HIGH);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

//transmiterea semnalelor corespunzătoare către motoare pentru executarea unui viraj la stânga (roata din dreapta merge în față iar cea din stânga, în spate)
void left() { 
  digitalWrite(PIN_DRIVER_IN1, HIGH);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, HIGH); 
}

//transmiterea semnalelor corespunzătoare către motoare pentru executarea unei mișcări circulare la stânga (roata din stânga stă pe loc iar cea din dreapta merge în față)
void circleLeft() {
  digitalWrite(PIN_DRIVER_IN1, HIGH);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

//declanșarea unui sunet cu frecvența de 800 Hz și durata de 300 ms
void honk() {
  tone(PIN_BUZZER, 800, 300);
}

//transmiterea semnalelor corespunzătoare către motoare pentru oprirea lor
void motorStop() {
  digitalWrite(PIN_DRIVER_IN1, LOW);
  digitalWrite(PIN_DRIVER_IN2, LOW);
  digitalWrite(PIN_DRIVER_IN3, LOW);
  digitalWrite(PIN_DRIVER_IN4, LOW);
}

//se testează valorile pinilor motoarelor pentru mers înainte, respectiv înapoi
int goingForward() {
  return digitalRead(PIN_DRIVER_IN1) == HIGH
    && digitalRead(PIN_DRIVER_IN2) == LOW
    && digitalRead(PIN_DRIVER_IN3) == HIGH
    && digitalRead(PIN_DRIVER_IN4) == LOW;
}

int goingBackwards() {
  return digitalRead(PIN_DRIVER_IN1) == LOW
    && digitalRead(PIN_DRIVER_IN2) == HIGH
    && digitalRead(PIN_DRIVER_IN3) == LOW
    && digitalRead(PIN_DRIVER_IN4) == HIGH;
}

//afișarea pe modulul LCD a unor spații ce au ca scop alinierea informațiilor
void printPadding(int distance) {
  if (distance < 10)
    LCD.print("  ");
  else if (distance < 100)
    LCD.print(" ");
}

void loop()
{
  //preluarea informațiilor de la senzori și afișarea lor pe LCD
  int front_dist = sensors.dist(0);
  LCD.setCursor(7, 0);
  printPadding(front_dist);
  if (front_dist == 0)
    LCD.print("INFINIT");
  else {
    LCD.print(front_dist);
    LCD.print(" CM    "); 
  }
  
  //se face o pauză înaintea citirii următorului senzor, pentru a nu citi o valoare eronată
  delay(50);

  int back_dist = sensors.dist(1);
  LCD.setCursor(7, 1);
  printPadding(back_dist);
  if (back_dist == 0)
    LCD.print("NaN");
  else {
      LCD.print(back_dist);
      LCD.print(" CM    ");
  }

  //dacă distanța până la un obstacol este mai mică de 25 cm și se merge în direcția obstacolului, se claxonează și se opresc motoarele
  if (front_dist != 0 && front_dist < 25 && goingForward()) {
    honk();
    motorStop();
  }
  if (back_dist != 0 && back_dist < 25 && goingBackwards()) {
    honk();
    motorStop();
  }

  //se face o pauză înaintea citirii bluetooth-ului, pentru a nu citi o valoare eronată
  delay(100);

  //preluarea valorii de la modulul Bluetooth și alegerea, în funcție de valoare, a acțiunii de executat
  Bluetooth.listen();
  val = Bluetooth.read();
  if(val != -1) {
    Bluetooth.print("Received value: ");
    Bluetooth.println(val);
    if (val == 'w' || val == '8')
      if (front_dist != 0 && front_dist > 10)
        forward();
      else
        honk();
    else if (val == 'x' || val == '2')
      if (back_dist != 0 && back_dist > 10)
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

