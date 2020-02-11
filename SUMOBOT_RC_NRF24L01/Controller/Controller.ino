#include <SPI.h>  
#include "RF24.h"

// 74HC165 Config
#define NUMBER_OF_SHIFT_CHIPS   2
#define DATA_WIDTH              NUMBER_OF_SHIFT_CHIPS * 8

#define ploadPin        4
#define clockEnablePin  5
#define dataPin         7
#define clockPin        6

//Analog Value Tolerance
#define Tolerance   50

RF24 myRadio (9, 10);
byte addresses[][6] = {"0"};

typedef struct package Package;

struct package {
    unsigned int pinValues;

    int Analog_LX;
    int Analog_LY;
    int Analog_RX;
    int Analog_RY;

    bool isEqual(Package p){
        if (pinValues != p.pinValues) 
            return false;
        
        if (p.Analog_LX > Analog_LX + Tolerance || p.Analog_LX < Analog_LX - Tolerance)
            return false;
        if (p.Analog_LY > Analog_LY + Tolerance || p.Analog_LY < Analog_LY - Tolerance)
            return false;
        if (p.Analog_RX > Analog_RX + Tolerance || p.Analog_RX < Analog_RX - Tolerance)
            return false;
        if (p.Analog_RY > Analog_RY + Tolerance || p.Analog_RY < Analog_RY - Tolerance)
            return false;
        
        return true;
    }
};


Package data, newData;


unsigned int newPinValues;

void setup() {
    Serial.begin(115200);
     
    myRadio.begin();  
    myRadio.setChannel(115); 
    myRadio.setPALevel(RF24_PA_MAX);
    myRadio.setDataRate( RF24_250KBPS );
    myRadio.openWritingPipe(addresses[0]);

    pinMode(ploadPin, OUTPUT);
    pinMode(clockEnablePin, OUTPUT);
    pinMode(clockPin, OUTPUT);
    pinMode(dataPin, INPUT);

    digitalWrite(clockPin, LOW);
    digitalWrite(ploadPin, HIGH);
}

void loop() {
    
    newData.pinValues = read_shift_regs();
    read_analog_values(newData);
    
    if(!data.isEqual(newData))
    {
        data = newData;
        myRadio.write(&data, sizeof(data));
        Serial.print("*Pin value change detected*\r\n Size Of Data : ");
        Serial.println(sizeof(data));

        display_pin_values(data);
        
        Serial.println("Analog 1 X : " + String(data.Analog_LX) + ",   Y : " + String(data.Analog_LY));
        Serial.println("Analog 2 X : " + String(data.Analog_RX) + ",   Y : " + String(data.Analog_RY));
    
    }
    
    delay(25);
    
}

unsigned int read_shift_regs()
{
    long bitVal;
    unsigned int bytesVal = 0;
    digitalWrite(clockEnablePin, HIGH);
    digitalWrite(ploadPin, LOW);
    delayMicroseconds(5);
    digitalWrite(ploadPin, HIGH);
    digitalWrite(clockEnablePin, LOW);
    
    for(int i = 0; i < DATA_WIDTH; i++)
    {
        bitVal = digitalRead(dataPin);
        bytesVal |= (bitVal << ((DATA_WIDTH-1) - i));
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(5);
        digitalWrite(clockPin, LOW);
    }

    return(bytesVal);
}

void display_pin_values(Package &p)
{
    Serial.print("Pin States:\r\n");

    for(int i = 0; i < DATA_WIDTH; i++)
    {
        Serial.print("  Pin-" + String(i) + " : ");

        if((p.pinValues >> i) & 1)
            Serial.print("HIGH");
        else
            Serial.print("LOW");

        Serial.print("\r\n");
    }

    Serial.print("\r\n");
}

void read_analog_values(Package &p){
    p.Analog_LX = analogRead(A1);
    p.Analog_LY = analogRead(A0);
    p.Analog_RX = analogRead(A2);
    p.Analog_RY = analogRead(A3);
}
