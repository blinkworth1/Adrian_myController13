


class Fader
{
  public:
    void PositionUSBRead ();
    void PositionPinRead (int);
    void RW (int, int, int, int, uint8_t);
    void Mota (int, int);
    void Halt (int, int);
    void PinWrite ();

  private:
    uint16_t hystPinRead;
    uint16_t touchPinRead;
    uint16_t currentPinRead = 640;
    uint16_t currentUSBRead = 12;
    uint8_t readMSB;
    uint8_t readLSB;
    uint8_t sendMSB;
    uint8_t sendLSB;
    bool TOUCHSENT = false;
    bool PINUPDATED = true;
};

class Switches
{
  public:
    void RW (uint8_t, uint8_t);
    static volatile uint16_t pressed;
    static volatile uint16_t released;

  private:
    uint16_t mask = 0;
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
    //KEY_M is my key in the UI for 'escape' key...
    const int sendNotes [13] {0, 0, 0, 0, 0, 0, 0, 70, 71, 72, 73, 74, 75};
    const int sendFPPdata [13]{0,0,0,0,6,1,5,3,4,7,10,9,8}; ///last one needs to be changed to SHIFT
};

class Rotary
{
  public:
    uint8_t RW (uint8_t, uint8_t);
    uint8_t rotaryUpdate (uint8_t, uint8_t);
    static volatile bool rotaryState;
    static volatile uint16_t readout;
  private:
    int16_t readin;
    uint8_t count = 0;
    const int8_t enc_states[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
};

class Controller
{
  public:
    void Mode (int, int, int);
    void Loop ();

  private:
    uint8_t choice;
    uint8_t channel = 1;
    uint8_t oldchannel = 1;
    const uint8_t switchesPinTable [15] {2, 14, 7, 8, 6, 15, 22, 23, 18, 3, 4, 5, 11, 16, 17}; // list of PULLUP_SWITCHES
    int pwmPin = 20;  //If using the motor the pwm pin is 20, and the direction pin is 21
    int dirPin = 21;
    int wiperPin = A14;  //read fader value analog pin
    int touchPin = 19;  //capacitive touch fader pin.
    byte CHOICE [7] {240,127,127,127,0,0,247};
    Fader myFader;
    Rotary myRotary;
    Switches mySwitches;
};

