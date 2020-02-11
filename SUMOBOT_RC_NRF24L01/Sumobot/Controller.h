
#define B_SELECT      15
#define B_START       6
#define B_ANALOG      4
#define B_PAD_UP      11
#define B_PAD_RIGHT   13
#define B_PAD_DOWN    14
#define B_PAD_LEFT    12
#define B_L2          9
#define B_R2          1
#define B_L1          10
#define B_R1          0
#define B_TRIANGLE    2
#define B_CIRCLE      3
#define B_CROSS       7
#define B_SQUARE      5



//Analog Value Tolerance
#define Tolerance   50


typedef struct package Package;

struct package {
    unsigned int pinValues;

    int Analog_LX = 512;
    int Analog_LY = 512;
    int Analog_RX = 512;
    int Analog_RY = 512;

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

    bool Button(byte btnId){
        return (pinValues >> btnId) & 1;
    }
};


void display_pin_values(Package &p)
{
    Serial.print("Pin States:\r\n");

    for(int i = 0; i < 16; i++)
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
