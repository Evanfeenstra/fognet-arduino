#include <Wire.h>
#include <elapsedMillis.h>

// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>

// pins unused on the nRF51832
#define BLE_REQ   -1
#define BLE_RDY   -1
#define BLE_RST   -1

// orange LED pin
//#define LED_PIN   13

//byte led = 0;

BLEPeripheral blePeripheral = BLEPeripheral(BLE_REQ, BLE_RDY, BLE_RST);
BLEService ledService = BLEService("00001234-0000-1000-8000-00805f9b34fb");
BLECharacteristic characteristic = BLECharacteristic("00001234-0000-1000-8000-00805f9b34fb", BLEWriteWithoutResponse, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
BLECharacteristic notifyChar = BLECharacteristic("00001235-0000-1000-8000-00805f9b34fb", BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);

void setup() {
  Wire.begin();
  Serial.begin(9600);
#if defined (__AVR_ATmega32U4__)
  delay(5000);  //5 seconds delay for enabling to see the start up comments on the serial board
#endif

  // set LED pin to output mode
  //pinMode(LED_PIN, OUTPUT);

  // set advertised local name and service UUID
  blePeripheral.setLocalName("FogNet");
  blePeripheral.setAdvertisedServiceUuid(ledService.uuid());
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(characteristic);
  blePeripheral.addAttribute(notifyChar);

  // assign event handlers for connected, disconnected to peripheral
  //blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  //blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  
  // assign event handlers for characteristic
  characteristic.setEventHandler(BLEWritten, characteristicWritten);

  // begin initialization
  blePeripheral.begin();

  Serial.println(F("BLE LED Peripheral"));
}

elapsedMillis timeElapsed = 101;
int idx = 0;

void loop() {
  blePeripheral.poll();
  delay(1);
  if(timeElapsed>5000){
    //pollWire();
    pollWire2();
    timeElapsed = 0;
  }
}

void blePeripheralConnectHandler(BLECentral& central) {
  Wire.beginTransmission(8);
  Wire.write("<*>connected        ");
  Wire.write("<^>");
  Wire.endTransmission();
}

void blePeripheralDisconnectHandler(BLECentral& central) {
  // central disconnected event handler
  Serial.print(F("Disconnected event, central: "));
  Serial.println(central.address());
}

void characteristicWritten(BLECentral& central, BLECharacteristic& characteristic) {
  // central wrote new value to characteristic, update LED
  Serial.print(F("Characteristic event, writen: "));
  char msg[20];
  sprintf(msg, "%20c", NULL);
  memcpy(msg, (char*)characteristic.value(), characteristic.valueLength());
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(msg);
  Wire.endTransmission();
}

char* receiveBuffer;
void pollWire() {
  char* buff;
  //int n = 0;
  int n = Wire.requestFrom(8,20); // THIS IS SPAMMING
  if(n==20) {
    Wire.readBytes(buff, n);
    if(buff!=receiveBuffer){
      notifyChar.setValue(buff);
      receiveBuffer = buff;
    }
  }
  /*while(Wire.read()) {
    num = Wire.receive();
  }*/
  //char* x = Wire.read();
  //notifyChar.setValue(x);
  /*char* x;
  while (Wire.available() > 0) { 
    char c = Wire.read();
    x += c;
    if(x[0]!=0 && x[1]!=0 && x[2]!=0){
      notifyChar.setValue(x);
    }
  }*/
}

void pollWire2() {
  int n = Wire.requestFrom(8,20);
  String buff = "";
  //int n=0;
  while( Wire.available() > 0 ) {
    char c = Wire.read();
    buff += c;
  }
  if(buff!=""){
    //buff[19]='\0';
    //char a[20];
    //sprintf(a,buff);
    //char msg[20];
    //sprintf(buff, "%20c", NULL);
    char msg[20];
    buff.toCharArray(msg, 20);
    notifyChar.setValue((unsigned char*)msg,20);
    //notifyChar.setValue(buff);
  } 
  /*if(n==20) {
    Wire.readBytes(buff, n);
    if(strcmp(buff, receiveBuffer) != 0){
      notifyChar.setValue(buff);
      receiveBuffer = buff;
    }
  }*/
}

