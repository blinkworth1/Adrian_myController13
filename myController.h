#include <Arduino.h>

class
  Switches {
  public:
    Switches (uint8_t, uint8_t, uint8_t, 
    uint8_t, uint8_t, uint8_t);
    Switches (uint8_t, uint8_t, uint8_t, 
    uint8_t, uint8_t, uint8_t, uint8_t, 
    uint8_t, uint8_t, uint8_t, uint8_t, 
    uint8_t, uint8_t);
    static void ReadWrite();
    void SetHandleB1ON (void (void));
    void SetHandleB1OFF (void (void));
    void SetHandleB2ON (void (void));
    void SetHandleB2OFF (void (void));
    void SetHandleB3ON (void (void));
    void SetHandleB3OFF (void (void));
    void SetHandleB4ON (void (void));
    void SetHandleB4OFF (void (void));
    void SetHandleB5ON (void (void));
    void SetHandleB5OFF (void (void));
    void SetHandleB6ON (void (void));
    void SetHandleB6OFF (void (void));
    void SetHandleB7ON (void (void));
    void SetHandleB7OFF (void (void));
    void SetHandleB8ON (void (void));
    void SetHandleB8OFF (void (void));
    void SetHandleB9ON (void (void));
    void SetHandleB9OFF (void (void));
    void SetHandleB10ON (void (void));
    void SetHandleB10OFF (void (void));
    void SetHandleB11ON (void (void));
    void SetHandleB11OFF (void (void));
    void SetHandleB12ON (void (void));
    void SetHandleB12OFF (void (void));
    void SetHandleB13ON (void (void));
    void SetHandleB13OFF (void (void));
uint8_t switchesPinTable [13];  
private:
    static void SwitchesRead();
    bool begin;
    uint8_t switchesArray [13] = {0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007};
    uint16_t switchesRaw;
    uint16_t switchesData;
    uint16_t bounce = 0; // bouncemask
    uint16_t pressed = 0; // pressedmask
    uint8_t numberOfSwitches;
    void (*pB1ON) (void) = NULL;
    void (*pB1OFF) (void) = NULL;
    void (*pB2ON) (void) = NULL;
    void (*pB2OFF) (void) = NULL;
    void (*pB3ON) (void) = NULL;
    void (*pB3OFF) (void) = NULL;
    void (*pB4ON) (void) = NULL;
    void (*pB4OFF) (void) = NULL;
    void (*pB5ON) (void) = NULL;
    void (*pB5OFF) (void) = NULL;
    void (*pB6ON) (void) = NULL;
    void (*pB6OFF) (void) = NULL;
    void (*pB7ON) (void) = NULL;
    void (*pB7OFF) (void) = NULL;
    void (*pB8ON) (void) = NULL;
    void (*pB8OFF) (void) = NULL;
    void (*pB9ON) (void) = NULL;
    void (*pB9OFF) (void) = NULL;
    void (*pB10ON) (void) = NULL;
    void (*pB10OFF) (void) = NULL;
    void (*pB11ON) (void) = NULL;
    void (*pB11OFF) (void) = NULL;
    void (*pB12ON) (void) = NULL;
    void (*pB12OFF) (void) = NULL;
    void (*pB13ON) (void) = NULL;
    void (*pB13OFF) (void) = NULL;
    void  (*switchesPointersOFF [13]) (void) {
      pB1OFF, pB2OFF, pB3OFF, pB4OFF, pB5OFF, pB6OFF,
              pB7OFF, pB8OFF, pB9OFF, pB10OFF, pB11OFF, pB12OFF, pB13OFF
    };
    void  (*switchesPointersON [13]) (void) {
      pB1ON, pB2ON, pB3ON, pB4ON, pB5ON, pB6ON,
             pB7ON, pB8ON, pB9ON, pB10ON, pB11ON, pB12ON, pB13ON
    };
};

class
  Rotary {
  public:
    Rotary (uint8_t, uint8_t );
    static uint8_t objectIndex = 0;
    static void ReadWrite();
    void SetHandleLeft (void (void));
    void SetHandleRight (void (void));
  private:
    static void RotaryRead();
    bool begin;
    void (*pLeft) (void) = NULL;
    void (*pRight) (void) = NULL;
	uint8_t RDDB;
	uint8_t rotaryA;
    uint8_t rotaryB;
    uint8_t rotaryData;
    int8_t rotaryState;
	uint8_t rotaryAraw;
	uint8_t rotaryBraw;
	uint8_t leftPin;
	uint8_t rightPin;
};
class
	Fader {
public:
	Fader (uint8_t, uint8_t);
	void begin(uint8_t, int, uint8_t, uint8_t, uint8_t);
	void Motor(int);
	void Halt();
	static void ReadWrite(); 
	static uint8_t objectIndex = 0;
	void SetHandleIncrease(void(int));
	void SetHandleDecrease(void(int));
	void SetHandleSame(void(int));
	void SetHandleTouchON(void(int));
	void SetHandleTouchOFF(void(int));
     
private:
bool MOTOR = false;	
static void FaderRead();
	void(*pIncrease) (int) = NULL;
	void(*pDecrease) (int) = NULL;
	void(*pSame) (int) = NULL;
	void(*pTouchON) (int) = NULL;
	void(*pTouchOFF) (int) = NULL;
	uint8_t wiperPin;
	uint8_t touchPin;
	uint8_t hyst;
	int touchthreshold;
	int hystPinRead;
	int currentPinRead = 600;
	int touchPinRead;
	int pwmPin;
	int dirPin;
	int dirPinU;
};
