#include "pitches.h"
#include <Servo.h>

//윤형우 코드에 필요한 변수
int buttonPin = 2;
int stopButtonPin = 4;
int ledPins[] = {A1, A2, A3};
int buttonState = 0;
int lastButtonState = 0;
int stopButtonState = 0;
int inputCount = 0;
int maxCount;
int temp;


//변성훈 코드에 필요한 변수
int boozer = NOTE_A2;
int notes[] = {NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};

//김성현 코드에 필요한 변수
const int pingPin1 = 9; //trig
const int pingPin2 = 8; // echo
int PressSensor = 1000;
int led1 = 5;
int led2 = 6;
int drink_count = 0;

//나동현 코드에 필요한 변수
Servo myservo;
Servo myservo2;
Servo myservo3;
int val;
int bcount = 0;
int AA = 12;  // AA를 연결한 디지털 핀
int AB = 13;   // AB를 연결한 디지털 핀

//주량 넘길경우 경보음을 출력하는 함수
void booz(int drink_capacity, int toast_count)
{

  if (toast_count == drink_capacity) //주량에 도달했을 때
  {
    //그만 마시자고 이야기한다. -> 일단 멜로디로 구성함
    for (int thisNote = 0; thisNote < 9; thisNote++)
    {
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(7, notes[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      delay(pauseBetweenNotes);

      noTone(7);
    }
  }
  else //주량을 넘겼을 때
  {
    //부저를 울린다
    tone(7, boozer, 2000);
  }
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}

void cheers() {
  int total = maxCount * 8; // 한 병에 8잔으로 계산
  int brightness = 255 / total;


  analogWrite(led1, drink_count * brightness);
  analogWrite(led2, drink_count * brightness);

  if (digitalRead(8) == HIGH || PressSensor < 1000) {
    Serial.println("버튼눌림/진동센서 인식");
    if (drink_count > total) {
      analogWrite(led1, HIGH);
      analogWrite(led2, HIGH);
      booz(total, drink_count); //
    }
  }

  //Serial.println(PressSensor);
  delay(100);
}

void setup() {
  //윤형우 코드에 필요한 셋업
  pinMode(buttonPin, INPUT);
  pinMode(ledPins[0], OUTPUT);
  pinMode(ledPins[1], OUTPUT);
  pinMode(ledPins[2], OUTPUT);
  Serial.begin(9600);

  Serial.println(maxCount);

  //김성현 코드 셋업
  pinMode(pingPin1, OUTPUT);
  pinMode(pingPin2, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);


  //나동현 코드 셋업
  myservo.attach(10);
  myservo2.attach(9);
  myservo3.attach(11);
  pinMode(AA, OUTPUT);  // AA를 출력 핀으로 설정
  pinMode(AB, OUTPUT);  // AB를 출력 핀으로 설정
  pinMode(8, INPUT);

  //초기화

  myservo.write(30);
  myservo2.write(0);
  myservo3.write(180);
  Serial.println("서보모터 초기화");
  delay(500);
  //water(1800);
  Serial.println("주량 입력");
  maxCount = inputBottles();
  Serial.println("주량 입력 받음");
  temp = maxCount;

}

void loop() {
  PressSensor = analogRead(A0);
  cheers();
  Serial.println(drink_count);
  Serial.println(PressSensor);
  robotMotor(PressSensor, drink_count, maxCount);

}

int inputBottles() {
  while (1) {
    stopButtonState = digitalRead(stopButtonPin);
    if (stopButtonState != 0) break;
    buttonState = digitalRead(buttonPin);
    delay(100);
    if (buttonState != lastButtonState) {
      if (buttonState == HIGH) {
        digitalWrite(ledPins[inputCount], HIGH);
        inputCount++;
      }
    }
    lastButtonState = buttonState;
  }
  return inputCount;
}

void robotMotor(int PressSensor, int count, int maxCount) {
  //대기모드
  if (count > maxCount * 8) {
    myservo.write(90);
    myservo2.write(60);
    myservo3.write(125);
    Serial.println("대기");
    delay(15);
  }
  //건배모드
  else {

    delay(15);

    //마시기모드
    if (digitalRead(8) == HIGH || PressSensor < 1000) {
      //myservo.write(120);
      for (int i = 30; i <= 125; i++) {
        myservo.write(i);
        delay(10);
      }
      myservo2.write(0);
      myservo3.write(180);
      delay(6000);
      Serial.println("마시기");
      water(2500);
      Serial.println("워터펌프작동");
      drink_count++;
      Serial.println("drink_count++");
      myservo.write(30);
      myservo2.write(0);
      myservo3.write(180);
      Serial.println("건배"); myservo.write(30);
      myservo2.write(0);
      myservo3.write(180);
      Serial.println("건배");
      if (drink_count % 8 == 0 && drink_count != 0) {
        Serial.println("실행 됨");
        digitalWrite(ledPins[temp - 1], LOW);
        temp--;
      }
      delay(10000);
      Serial.println("딜레이 실행됨");
    }
  }
}



void water(int time)
{
  digitalWrite(AA, LOW);  // 정방향으로 모터 회전
  digitalWrite(AB, HIGH);
  delay(time);
  digitalWrite(AA, LOW);
  digitalWrite(AB, LOW);
  delay(time);
  return; // time ms 동안 상태 유지(1000ms = 5s)
}
