#include <Wire.h>
#include <Adafruit_LiquidCrystal.h>

Adafruit_LiquidCrystal lcd(0);


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
  Serial.begin(9600);
  Serial.println("teensy started"); 
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("hi");
  delay(1000);
  lcd.clear();
}

void loop() {
  delay(1);
  if (Serial.available() > 0) {
   //digitalWrite(led, HIGH);
   String val = "";
   bool returned = false;
   while(1) {
     char incomingByte = Serial.read();
     // '\r\n' to break
     if (incomingByte == '\n' && returned==true) {
      digitalWrite(led, HIGH);
      receiveSerial(val);
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
   //digitalWrite(led, LOW);
  }
  lcd.setCursor(0, 1);
  lcd.print(millis() / 1000);
}

void receiveSerial(String val) {
  
}

void receiveBle(int howMany) {
  while (1 < Wire.available()) { 
    char c = Wire.read(); 
    Serial.print(c);
  }
  char x = Wire.read();
  Serial.println(x);
}
