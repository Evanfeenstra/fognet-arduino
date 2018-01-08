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
  digitalWrite(led3, HIGH);
  delay(100);
  digitalWrite(led3, LOW);
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
void loop() {
  delay(1);
  lcd.setCursor(0, 1);
  lcd.print(ins);
  if(timer>5000){
    Serial.println("TEENSY HEARTBEAT");
    timer = 0;
  }
  /*if (Serial.available() > 0) {
   digitalWrite(led3, HIGH);
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
   digitalWrite(led3, LOW);
  }*/
}

char* serialBuffer;
int outs = 0;
/*void receiveSerial(String val) {
  char c[20];
  val.toCharArray(c, 20);
  serialBuffer = c;
  outs++;
  lcd.setCursor(0, 0);
  lcd.print(val);
  lcd.setCursor(8, 1);
  lcd.print(outs);
  //Wire.write(serialBuffer);
}*/

void bleRequest(){
  String hi = "hihihihi";
  char c[20];
  hi.toCharArray(c, 20);
  Wire.write(c);
  /*if (Serial.available() > 0) {
   String val = "";
   bool returned = false;
   while(1) {
     char incomingByte = Serial.read();
     // '\r\n' to break
     if (incomingByte == '\n' && returned==true) {
      char c[20];
      val.toCharArray(c, 20);
      //Wire.write(c);
      Wire.write("hi testing");
      break;
     }
     if (incomingByte == '\r') {
      returned = true;
     } else {
      returned = false;
     }
     if (incomingByte == -1) continue;
     val += incomingByte;
   }
  }*/
}

String wireBuff;
void receiveBle(int howMany) { // also runs on requestfrom???
  String buff;
  //Serial.println(howMany);
  //ins = 10001;
  /*for(int i=0; i<howMany; i++){
    char c = Wire.read();
    buff += c; 
  }
  while(Wire.read()) {
    buff += Wire.receive();
  }
  ins++;
  lcd.setCursor(0, 1);
  lcd.print(ins);*/
  
  /*if(buff[0]!=0){
    Serial.print(buff);
  }*/
  
  while (1 < Wire.available()) {
    char c = Wire.read();
    buff += c;
    //Serial.print(c);
  }
  if(Wire.available()==1){
    char x = Wire.read();
    buff += x;
    //if(buff!=){      
      ins++;
      Serial.println(buff);
      Wire.flush();
    //}
  }
}
