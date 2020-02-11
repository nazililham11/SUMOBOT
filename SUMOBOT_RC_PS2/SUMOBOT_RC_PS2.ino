#include <PS2X_lib.h>  //library v1.6

#define PS2_DAT     12  //14    
#define PS2_CMD     11  //15
#define PS2_SEL     10  //16
#define PS2_CLK     13  //17
#define pressures   true
#define rumble      true

#define m1 3 //kanan maju
#define m2 5 //kanan mundur
#define m3 6 //kiri maju
#define m4 9 //kiri mundur
#define m5 4 //relay

#define led 2

#define led1 2
#define led2 2

//sensor
const int s01  = A0;
const int s02  = A1;
const int s03  = A2;
const int s04  = A3;
const int volt = A4; //voltmeter
const int ser  = A5; //servo

int s1;
int s2;
int s3;
int s4;
int v;

float vout = 0.0;
float vin = 0.0;
float R1 = 100000.0; // resistansi R1 (100K)
float R2 = 10000.0;  // resistansi R2 (10K)

PS2X ps2x; // PS2 Controller Class

int error = 0;
byte type = 0;

void setup(){
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  pinMode(m5, OUTPUT);
  
  pinMode(led, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  
  pinMode(s01, INPUT);
  pinMode(s02, INPUT);
  pinMode(s03, INPUT);
  pinMode(s04, INPUT);
  pinMode(volt, INPUT);  

  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);
  digitalWrite(m4, LOW);
  digitalWrite(m5, LOW);
  digitalWrite(led, LOW);
  
  Serial.begin(57600);
  delay(300);  //added delay to give wireless ps2 module some time to startup, before configuring it
  
  //setup pins and settings: GamePad(clock, command, attention, data, Pressures?, Rumble?) check for error
  error = ps2x.config_gamepad(PS2_CLK, PS2_CMD, PS2_SEL, PS2_DAT, pressures, rumble);
  
  if(error == 0){
    Serial.println("Found Controller, configured successful ");
    Serial.println("pressures = " + String(pressures ? "true " : "false"));
    Serial.println("rumble = " + String(rumble ? "true)" : "false"));
    Serial.println("Try out all the buttons, X will vibrate the controller, faster as you press harder;");
    Serial.println("holding L1 or R1 will print out the analog stick values.");
    Serial.println("Note: Go to www.billporter.info for updates and to report bugs.");
  }  
  else if(error == 1)
    Serial.println("No controller found, check wiring, see readme.txt to enable debug. visit www.billporter.info for troubleshooting tips");
   
  else if(error == 2)
    Serial.println("Controller found but not accepting commands. see readme.txt to enable debug. Visit www.billporter.info for troubleshooting tips");

  else if(error == 3)
    Serial.println("Controller refusing to enter Pressures mode, may not support it. ");
  
  type = ps2x.readType(); 

  if      (type == 0) Serial.print("Unknown Controller type found ");
  else if (type == 1) Serial.print("DualShock Controller found ");
  else if (type == 2) Serial.print("GuitarHero Controller found ");
  else if (type == 3) Serial.print("Wireless Sony DualShock Controller found ");

}

void loop() {
  
  s1 = digitalRead(s01);
  s2 = digitalRead(s02);
  s3 = digitalRead(s03);
  s4 = digitalRead(s04);
  v = analogRead(volt);
  
  vout = (v * 5.0) / 1024.0;
  vin = vout / (R2/(R1+R2)); 
  
  if (vin <= 11.4) digitalWrite(led, HIGH);
  
  ps2x.read_gamepad(); //read controller and set large motor to spin at 'vibrate' speed

  if (error == 1);
    setup();
        
  berhenti();

  if      (ps2x.Analog(PSS_LY) <= 50)  kiri_mundur();
  else if (ps2x.Analog(PSS_LY) >= 200) kiri_maju();
  else                                 kiri_mati();
  
  if      (ps2x.Analog(PSS_RY) <= 50)  kanan_mundur();
  else if (ps2x.Analog(PSS_RY) >= 200) kanan_maju();
  else                                 kanan_mati();
  
  if (ps2x.Button(PSB_R1)) digitalWrite(m5, HIGH); 

  if (ps2x.Button(PSB_CROSS))  maju(); 
  if (ps2x.Button(PSB_SQUARE)) mundur(); 
   
  
  delay(10);  
}

void kiri_maju(){  
  digitalWrite(m1, HIGH);
  digitalWrite(m2, LOW);
}

void kiri_mundur(){
  digitalWrite(m1, LOW);
  digitalWrite(m2, HIGH);
}

void kiri_mati(){
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
}

void kanan_maju(){
  digitalWrite(m3, HIGH);
  digitalWrite(m4, LOW);
}

void kanan_mundur(){
  digitalWrite(m3, LOW);
  digitalWrite(m4, HIGH);
}

void kanan_mati(){
  digitalWrite(m3, LOW);
  digitalWrite(m4, LOW);
}

void mundur(){
  kiri_mundur();
  kanan_mundur();
}

void maju(){
  kanan_maju();
  kiri_maju();
}

void berhenti(){
  kanan_mati();
  kiri_mati();
}
