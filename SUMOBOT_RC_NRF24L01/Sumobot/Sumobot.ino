#include <SPI.h>  
#include "RF24.h"

#include "Controller.h"

#define led 2

#define m1 3 //kanan maju
#define m2 5 //kanan mundur
#define m3 6 //kiri maju
#define m4 9 //kiri mundur
#define m5 4 //relay


RF24 myRadio (8, 10);
byte addresses[][6] = {"0"};

Package data;

const int volt = A4; //voltmeter

int v;

float vout = 0.0;
float vin = 0.0;
float R1 = 1000 ; // resistansi R1 (100K)
float R2 = 10000.0; // resistansi R2 (10K)


void setup(){
  pinMode(m1, OUTPUT);
  pinMode(m2, OUTPUT);
  pinMode(m3, OUTPUT);
  pinMode(m4, OUTPUT);
  pinMode(m5, OUTPUT);
  
  pinMode(led, OUTPUT);
  
  pinMode(volt, INPUT);  
  
  digitalWrite(m1, LOW);
  digitalWrite(m2, LOW);
  digitalWrite(m3, LOW);
  digitalWrite(m4, LOW);
  digitalWrite(m5, LOW);

  Serial.begin(57600);
  
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS );
  
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
}

void loop() {
    while (myRadio.available()){
        myRadio.read(&data, sizeof(data));
        display_pin_values(data);
        Serial.println("Analog 1 X : " + String(data.Analog_LX) + ",   Y : " + String(data.Analog_LY));
        Serial.println("Analog 2 X : " + String(data.Analog_RX) + ",   Y : " + String(data.Analog_RY));\

        
        if(data.Analog_LY <= 350) {      //will be TRUE as long as button is pressed
            kiri();
        }
        else if(data.Analog_LY >= 700){
            kiri2();
        } else {
            kiri_berhenti();
        }
        
        if(data.Analog_RY <= 350){
            kanan2();
        }
        else if(data.Analog_RY >= 700){
            kanan();
        } else {
             kanan_berhenti();
        }
        
        if(data.Button(B_CIRCLE))
          digitalWrite(m5, HIGH);
        else
          digitalWrite(m5, LOW);
       
    }
    
    v = analogRead(volt);
    
    vout = (v * 5.0) / 1024.0;
    vin = vout / (R2/(R1+R2)); 
    
    if (vin <= 11.4)
      digitalWrite(led, HIGH);
    
    
    
    delay(1);  
}

void kiri(){  
    digitalWrite(m1, HIGH);
    digitalWrite(m2, LOW);
    Serial.println("Kiri");
}
void kiri2(){
    digitalWrite(m1, LOW);
    digitalWrite(m2, HIGH);
    Serial.print("Kiri2");
}
void kanan(){
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
    Serial.println("Kanan");
}
void kanan2(){
    digitalWrite(m3, LOW);
    digitalWrite(m4, HIGH);
    Serial.println("Kanan2");
}
void maju(){
    digitalWrite(m1, HIGH);
    analogWrite(m2, LOW);
    digitalWrite(m3, HIGH);
    digitalWrite(m4, LOW);
    Serial.println("Kiri");
}
void mundur(){
    digitalWrite(m1, LOW);
    analogWrite(m2, HIGH);
    digitalWrite(m3, LOW);
    digitalWrite(m4, HIGH);
    Serial.println("Kiri");
}
void berhenti(){
    kiri_berhenti();
    kanan_berhenti();
}
void kiri_berhenti(){
    digitalWrite(m1, LOW);
    digitalWrite(m2, LOW);
}
void kanan_berhenti(){
    digitalWrite(m3, LOW);
    digitalWrite(m4, LOW);
}
