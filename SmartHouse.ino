#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define FIREBASE_HOST "123.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "123"
#define WIFI_SSID "ASPC_H3C"
#define WIFI_PASSWORD "Fiks$20aspC#75T"

#include <IRremote.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <TroykaMQ.h>
#include <stdio.h>
#include <Servo.h>

IRrecv receiver(6);
decode_results results;
bool alarm = false;

Servo servo;
#define PIN_MQ2 A0
MQ2 mq2(PIN_MQ2);
#define WaterKitchen A1
#define WaterBasement A2
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
int waterK = 0, waterBs = 0;
int dthK = 0, dthB = 0, dthH = 0;
int dthHK = 0, dthHB = 0, dthHH = 0;
int moveH = 0, moveB = 0, moveK = 0, moveL = 0;

int Led = 12;
int Sound = 11;
int Relay1 = 7;
int Relay2 = 8;
int r1State = 0;
int r2State = 0;

int LeakThresh = 400;
int leakKitchen = 0;
int leakBasement = 0;

void HandleIR() {
  if (receiver.decode(&results)) {
    if (results.value == 16750695) {
      alarm = false;
    }
    else if (results.value == 16738455) {
      alarm = true;
    }
    else if (results.value == 16756815) {
      r1State = 1 - r1State;
      digitalWrite(Relay1, r1State);
    }
    else if (results.value == 16748655) {
      r2State = 1 - r2State;
      digitalWrite(Relay2, r2State);
    }
    receiver.resume();
  }
}

void GuardCheck() {
  if (alarm) {
    if (moveK || moveB || moveH || moveL) {
      digitalWrite(Led, 1);
      digitalWrite(Sound, 1);
    }
    else {
      digitalWrite(Led, 0);
      digitalWrite(Sound, 0);
    }
  }
  else {
    digitalWrite(Led, 0);
    digitalWrite(Sound, 0);
  }
}

void GetData() {
  mq2.calibrate();
  mq2LPG = 0;
  mq2methane = 0;
  mq2smoke = 0;
  mq2hydrogen = 0;
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
  waterBs = analogRead(WaterBasement);
  leakKitchen = (waterK > LeakThresh) ? 1 : 0;
  leakBasement = (waterBs > LeakThresh) ? 1 : 0;

  dthK = dthKitchen.readTemperature();
  dthB = dthBedroom.readTemperature();
  dthH = dthHall.readTemperature();

  dthHK = dthKitchen.readHumidity();
  dthHB = dthBedroom.readHumidity();
  dthHH = dthHall.readHumidity();

  moveK = digitalRead(MoveKitchen);
  moveB = digitalRead(MoveBedroom);
  moveH = digitalRead(MoveHall);
  moveL = digitalRead(MoveLong);
}

void SendFirebase() {
  Firebase.setFloat("mq2/LPG", mq2LPG);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("mq2/methane", mq2methane);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("mq2/smoke", mq2smoke);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("mq2/hydrogen", mq2hydrogen);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("env/kitchen/temperature", dthK);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("env/bedroom/temperature", dthB);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("env/hall/temperature", dthH);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("env/kitchen/humidity", dthHK);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("env/bedroom/humidity", dthHB);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("env/hall/humidity", dthHH);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("water/kitchen", waterK);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("water/basement", waterBs);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("leak/kitchen", leakKitchen);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("leak/basement", leakBasement);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("move/kitchen", moveK);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("move/bedroom", moveB);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("move/hall", moveH);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("move/long", moveL);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("alarm/enabled", alarm ? 1 : 0);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("appliance/relay1", r1State);
  if (Firebase.failed()) {
    return;
  }
  Firebase.setFloat("appliance/relay2", r2State);
  if (Firebase.failed()) {
    return;
  }

  Serial.println(mq2LPG);
  Serial.println(mq2methane);
  Serial.println(mq2smoke);
  Serial.println(mq2hydrogen);
  Serial.println(waterK);
  Serial.println(waterBs);
  Serial.println(dthK);
  Serial.println(dthB);
  Serial.println(dthH);
  Serial.println(dthHK);
  Serial.println(dthHB);
  Serial.println(dthHH);
  Serial.println(moveK);
  Serial.println(moveB);
  Serial.println(moveH);
  Serial.println(moveL);
  Serial.println(leakKitchen);
  Serial.println(leakBasement);
  Serial.println(alarm ? 1 : 0);
  Serial.println(r1State);
  Serial.println(r2State);
}

void setup() {
  Serial.begin(9600);
  servo.attach(10);
  dthKitchen.begin();
  dthBedroom.begin();
  dthHall.begin();
  receiver.enableIRIn();

  pinMode(Led, OUTPUT);
  pinMode(Sound, OUTPUT);
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  digitalWrite(Led, 0);
  digitalWrite(Sound, 0);
  digitalWrite(Relay1, r1State);
  digitalWrite(Relay2, r2State);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  HandleIR();
  GetData();
  GuardCheck();
  SendFirebase();
}
