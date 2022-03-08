#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "your FIREBASE HOST"
#define FIREBASE_AUTH "your secret"
#define WIFI_SSID "your WIFI SSID"
#define WIFI_PASSWORD "your WIF PASSWORD"

#include <IRremote.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <TroykaMQ.h>
#include <stdio.h>
#include <Servo.h> //используем библиотеку для работы с сервоприводом

IRrecv receiver(6); // create a receiver object of the IRrecv class
decode_results results; // create a results object of the decode_results class
bool alarm = false;

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
  receiver.enableIRIn(); // enable the receiver

    // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}

void loop() {
  if (receiver.decode(&results)) { // decode the received signal and store it in results
    Serial.println(results.value, HEX); // print the values in the Serial Monitor
    if (results.value == 16750695){
      alarm = false;
      Serial.println("alarm off");
    }
    else if (results.value == 16738455){
      alarm = true;
      Serial.println("alarm on");
    }
    receiver.resume(); // reset the receiver for the next code
  }
  
  GetData();
  SendFirebase();
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
