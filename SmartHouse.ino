#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <TroykaMQ.h>
#include <stdio.h>
#include <Servo.h> //используем библиотеку для работы с сервоприводом

Servo servo; //объявляем переменную servo типа Servo
#define PIN_MQ2 A0
MQ2 mq2(PIN_MQ2);
#define WaterKitchen A1
#define WaterBath A2
#define DthKitchen A3
DHT dthKitchen(DthKitchen, DHT11);
#define DthBedroom A4
DHT dthBedroom(DthBedroom, DHT11);
#define DthHall A5
DHT dthHall(DthHall, DHT11);
#define MoveHall 3
#define MoveBedroom 2
#define MoveKitchen 4
#define MoveLong 5
int mq2LPG = 0, mq2methane = 0, mq2smoke = 0, mq2hydrogen = 0;
int waterK, waterB;
int dthK, dthB, dthH;
int moveH, moveB, moveK, moveL;

//infrare 6;

#define FIREBASE_HOST "realairsmort.firebaseio.com" //Firebase project link
#define FIREBASE_AUTH "dedvJsRv3jHcHplnFaPeJiPlsJKrOdNenfPCHWWr" //Firebase key

void setup() {
  Serial.begin(9600);
  servo.attach(10); //привязываем привод к порту 10
  dthKitchen.begin();
  dthBedroom.begin();
  dthHall.begin();
}

void loop() {
  GetData();
  SendFirebase();
  delay(2000);
}

void GetData(){
  mq2.calibrate();
  for (int i = 0; i < 10; i++) {
    mq2LPG = mq2LPG + mq2.readLPG();
    mq2methane = mq2methane + mq2.readMethane();
    mq2smoke = mq2smoke + mq2.readSmoke();
    mq2hydrogen = mq2hydrogen + mq2.readHydrogen();
  }
  mq2LPG = mq2LPG / 10;
  mq2methane = mq2methane / 10;
  mq2smoke = mq2smoke / 10;
  mq2hydrogen = mq2hydrogen / 10;

  waterK = analogRead(WaterKitchen);
  waterB = analogRead(WaterBath);
  
  dthK = dthKitchen.readTemperature();
  dthB = dthBedroom.readTemperature();
  dthH = dthHall.readTemperature();

  moveK = digitalRead(MoveKitchen);
  moveB = digitalRead(MoveBedroom);
  moveH = digitalRead(MoveHall);
  moveL = digitalRead(MoveLong);
}

void SendFirebase(){
  Serial.println("");
  Serial.println(mq2LPG);
  Serial.println(waterK);
  Serial.println(waterB);
  Serial.println(dthK);
  Serial.println(dthB);
  Serial.println(dthH);
  Serial.println(moveK);
  Serial.println(moveB);
  Serial.println(moveH);
  Serial.println(moveL);
}
