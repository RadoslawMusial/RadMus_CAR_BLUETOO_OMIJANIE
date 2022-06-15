
#include <Servo.h> 
#include <AFMotor.h>
#define Echo A0
#define Trig A1
char przycisk;
int pomiar_odl;
int w_lewo;
int w_prawo;
int echo_lewo = 0;
int echo_prawo = 0;
Servo servo;
AF_DCMotor M1(1);
AF_DCMotor M2(2);
AF_DCMotor M3(3);
AF_DCMotor M4(4);
void setup() {
  Serial.begin(9600);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(10);
  M1.setSpeed(150);
  M2.setSpeed(150);
  M3.setSpeed(150);
  M4.setSpeed(150);
}
void loop() {
  
  omijanie_przeszkod();
  Kontrola_bluetooth();
 
}
void Kontrola_bluetooth() {
  if (Serial.available() > 0) {
    przycisk = Serial.read();
    Serial.println(przycisk);
  }
  if (przycisk == 'F') {
    jedz_przod();
  } else if (przycisk == 'B') {
    jedz_tyl();
  } else if (przycisk == 'L') {
    jedz_w_lewo();
  } else if (przycisk== 'R') {
    jedz_w_prawo();// pobranie odczytu z czujnika ultradźwiękowego oraz umieszczenie go w zmiennej
  } 
}
void omijanie_przeszkod() {
  pomiar_odl = odczyt_danych(); 
  if (pomiar_odl <= 15) {
    Stop();
    jedz_tyl();
    delay(100);
    Stop();
    echo_lewo = zmierz_L();
    servo.write(100);
    delay(800);
    echo_prawo = zmierz_P();
    servo.write(100);
    if (echo_lewo  < echo_prawo) {
      jedz_w_prawo();
      delay(500);
    Stop();
      delay(200);
    } else if (echo_lewo  > echo_prawo) {
      jedz_w_lewo();
      delay(500);
      Stop();
      delay(200);
    }
  } else {
    jedz_przod();
  }
}

int odczyt_danych() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(4);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t / 58; 
  return cm;
}
int zmierz_P() {
  servo.write(20);
  delay(800);
  w_lewo = odczyt_danych();
  return w_lewo;
}
int zmierz_L() {
  servo.write(180);
  delay(800);
  w_prawo = odczyt_danych();
  return w_prawo;
}
void jedz_przod() {
  M1.run(FORWARD);
  M2.run(FORWARD);
  M3.run(FORWARD);
  M4.run(FORWARD);
}
void jedz_tyl() {
  M1.run(BACKWARD);
  M2.run(BACKWARD);
  M3.run(BACKWARD);
  M4.run(BACKWARD);
}
void jedz_w_prawo() {
  M1.run(BACKWARD);
  M4.run(BACKWARD);
  delay(1000);
}
void jedz_w_lewo() {
  M2.run(BACKWARD);
  M3.run(BACKWARD);
 delay(1000); 
}
void Stop() {
  M1.run(RELEASE);
  M2.run(RELEASE);
  M3.run(RELEASE);
  M4.run(RELEASE);
}
