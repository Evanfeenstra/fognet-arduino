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
BLECharacteristic characteristic = BLECharacteristic("00001234-0000-1000-8000-00805f9b34fb", BLEWriteWithoutResponse | BLENotify, BLE_ATTRIBUTE_MAX_VALUE_LENGTH);
BLEDescriptor descriptor = BLEDescriptor("00001234-0000-1000-8000-00805f9b34fb", "FogNet");

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

  // add service and characteristic
  blePeripheral.addAttribute(ledService);
  blePeripheral.addAttribute(characteristic);

  // assign event handlers for connected, disconnected to peripheral
  blePeripheral.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  blePeripheral.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);
  
  // assign event handlers for characteristic
  characteristic.setEventHandler(BLEWritten, characteristicWritten);

  // begin initialization
  blePeripheral.begin();

  Serial.println(F("BLE LED Peripheral"));
}

elapsedMillis timeElapsed = 10001;
int idx = 0;

void loop() {
  // poll peripheral
  blePeripheral.poll();
  if (timeElapsed > 10000) {
    Wire.beginTransmission(8);
    Wire.write("x is ");
    Wire.write(idx);
    Wire.endTransmission();
    characteristic.setValue("HITEST");
    idx++;
    timeElapsed = 0;
  }
  pollWire();
}

void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
  Wire.beginTransmission(8);
  Wire.write("<*>connected<^>");
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
  //const unsigned char* val = characteristic.value();
  Wire.beginTransmission(8); // transmit to device #8
  Wire.write(msg);
  Wire.endTransmission();
  //led = characteristic.value();
}

void pollWire() {
  //Wire.requestFrom(8,20);
  char* x;
  while (0 < Wire.available()) { 
    char c = Wire.read();
    x += c;
  }
  if(strlen(x)>0){
    characteristic.setValue(x);
  } 
}
