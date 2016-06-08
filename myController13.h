#include <Arduino.h>

#define NUM_SWITCHES 13

class Controller
{
  public: 
    void begin (int, int, int, int, int);
    volatile uint16_t switchesRaw = 0x000;
 

volatile uint8_t rotaryAraw = 0; 
uint8_t rotaryBraw = 0;
volatile uint8_t rotaryCount = 0x000;    
volatile uint8_t faderCount = 0x000;    
volatile uint16_t rotaryData = 0x000;
uint16_t currentUSBRead = 600;
    volatile uint16_t hystPinRead;
    volatile uint16_t touchPinRead;

    void switchesWrite ();
    void faderWrite ();
    void rotaryWrite();
    
    int wiperPin = A14;
    int touchPin = 33;

  private: 
void Motor();    
uint8_t switchesArray [13] = {0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007};
    uint16_t switchesData;       
    uint16_t pressed = 0; // mask
    
    const byte MMCSTOP [6]     {240, 127, 127, 6, 1, 247};
    const byte MMCPLAY [6]     {240, 127, 127, 6, 2, 247};
    const byte MMCFFWD [6]      {240, 127, 127, 6, 4, 247};
    const byte MMCRWD [6]     {240, 127, 127, 6, 5, 247};
    const byte MMCPUNCHIN [6]   {240, 127, 127, 6, 6, 247};
    const byte MMCPUNCHOUT[6]   {240, 127, 127, 6, 7, 247};
    const byte MMCPAUSE  [6]     {240, 127, 127, 6, 9, 247};
    const byte* MMCTransport [7] {MMCSTOP, MMCPLAY, MMCFFWD, MMCRWD, MMCPUNCHIN, MMCPUNCHOUT, MMCPAUSE};
    const int sendKeys [13] {KEY_DOWN, KEY_UP, KEY_LEFT, KEY_RIGHT, KEY_A, KEY_B, KEY_C, KEY_D, KEY_E, KEY_ENTER, KEY_M, KEY_F1, KEY_F5};
    //MAME keys for arcade control DEFENDER
    //KEY_M is my key in the MAME UI for 'escape' key...
    
    const int sendNotes [NUM_SWITCHES] {0, 0, 0, 0, 0, 0, 0, 70, 71, 72, 73, 74, 75};

    const int sendFPPdata [NUM_SWITCHES]{0,0,0,0,6,1,5,3,4,7,10,9,8}; //last one needs to be changed to SHIFT

    void faderHalt ();    
    uint16_t currentPinRead = 640;
    uint16_t oldPinRead = 640;
    bool touchActive=true;
    uint8_t sendMSB;
    uint8_t sendLSB;
    bool TOUCHSENT = false;
    uint8_t oldFaderCount; 
    int touch;
    int hyst;
    uint16_t oldUSBRead; 
uint8_t RDDB;
uint8_t currentRotaryCount; 
uint8_t oldRotaryCount; 
uint8_t rotaryA = 0; 
uint8_t rotaryB = 0;
const int8_t enc_states[16] {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
int rotaryState;
uint8_t choice;
    uint8_t channel = 1;
   // uint8_t oldchannel = 1;
   // byte CHOICE [7] {240,127,127,127,0,0,247};
  
  
    const uint8_t switchesPinTable [15] {22, 23, 9, 10, 13, 11, 12, 28, 27, 16, 17, 19, 18, 2, 14}; 
// 2 and 14 are the rotary inputs
    int switchesNo = NUM_SWITCHES;    
    int pwmPin = 21;
    int dirPin = 20;
    int dirPinU = 15;
};


