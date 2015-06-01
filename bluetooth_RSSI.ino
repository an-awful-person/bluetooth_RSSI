#include <SoftwareSerial.h>

//Define Pins
#define RxD 11  //Receive Serial pin, should be connected to the Transmit Serial pin of the bluetooth device
#define TxD 10  //Transmit Serial pin, should be connected to the Receive Serial pin of the bluetooth device
SoftwareSerial bt(RxD, TxD);

int ledPin[5] = {12, 13, A0, A1, A2};
int buttonPin = 9;

//Define for delay
unsigned long previousMillis = 0;
const long interval = 100;

//Define for Data
char btData;
char crud;
String splitString(String str, String devider, int arrayNumber);
String deviceAddress;
String deviceClass;
String deviceRSSI;
String message;

bool ATCommandsVisible = true;
bool RSSIVisible = true;
String atQuestEnd = "?\r\n";
String atSetEnd = "\r\n";
String atStart = "AT+";
String Version = "VERSION";
String Address = "ADDR";
String Name = "NAME";
String Role = "ROLE";
String UART = "UART";
String State = "STATE";
String Password = "PSWD";
String Reset = "ORGL";
String Initialize = "INIT";
String accesCode = "IAC=9E8B33";
String deviceType = "CLASS=0";
String searchRSSI = "INQM=1,1000,48";
//RSSI, limit of devices receivable(will find more of the same), max time spend searching *1.28
String receiveRSSI = "INQ";
String roleIsMaster = "ROLE=1";
String roleIsSlave = "ROLE=0";

void setup() {
  Serial.begin(9600);
  bt.begin(38400);
  showMenu();
  ATCmdSetup();
  for (int i = 0; i < 5; i++) {
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(buttonPin,INPUT);
}

void loop() {
  unsigned long currentMillis = millis();
  int buttonState = digitalRead(buttonPin);


  if (Serial.available()) {
    crud = Serial.read();
  }
  if (crud != 0) {
    commandControl();
  }
  crud = 0;

  //beacon 1 Read
  bt.listen();
  if (bt.available()) {

    btData = bt.read();
    message.concat(btData);
    if (btData == '\n') {
      filterCode();
    }
    if (ATCommandsVisible) {
      Serial.write(btData);
    }
  }
  if (currentMillis - previousMillis > interval) {
    previousMillis = currentMillis;
    if(!buttonState){
     listRSSI(); 
    }
    int receivedRSSI = pow((-(int(hexToDec(deviceRSSI)) - int(hexToDec("D666")))),0.45);
    for (int i = 0; i < 5; i++) {
      if (receivedRSSI < 100 - (20 * i)) {
        digitalWrite(ledPin[i], HIGH);
      } else {
        digitalWrite(ledPin[i], LOW);
      }
    }
  }
}





























