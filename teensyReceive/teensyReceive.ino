#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

// MOSI, SCK, LATCH
Adafruit_LiquidCrystal lcd(11, 13, 15);

int led = 5;
int led2 = 7;
int led3 = 9;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  digitalWrite(led, HIGH);
  delay(100);
  digitalWrite(led, LOW);
  Wire.begin(8);
  Wire.onReceive(receiveBle);
  Wire.onRequest(bleRequest);
  Serial.begin(9600);
  Serial.println("teensy started");
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("ho");
  delay(1000);
  lcd.clear();
}

elapsedMillis timer = 5001;
int ins = 0;
int outs = 0;
String incomingSerial;
void loop() {
  delay(1);
  lcd.setCursor(0, 1);
  lcd.print(ins);
  //lcd.setCursor(0, 0);
  //lcd.print(val);
  lcd.setCursor(8, 1);
  lcd.print(outs);
  if (incomingSerial == ""){
    checkForSerial();
  }
}

void checkForSerial(){
  if (incomingSerial == "" && Serial.available() > 0) {
   digitalWrite(led3, HIGH);
   String val = "";
   bool returned = false;
   while(1) {
     char incomingByte = Serial.read();
     // '\r\n' to break
     if (incomingByte == '\n' && returned==true) {
      digitalWrite(led, HIGH);
      //receiveSerial(val);
      incomingSerial=val;
      break;
     }
     if (incomingByte == '\r') {
      digitalWrite(led2, HIGH);
      returned = true;
     } else {
      returned = false;
     }
     if (incomingByte == -1) continue;
     val += incomingByte;
   }
   digitalWrite(led3, LOW);
  }
}

void bleRequest(){
  if(incomingSerial!=""){
    char c[20];
    incomingSerial.toCharArray(c, 20);
    Wire.write(c);
    incomingSerial = "";
    outs++;
  } 
}

String wireBuff;
void receiveBle(int howMany) { // also runs on requestfrom???
  String buff;
  while (1 < Wire.available()) {
    char c = Wire.read();
    buff += c;
    //Serial.print(c);
  }
  if(Wire.available()==1){
    char x = Wire.read();
    buff += x;
    if(buff!=wireBuff){      
      ins++;
      Serial.println(buff);
      wireBuff=buff;
      //Wire.flush();
    }
  }
}
