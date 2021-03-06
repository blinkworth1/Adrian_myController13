#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "MyRenderer.h"
#include "myController.h"
#include "FlashStorage.h"
#include "elapsedMillis.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BLEMIDI.h"

const unsigned char mybitmap [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x7F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x4F, 0xE0, 0x03, 0xFC, 0x3F, 0xE0, 0x00, 0x07, 0xFE, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x7F, 0xCF, 0xE0, 0x0F, 0xFC, 0x3F, 0xF8, 0x00, 0x0F, 0xFF, 0x80, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xCF, 0xE0, 0x3F, 0xFC, 0x3F, 0xFC, 0x00, 0x3F, 0xFF, 0xC0, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xFF, 0xCF, 0xE0, 0x7F, 0xFC, 0x3F, 0xFE, 0x00, 0x7F, 0xFF, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xFF, 0xCF, 0xE0, 0xFF, 0xFC, 0x3F, 0xFF, 0x00, 0xFF, 0xFF, 0xF0, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xFF, 0xCF, 0xE1, 0xFF, 0xFC, 0x3F, 0xFF, 0x80, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xFF, 0xCF, 0xE1, 0xFF, 0x00, 0x00, 0xFF, 0xC1, 0xFF, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0xFF, 0xCF, 0xE3, 0xFC, 0x00, 0x00, 0x3F, 0xC1, 0xFF, 0x07, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x3F, 0xE0, 0xCF, 0xE3, 0xF8, 0x00, 0x00, 0x1F, 0xE3, 0xFC, 0x01, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x3F, 0xC0, 0x0F, 0xE7, 0xF0, 0x00, 0x00, 0x0F, 0xE3, 0xF8, 0x01, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x80, 0x0F, 0xE7, 0xF0, 0x00, 0x00, 0x0F, 0xE3, 0xF8, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x80, 0x0F, 0xE7, 0xFF, 0xFC, 0x3F, 0xFF, 0xE3, 0xF8, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x0F, 0xE7, 0xFF, 0xFC, 0x3F, 0xFF, 0xE3, 0xF0, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x0F, 0xE7, 0xFF, 0xFC, 0x3F, 0xFF, 0xE3, 0xF0, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x0F, 0xE7, 0xFF, 0xFC, 0x3F, 0xFF, 0xE3, 0xF0, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x0F, 0xE7, 0xFF, 0xFC, 0x3F, 0xFF, 0xE3, 0xF0, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x00, 0x0F, 0xE7, 0xFF, 0xFC, 0x3F, 0xFF, 0xE3, 0xF0, 0x00, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0x80, 0x1F, 0xE7, 0xF0, 0x00, 0x00, 0x0F, 0xE3, 0xF0, 0x01, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x3F, 0x80, 0x1F, 0xC3, 0xF8, 0x00, 0x00, 0x0F, 0xE3, 0xF0, 0x03, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x3F, 0xC0, 0x3F, 0xC3, 0xFC, 0x00, 0x00, 0x1F, 0xC3, 0xF3, 0x0F, 0xFC, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0xF0, 0xFF, 0xC3, 0xFE, 0x00, 0x00, 0x3F, 0xC3, 0xF3, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x1F, 0xFF, 0xFF, 0x81, 0xFF, 0xFC, 0x3F, 0xFF, 0x83, 0xF3, 0xFF, 0xF8, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xFF, 0xFF, 0x00, 0xFF, 0xFC, 0x3F, 0xFF, 0x83, 0xF3, 0xFF, 0xF0, 0x00, 0x00,
  0x00, 0x00, 0x0F, 0xFF, 0xFF, 0x00, 0xFF, 0xFC, 0x3F, 0xFF, 0x03, 0xF3, 0xFF, 0xE0, 0x00, 0x00,
  0x00, 0x00, 0x07, 0xFF, 0xFE, 0x00, 0x7F, 0xFC, 0x3F, 0xFE, 0x03, 0xF3, 0xFF, 0xC0, 0x00, 0x00,
  0x00, 0x00, 0x03, 0xFF, 0xFC, 0x00, 0x1F, 0xFC, 0x3F, 0xFC, 0x03, 0xF3, 0xFF, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0xFF, 0xF0, 0x00, 0x0F, 0xFC, 0x3F, 0xF0, 0x03, 0xF2, 0xFC, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x3F, 0xC0, 0x00, 0x01, 0xFC, 0x3F, 0x80, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xF2, 0x3F, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xF2, 0x3F, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xE3, 0xF2, 0x3F, 0xFE, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFE, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x60, 0xD8, 0x1C, 0x3C, 0x0E, 0x0D, 0x83, 0x60, 0x78, 0x6C, 0x1E, 0x06, 0x00, 0x00,
  0x00, 0x00, 0x60, 0xD8, 0x3C, 0x3C, 0x1F, 0x0D, 0x87, 0xF0, 0x60, 0x6C, 0x1E, 0x0F, 0x00, 0x00,
  0x00, 0x00, 0x60, 0xF8, 0x30, 0x18, 0x1B, 0x0D, 0x87, 0xF0, 0x70, 0x7C, 0x0C, 0x0C, 0x00, 0x00,
  0x00, 0x00, 0x60, 0xF8, 0x18, 0x18, 0x1F, 0x0D, 0x86, 0xB0, 0x70, 0x7C, 0x0C, 0x06, 0x00, 0x00,
  0x00, 0x00, 0x60, 0xF8, 0x3C, 0x18, 0x1E, 0x0F, 0x86, 0xB0, 0x60, 0x7C, 0x0C, 0x0F, 0x00, 0x00,
  0x00, 0x00, 0x60, 0xD8, 0x38, 0x18, 0x1B, 0x07, 0x06, 0xB0, 0x78, 0x6C, 0x0C, 0x0E, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

#define pwm 10 // pwm pin for leds
#define OLED_DATA   20 //i2c pins for display
#define OLED_CLK    21
#define OLED_RESET  5
Fader slider1 (A8, 9); //aref and jitter suppression amount
Fader slider2 (A2, 9);
Fader slider3 (A3, 9);
Fader slider4 (A4, 9);
Rotary encoder1 (19, 0); // left and right
Switches Buttons (6); //the mux data pin
Adafruit_BluefruitLE_SPI ble(8, 7, 4); //these are internal connections, don't worry about them.
Adafruit_BLEMIDI midi(ble);
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 * dptr = &display;
MyRenderer my_renderer (dptr);
MenuSystem ms(my_renderer);
elapsedMillis switchesPressTimer;
//MIDI_CREATE_INSTANCE (HardwareSerial, Serial1, midiA);

/*Forward Declarations*/
class Base;
Base * currentDataPointer;
void fademoveDisplayUpdate(void);
void buttpressDisplayUpdate (void);
void editMenuDisplayUpdate();
void CCbleTXmidi (int, int);
void connected(void);
void disconnected(void);
void presetDisplayUpdate (void);
void on_itemGLOBAL_selected(MenuItem* p_menu_item);
void on_itemLED_selected(MenuItem* p_menu_item);
void on_item0_selected(MenuItem* p_menu_item);
void on_item1_selected(MenuItem* p_menu_item);
void on_item2_selected(MenuItem* p_menu_item);
void on_item3_selected(MenuItem* p_menu_item);
void on_itemLINE6_selected(MenuItem* p_menu_item);
void on_itemAXE_selected(MenuItem* p_menu_item);
void on_item6_selected(MenuItem* p_menu_item);
void on_item7_selected(MenuItem* p_menu_item);
void on_item8_selected(MenuItem* p_menu_item);
void on_item9_selected(MenuItem* p_menu_item);
void on_item10_selected(MenuItem* p_menu_item);
void on_item11_selected(MenuItem* p_menu_item);
void on_item12_selected(MenuItem* p_menu_item);
void on_item13_selected(MenuItem* p_menu_item);
void on_item14_selected(MenuItem* p_menu_item);
void on_item15_selected(MenuItem* p_menu_item);
void SelectPress (void);
void SelectRelease (void);
void Left (void);
void Right (void);
void EditPress (void);
void EditRelease (void);
void Stomp1ON(void);
void Stomp2ON(void);
void Stomp3ON(void);
void Stomp4ON(void);
void Tog1ON(void);
void Tog2ON(void);
void slider1Inc (int);
void slider1Dec (int);
void slider2Inc (int);
void slider2Dec (int);
void slider3Inc (int);
void slider3Dec (int);
void slider4Inc (int);
void slider4Dec (int);
void slider1SAME (int);
void slider2SAME (int);
void slider3SAME (int);
void slider3SAME (int);

const char buttOn [] = "ON";
const char buttOff [] = "OFF";
const char *buttOnOff [4] {buttOff, buttOff, buttOff, buttOff};
int faderValue [4] = {0, 0, 0, 0};

bool SCENE;

class State {
  public:
    virtual void execute1() {};
    virtual void execute2() {};
    virtual void execute3() {};
    virtual void execute4() {};
    virtual void execute5() {};
        virtual void execute6() {};
    uint8_t identifier;
};

class state0 : public State {
  public:
    state0 () {
      identifier = 0;
    }
    void execute1();
    void execute2();
    void execute3();
    void execute4();
} state0;

class state1 : public State {
  public:
    state1 () {
      identifier = 1;
    }
    void execute1();
    void execute2();
    void execute3();
    void execute4();
} stateOne;

class state2 : public State {
  public:
    state2 () {
      identifier = 2;
    }
    void execute6(); //select release in preset/SCENE mode
} stateTwo;


class state3 : public State {
  public:
    state3 () {
      identifier = 3;
    }
    void execute5();
} stateThree;

class state4 : public State {
  public:
    state4 () {
      identifier = 4;
    }
    void execute1();
    void execute2();
    void execute3();
    void execute4();
} stateFour;

class state6 : public State {
  public:
    state6 () {
      identifier = 6;
    }
    void execute1();
    void execute2();
    void execute3();
    void execute4();
} stateSix;

State * currentState;// = &stateOne;

struct Settings {
  bool valid;
  float msdelay;
  int channel;
  int program;
  int CCnumber [8];
  int rotary1mod;
  uint8_t preset;
  uint8_t scene;
  uint8_t sceneCC;
} storedSettings = {true, 1.0, 1, 0, {21, 22, 23, 24, 31, 32, 33, 34}, 20, 21, 1, 34};

FlashStorage(my_flash_store, Settings);
int updateprogram = storedSettings.program;
uint8_t lcount = 0;
uint8_t rcount = 0;
bool INIT2 [] = {true, true, true, true};
bool isConnected = false;
const int alpha [] {65, 66, 67, 68};

class Base {
  public :
    virtual void plus () {
      updateprogram++;
      if (updateprogram >= 128) {
        updateprogram = 0;
      }
    }
    virtual void minus () {
      updateprogram--;
      if (updateprogram <= -1) {
        updateprogram = 127;
      }
    }
    virtual void select() = 0;
    virtual void store() = 0;
    virtual void presetSelect() {};
    virtual void currentPreset() {};
    char *heading;
    char *description;
    char *format;
    uint8_t identifier;
    bool current;
    virtual void bignumberstored () {}
    virtual void bignumber() {}
};

template <class T> class Data : public Base {
  public:
    T Update;
    void peripheralDisplayUpdate (T Stored) {
      display.clearDisplay();
      display.setFont ();
      display.setCursor(0, 4);
      display.setTextSize(1);
      display.printf("%s%s\n", heading, description);
      display.setCursor(0, 47);
      display.setFont (&FreeMono24pt7b);
      display.printf (format, Update);
      //display.printf ("%03d",1);
      if (current) {
        display.setFont ();
        display.setCursor(85, 23);
        display.setTextSize(1);
        display.print ("current");
        display.setCursor(83, 47);
        display.setFont (&FreeMono12pt7b);
        display.printf (format, Stored);
      }
      display.display();
    }
};

class Control: public Data<int> {
  public:
    Control (uint8_t _identifier, char * _heading, char * _description, char * _format, int _Update, bool _current) {
      identifier = _identifier;
      heading = _heading;
      description = _description;
      format = _format;
      Update = _Update;
      current = _current;
    }
    virtual void plus () {
      Update++;
      if (Update >= 128) {
        Update = 0;
      }
    }
    virtual void minus () {
      Update--;
      if (Update <= -1) {
        Update = 127;
      }
    }
    virtual void select () {
      description = " CC SELECT";
      current = true;
      peripheralDisplayUpdate(storedSettings.CCnumber[identifier]);
    }
    virtual void store () {
      description = " CC STORED";
      current = false;
      storedSettings.CCnumber [identifier] = Update;
      my_flash_store.write(storedSettings);
      peripheralDisplayUpdate(storedSettings.CCnumber[identifier]);
    }
};

Control fader1 (4, "FADER 1", " SELECT", "%03d", storedSettings.CCnumber[4], true);
Control fader2 (5, "FADER 2", " SELECT", "%03d", storedSettings.CCnumber[5], true);
Control fader3 (6, "FADER 3", " SELECT", "%03d", storedSettings.CCnumber[6], true);
Control fader4 (7, "FADER 4", " SELECT", "%03d", storedSettings.CCnumber[7], true);
Control stomp1 (0, "STOMP 1", " SELECT", "%03d", storedSettings.CCnumber[0], true);
Control stomp2 (1, "STOMP 2", " SELECT", "%03d", storedSettings.CCnumber[1], true);
Control stomp3 (2, "STOMP 3", " SELECT", "%03d", storedSettings.CCnumber[2], true);
Control stomp4 (3, "STOMP 4", " SELECT", "%03d", storedSettings.CCnumber[3], true);

class MIDIChannel: public Data<int> {
  public:
    MIDIChannel (uint8_t _identifier, char * _heading, char * _description, char * _format, int _Update, bool _current) {
      identifier = _identifier;
      heading = _heading;
      description = _description;
      format = _format;
      Update = _Update;
      current = _current;
    }
    virtual void plus () {
      Update++;
      if (Update >= 17) {
        Update = 1;
      }
    }
    virtual void minus () {
      Update--;
      if (Update <= 0) {
        Update = 16;
      }
    }
    virtual void select () {
      description = " SELECT";
      current = true;
      peripheralDisplayUpdate(storedSettings.channel);
    }
    virtual void store () {
      description = " STORED";
      current = false;
      storedSettings.channel = Update;
      my_flash_store.write(storedSettings);
      peripheralDisplayUpdate(storedSettings.channel);
    }
} midi_channel (10, "MIDI CHANNEL", " SELECT", "%02d", storedSettings.channel, true);

class GlobalDelay: public Data<float> {
  public:
    GlobalDelay (uint8_t _identifier, char * _heading, char * _description, char * _format, float _Update, bool _current) {
      identifier = _identifier;
      heading = _heading;
      description = _description;
      format = _format;
      Update = _Update;
      current = _current;
    }
    virtual void plus () {
      Update += 0.1;
      if (Update >= 2.0) {
        Update = 2.0;
      }
    }
    virtual void minus () {
      Update -= 0.1;
      if (Update <= 0.05) {
        Update = 0.0;
      }
    }
    virtual void select () {
      description = "";
      current = true;
      peripheralDisplayUpdate(storedSettings.msdelay);
    }
    virtual void store () {
      description = "";
      current = false;
      storedSettings.msdelay = Update;
      my_flash_store.write(storedSettings);
      peripheralDisplayUpdate(storedSettings.msdelay);
    }
} update_delay (11, "UPDATE DELAY (sec)", "", "%.1f", storedSettings.msdelay, true);

class Lights : public Data<int> {
  public:
    Lights (uint8_t _identifier, char * _heading, char * _description, char * _format, int _Update, bool _current) {
      identifier = _identifier;
      heading = _heading;
      description = _description;
      format = _format;
      Update = _Update;
      current = _current;
    }
    virtual void plus () {
      Update++;
      if (Update >= 20) {
        Update = 20;
      }
    }
    virtual void minus () {
      Update--;
      if (Update <= 0) {
        Update = 0;
      }
    }
    virtual void select () {
      analogWrite (pwm, Update * 10);
      description = " SELECT";
      current = true;
      peripheralDisplayUpdate(storedSettings.rotary1mod);
    }
    virtual void store () {
      description = " STORED";
      current = false;
      storedSettings.rotary1mod = Update;
      my_flash_store.write(storedSettings);
      peripheralDisplayUpdate(storedSettings.rotary1mod);
    }
} led_brightness (12, "LED BRIGHTNESS", " SELECT", "%02d", storedSettings.rotary1mod, true);

class Scene : public Data<int> {
  public:
    Scene (uint8_t _identifier, int _Update) {
      identifier = _identifier;
      Update = _Update;
    }
    
    void plus () {
      if (SCENE) {Update++;
      if (Update >= 9) {
        Update = 1;
      }
      } else{updateprogram++;
      if (updateprogram >= 128) {
        updateprogram = 0;
      }
      }
    }
    void minus () {
      if (SCENE){
      Update--;
      if (Update <= 0) {
        Update = 8;
      }
      } else {updateprogram--;
      if (updateprogram <= -1) {
        updateprogram = 127;
      }
      }
    }
    void presetSelect () {
      display.clearDisplay();
      display.setFont ();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.printf("%s", "PRESET");
      display.setCursor(0, 12);
      display.printf("%s", "FORMAT SELECTED:");
      display.setCursor(0, 34);
      display.setFont (&FreeMono9pt7b);
        display.println ("001 - 128 + scenes");
      display.display();
      if (storedSettings.msdelay > 0.05) {
        delay (storedSettings.msdelay * 1000);
        for (int i = 0; i < 4; i++) {
          CCbleTXmidi (i + 4, faderValue[i]);
        }
      }
    } 
    void currentPreset (){
      display.clearDisplay();
      display.setFont ();
      display.setCursor(11, 4);
      display.setTextSize(1);
      display.println ("- CURRENT PRESET -");
      display.setCursor(11, 50);
      display.setFont (&FreeMono24pt7b);
      display.printf ("%03d%s%d", storedSettings.program + 1, ":", Update);
      display.display();
      updateprogram = storedSettings.program;
      if (storedSettings.msdelay > 0.05) {
        delay (storedSettings.msdelay * 1000);
        for (int i = 0; i < 4; i++) {
          CCbleTXmidi (i + 4, faderValue[i]);
        }
      }
    }
    void select () {
      display.clearDisplay();
      display.setFont ();
      display.setTextColor(WHITE);
      display.setTextSize(1);
      if(SCENE) {
      display.setCursor(40, 4);
      display.println("SCENE SELECT:");
      }
      else {display.setCursor(0, 4);
      display.println("SELECT PRESET:");
      }
      display.setCursor(0, 50);
      display.setFont (&FreeMono24pt7b);
      display.printf ("%03d%s%d", storedSettings.program + 1, ":", Update);
      display.display();
      
    }
    void store () {currentPreset();}
} scene (25, storedSettings.scene);

class PresetControl : public Base {
  public:
    void peripheralDisplayUpdate () {
      display.clearDisplay();
      display.setFont ();
      display.setCursor(0, 4);
      display.setTextColor(WHITE);
      display.setTextSize(1);
      display.println("SELECT NEXT PRESET:");
      display.setCursor(0, 50);
      bignumber ();
      display.setFont ();
      display.setCursor(85, 23);
      display.setTextSize(1);
      display.print ("current");
      display.setCursor(91, 33);
      display.setTextSize(1);
      display.print ("preset");
      display.setCursor(84, 55);
      smallnumber ();
      display.display();
    }
    virtual void currentPreset () {
      display.clearDisplay();
      display.setFont ();
      display.setCursor(11, 4);
      display.setTextSize(1);
      display.println ("- CURRENT PRESET -");
      display.setCursor(22, 50);
      bignumberstored();
      display.display();
      updateprogram = storedSettings.program;
      if (storedSettings.msdelay > 0.05) {
        delay (storedSettings.msdelay * 1000);
        for (int i = 0; i < 4; i++) {
          CCbleTXmidi (i + 4, faderValue[i]);
        }
      }
    }
    virtual void presetSelect () {
      display.clearDisplay();
      display.setFont ();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.printf("%s", "PRESET");
      display.setCursor(0, 12);
      display.printf("%s", "FORMAT SELECTED:");
      display.setCursor(0, 34);
      display.setFont (&FreeMono9pt7b);
      display.println(heading);
      display.display();
    }
    virtual void bignumber () {}
    virtual void smallnumber () {}
};

class Preset1 : public PresetControl {
  public:
    Preset1 (uint8_t _identifier, char * _heading, char * _format) {
      identifier = _identifier;
      heading = _heading;
      format = _format;
    }
    void select () {
      peripheralDisplayUpdate();
    }
    void store () {
      currentPreset();
    }
    virtual void bignumber () {
      display.setFont (&FreeMono24pt7b);
      display.printf (format, updateprogram);
    }
    virtual void smallnumber () {
      display.setFont (&FreeMono12pt7b);
      display.printf (format, storedSettings.program);
    }
    virtual void bignumberstored () {
      display.setFont (&FreeMono24pt7b);
      display.printf (format, storedSettings.program);
    }
} preset1 (21, "000 - 127", "%03d");

class Preset2 : public PresetControl {
  public:
    Preset2 (uint8_t _identifier, char * _heading, char * _format) {
      identifier = _identifier;
      heading = _heading;
      format = _format;
    }
    void select () {
      peripheralDisplayUpdate();
    }
    void store () {
      //if (SCENE) {
      //  currentDataPointer = &scene;
      //  scene.select();
     // } else
     // {
        currentPreset();
      //}
    }
    virtual void bignumber () {
      display.setFont (&FreeMono24pt7b);
      display.printf (format, updateprogram + 1);
    }
    virtual void smallnumber () {
      display.setFont (&FreeMono12pt7b);
      display.printf (format, storedSettings.program + 1);
    }
    virtual void bignumberstored () {
      display.setFont (&FreeMono24pt7b);
      display.printf (format, storedSettings.program + 1);
    }
    /*void presetSelect () {
      display.clearDisplay();
      display.setFont ();
      display.setCursor(0, 0);
      display.setTextSize(1);
      display.printf("%s", "PRESET");
      display.setCursor(0, 12);
      display.printf("%s", "FORMAT SELECTED:");
      display.setCursor(0, 34);
      display.setFont (&FreeMono9pt7b);
      if (!SCENE) {
        display.println(heading);
      } else {
        display.println ("001 - 128 + scenes");
      }
      display.display();
      if (storedSettings.msdelay > 0.05) {
        delay (storedSettings.msdelay * 1000);
        for (int i = 0; i < 4; i++) {
          CCbleTXmidi (i + 4, faderValue[i]);
        }
      }
    }*/
} preset2 (22, "001 - 128", "%03d");

class Preset3 : public PresetControl {
  public:
    Preset3 (uint8_t _identifier, char * _heading, char * _format) {
      identifier = _identifier;
      heading = _heading;
      format = _format;
    }
    void select () {
      peripheralDisplayUpdate();
    }
    void store () {
      currentPreset();
    }
    virtual void plus () {
      updateprogram++;
      if (updateprogram >= 32) {
        updateprogram = 0;
      }
    }
    virtual void minus () {
      updateprogram--;
      if (updateprogram <= -1) {
        updateprogram = 31;
      }
    }
    virtual void bignumber () {
      display.setFont (&FreeMono24pt7b);
      int number = (updateprogram + 4) / 4;
      int letter = (updateprogram + 4) % 4;
      display.printf (format, number, alpha [letter]);
    }
    virtual void smallnumber () {
      display.setFont (&FreeMono12pt7b);
      int number = (storedSettings.program + 4) / 4;
      int letter = (storedSettings.program + 4) % 4;
      display.printf (format, number, alpha [letter]);
    }
    virtual void bignumberstored () {
      display.setFont (&FreeMono24pt7b);
      int number = (storedSettings.program + 4) / 4;
      int letter = (storedSettings.program + 4) % 4;
      display.printf (format, number, alpha [letter]);
    }
} preset3 (23, "1A - 8D", "%d%c");

class Preset4 : public PresetControl {
  public:
    Preset4 (uint8_t _identifier, char * _heading, char * _format) {
      identifier = _identifier;
      heading = _heading;
      format = _format;
    }
    virtual void select () {
      peripheralDisplayUpdate();
    }
    virtual void store () {
      currentPreset();
    }
    virtual void bignumber () {
      display.setFont (&FreeMono24pt7b);
      int number = (updateprogram + 4) / 4;
      int letter = (updateprogram + 4) % 4;
      display.printf (format, number, alpha [letter]);
    }
    virtual void smallnumber () {
      display.setFont (&FreeMono12pt7b);
      int number = (storedSettings.program + 4) / 4;
      int letter = (storedSettings.program + 4) % 4;
      display.printf (format, number, alpha [letter]);
    }
    virtual void bignumberstored () {
      display.setFont (&FreeMono24pt7b);
      int number = (storedSettings.program + 4) / 4;
      int letter = (storedSettings.program + 4) % 4;
      display.printf (format, number, alpha [letter]);
    }
} preset4 (24, "1A - 32D", "%d%c");

Base * cPParray[] = {&preset1, &preset2, &preset3, &preset4, &scene};

/*void Scene :: store () {
  storedSettings.scene = Update;
  my_flash_store.write(storedSettings);
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 4);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("SCENE DATA SENT:");
  display.setCursor(25, 40);
  display.setFont (&FreeMono12pt7b);
  display.printf ("%03d%s%d", storedSettings.program + 1, ":", Update);
  display.display();
  if (isConnected) {
    midi.send(0xB0 + (storedSettings.channel - 1), storedSettings.sceneCC, Update);
  }
}*/

void state0 :: execute1 () {// state0 is preset/SCENE select mode, execute1 is leftrotary
  currentDataPointer->minus();
  currentDataPointer->select();
  }
  void state0 :: execute2 () {// state0 is preset/SCENE select mode, execute2 is rightrotary
  currentDataPointer->plus();
  currentDataPointer->select();
  }
void state0 :: execute3 () { //press select button is execute3
  switchesPressTimer = 0;
  display.clearDisplay();
  display.display();
  currentState = &stateTwo; //timer mode for select button in preset/SCENE select mode
}
  void state0 :: execute4 () { //press edit button is execute4
  switchesPressTimer = 0;
  display.clearDisplay();
  display.display();
  currentState = &stateThree; //timer mode for preset-to-menu mode
}

void state1 :: execute1 () {// state1 is preset select mode, execute1 is leftrotary
  currentDataPointer->minus();
  currentDataPointer->select();
}
void state1 :: execute2 () { //execute2 is rightrotary
  currentDataPointer->plus();
  currentDataPointer->select();
}
void state1 :: execute3 () { //press select button is execute3
  storedSettings.program = updateprogram;
  if (isConnected) {
    midi.send(0xC0 + (storedSettings.channel - 1), storedSettings.program, storedSettings.program);
  }
  my_flash_store.write(storedSettings);
  // if (storedSettings.msdelay > 0.05) {
  //   delay (storedSettings.msdelay * 1000);
  //   for (int i = 0; i < 4; i++) {
  //     CCbleTXmidi (i + 4, faderValue[i]);
  //   }
  // }
  currentDataPointer->store();
}
void state1 :: execute4 () { //press edit button is execute4
  switchesPressTimer = 0;
  display.clearDisplay();
  display.display();
  currentState = &stateThree; //timer mode
}

void state2 :: execute6 () { //timer mode, release select button is execute6
  int time = switchesPressTimer - 2500;
  if (time < 0) {
    delay (200);
    storedSettings.program = updateprogram;
    storedSettings.scene = scene.Update;
  if (isConnected) {
    midi.send(0xC0 + (storedSettings.channel - 1), storedSettings.program, storedSettings.program);
    midi.send(0xB0 + (storedSettings.channel - 1), storedSettings.sceneCC, storedSettings.scene);
  }
  my_flash_store.write(storedSettings);
    currentDataPointer->store(); // store where we are at, short press
    //
  }
  else {
    SCENE = !SCENE; // toggle preset/SCENE on a long press
  // go back to preset/SCENE select mode
  }
  currentState = &state0; 
}

void state3 :: execute5 () { //timer mode, release edit button is execute5
  int time = switchesPressTimer - 2500;
  if (time < 0) {
    delay (200);
    for (int i = 0; i < 4; i++) {
      CCbleTXmidi (i + 4, faderValue[i]);
    }
    display.clearDisplay();
    display.setFont();
    display.setTextSize(2);
    display.setCursor(25, 22);
    display.println("faders");
    display.display();
    delay (300);
    display.clearDisplay();
    delay (70);
    display.setCursor(22, 22);
    display.println("updated");
    display.display();
    delay(300);
    
    if (currentDataPointer->identifier == 25) {currentState = &state0;} //25 is scene dataset and state0 is the mode
    else{
    currentState = &stateOne;}
    fademoveDisplayUpdate();
    //currentDataPointer->currentPreset();
  }
  else {
    currentState = &stateFour;
    ms.reset();
    editMenuDisplayUpdate();
  }
}

void state4 :: execute1 () { //state4 is menu ...execute1 is leftrotary
  ms.prev ();
  editMenuDisplayUpdate();
}
void state4 :: execute2 () { //execute2 is rightrotary
  ms.next ();
  editMenuDisplayUpdate();
}
void state4 :: execute3 () { //execute3 is the select button
  ms.select(); //ms.select menu select will mean, if a bottom level menu item is selected, an exit out of state4/menu ..
  if (currentState->identifier == 4) { //update/display menu if still in state4/menu
    editMenuDisplayUpdate ();
  }
}
void state4 :: execute4 () { // execute4 is edit button
  if ((ms._p_curr_menu == ms._p_root_menu)) {//if top level menu, exit and go to state1/preset select mode
    ms.reset();
    currentDataPointer = cPParray[storedSettings.preset - 21];
    currentDataPointer->currentPreset();
    if (currentDataPointer->identifier == 25) {currentState = &state0;} else {currentState = &stateOne;}
  }
  else { //else still in menu, so back up and update
    ms.back();
    editMenuDisplayUpdate();
  }
}

void state6 :: execute1 () {
  currentDataPointer->minus();
  currentDataPointer->select();
}
void state6 :: execute2 () {
  currentDataPointer->plus();
  currentDataPointer->select();
}
void state6 :: execute3 () {
  currentDataPointer->store();
  delay (2000);
  currentState = &stateFour;
  editMenuDisplayUpdate ();
}
void state6 :: execute4( ) {
  currentState = &stateFour;
  editMenuDisplayUpdate ();
}

/*Menu structure*/
Menu mu1("PRESET FORMAT");
Menu mu2("CC SELECT");
Menu mu5("STOMP CC SELECT");
Menu mu6("FADER CC SELECT");
Menu mu7("SCENE CC SELECT");
MenuItem mm_mi0 ("AUTO UPDATE DELAY", &on_itemGLOBAL_selected);
MenuItem mm_mi1 ("MIDI CHANNEL", &on_item0_selected);
MenuItem mm_mi2 ("LED BRIGHTNESS", &on_itemLED_selected);
MenuItem mu1_mi1("000 - 127", &on_item1_selected);
MenuItem mu1_mi2("001 - 128", &on_item2_selected);
MenuItem mu1_mi3("1A - 8D", &on_item3_selected);
MenuItem mu1_mi6("1A - 32D", &on_itemLINE6_selected);
MenuItem mu1_mi7("001 - 128 + scenes", &on_itemAXE_selected);
MenuItem mu2_mi1("STOMP 1", &on_item6_selected);
MenuItem mu2_mi2("STOMP 2", &on_item7_selected);
MenuItem mu2_mi3("STOMP 3", &on_item8_selected);
MenuItem mu2_mi4("STOMP 4", &on_item9_selected);
MenuItem mu3_mi1("FADER 1", &on_item10_selected);
MenuItem mu3_mi2("FADER 2", &on_item11_selected);
MenuItem mu3_mi3("FADER 3", &on_item12_selected);
MenuItem mu3_mi4("FADER 4", &on_item13_selected);
MenuItem mu4_mi1("AXE FX scene CC 34", &on_item14_selected);
MenuItem mu4_mi2("LINE 6 scene CC 69", &on_item15_selected);

void setup() {
  
  /*PINmodes*/
  pinMode(6, INPUT_PULLUP ); //datapin for mux
  pinMode(19, INPUT_PULLUP ); //rotary
  pinMode(0, INPUT_PULLUP ); //rotary
  pinMode(15, OUTPUT);  //A all off is select aka first position aka 0 pin ...  A on only is edit aka second position aka 1 pin
  pinMode(11, OUTPUT ); //B 
  pinMode(12, OUTPUT ); //C
  
  /*BLE setup and debug*/
  Serial.begin(38400);
  delay(500);
  ble.begin(false); // If set to 'true' enables debug output
  ble.echo(false);
  midi.begin(true);
  ble.verbose(false);

  /*FlashStorage management*/
  storedSettings = my_flash_store.read();
  if (!(storedSettings.valid)) {
    storedSettings = {true, 1.0, 1, 0, {21, 22, 23, 24, 31, 32, 33, 34}, 20, 21, 1, 34};
    currentDataPointer = cPParray[storedSettings.preset - 20];
  }
  else {
    fader1.Update = storedSettings.CCnumber[4];
    fader2.Update = storedSettings.CCnumber[5];
    fader3.Update = storedSettings.CCnumber[6];
    fader4.Update = storedSettings.CCnumber[7];
    stomp1.Update = storedSettings.CCnumber[0];
    stomp2.Update = storedSettings.CCnumber[1];
    stomp3.Update = storedSettings.CCnumber[2];
    stomp4.Update = storedSettings.CCnumber[3];
    midi_channel.Update = storedSettings.channel;
    update_delay.Update = storedSettings.msdelay;
    led_brightness.Update = storedSettings.rotary1mod;
    updateprogram = storedSettings.program;
    scene.Update = storedSettings.scene;
    currentDataPointer = cPParray[storedSettings.preset - 21];
  }
  if (currentDataPointer->identifier == 25) {currentState = &state0;} else {currentState = &stateOne;}; 
  analogWrite (pwm, storedSettings.rotary1mod * 10);
  // midiA.begin();

  /*set callbacks*/
  ble.setConnectCallback(connected);
  ble.setDisconnectCallback(disconnected);
  encoder1.SetHandleLeft (Left);
  encoder1.SetHandleRight (Right);
  Buttons.SetHandleB1ON (SelectPress);
  Buttons.SetHandleB1OFF (SelectRelease);
  Buttons.SetHandleB2ON (EditPress);
  Buttons.SetHandleB2OFF (EditRelease);
  Buttons.SetHandleB3ON (Stomp1ON);
  Buttons.SetHandleB4ON (Stomp2ON);
  Buttons.SetHandleB5ON (Stomp3ON);
  Buttons.SetHandleB6ON (Stomp4ON);
  Buttons.SetHandleB7ON (Tog1ON);
  Buttons.SetHandleB8ON (Tog2ON);
  slider1.SetHandleIncrease (slider1Inc);
  slider1.SetHandleDecrease (slider1Dec);
  slider2.SetHandleIncrease (slider2Inc);
  slider2.SetHandleDecrease (slider2Dec);
  slider3.SetHandleIncrease (slider3Inc);
  slider3.SetHandleDecrease (slider3Dec);
  slider4.SetHandleIncrease (slider4Inc);
  slider4.SetHandleDecrease (slider4Dec);
  slider1.SetHandleSame (slider1SAME);
  slider2.SetHandleSame (slider2SAME);
  slider3.SetHandleSame (slider3SAME);
  slider4.SetHandleSame (slider4SAME);
  ms.get_root_menu().add_menu(&mu1);
  ms.get_root_menu().add_menu(&mu2);
  ms.get_root_menu().add_item(&mm_mi0);
  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);
  mu1.add_item(&mu1_mi1);
  mu1.add_item(&mu1_mi2);
  mu1.add_item(&mu1_mi3);
  mu1.add_item(&mu1_mi6);
  mu1.add_item(&mu1_mi7);
  mu2.add_menu(&mu5);
  mu2.add_menu(&mu6);
  mu2.add_menu(&mu7);
  mu5.add_item(&mu2_mi1);
  mu5.add_item(&mu2_mi2);
  mu5.add_item(&mu2_mi3);
  mu5.add_item(&mu2_mi4);
  mu6.add_item(&mu3_mi1);
  mu6.add_item(&mu3_mi2);
  mu6.add_item(&mu3_mi3);
  mu6.add_item(&mu3_mi4);
  mu7.add_item(&mu4_mi1);
  mu7.add_item(&mu4_mi2);
  display.begin (SSD1306_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, mybitmap, 128, 64, 1);
  display.display();
  delay (2500);
  display.clearDisplay();
  delay (500);
  display.setCursor(23, 22);
  display.setFont();
  display.setTextSize(2);
  display.println("effects");
  display.display();
  delay (500);
  display.clearDisplay();
  delay (100);
  display.setCursor(24, 22);
  display.println("control");
  display.display();
  delay (500);
  display.clearDisplay();
  delay (100);
  display.setCursor(29, 22);
  display.println("system");
  display.display();
  delay (1000);
  display.clearDisplay();
  delay (500);
  currentDataPointer->presetSelect();
  delay (2000);
  display.clearDisplay();
  delay (500);
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 4);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("SELECT NEXT PRESET: ");
  currentDataPointer->bignumber();
  if (currentDataPointer->identifier == 25) {
  display.setCursor(0, 50);
      display.setFont (&FreeMono24pt7b);
      display.printf ("%03d%s%d", storedSettings.program + 1, ":", scene.Update);
  } else { display.setCursor(22, 50);
  currentDataPointer->bignumber();}
  display.display();
}

void loop() {
  ble.update(500); // interval for each scanning ~ 500ms (non blocking)
  encoder1.ReadWrite();
  Buttons.ReadWrite();
  Fader::ReadWrite();
}

void buttpressDisplayUpdate (void) {
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 3);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  for (int i = 0; i < 4; i++) {
    display.setFont ();
    display.printf("%s%d%s", "STOMP ", i + 1, " : ");
    display.setFont (&FreeMono9pt7b);
    if (buttOnOff[i] == buttOff) {
      display.printf("%s%s\n", "", buttOnOff[i]);
    }
    else {
      display.printf("%s%s\n", "    ", buttOnOff[i]);
    }
  }
  display.display();
}

void fademoveDisplayUpdate (void) {
  display.clearDisplay();
  display.setFont ();
  display.setCursor(34, 3);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.printf("%s\n\n\n", " - FADERS - ");
  display.setFont ();
  display.setCursor(0, 27);
  display.printf("%s", "1 - ");
  display.setFont (&FreeMono9pt7b);
  display.setCursor(21, 35);
  display.printf("%03d", faderValue[0]);
  display.setCursor(72, 35);
  display.printf("%03d\n", faderValue[2]);
  display.setFont ();
  display.setCursor(100, 27);
  display.printf("%s", " - 3");
  display.setFont ();
  display.setCursor(0, 55);
  display.printf("%s", "2 - ");
  display.setFont (&FreeMono9pt7b);
  display.setCursor(21, 63);
  display.printf("%03d", faderValue[1]);
  display.setCursor(72, 63);
  display.printf("%03d", faderValue[3]);
  display.setFont ();
  display.setCursor(100, 55);
  display.printf("%s", " - 4");
  display.display();
}

void editMenuDisplayUpdate (void) {
  display.clearDisplay();
  display.setFont ();
  display.setTextSize(1);
  display.setCursor(16, 0);
  if (ms._p_curr_menu == ms._p_root_menu) {
    display.printf ("%s", " - SETUP MENU - ");
  }
  ms.display();
  display.display();
}

/* BLE callbacks*/
void connected(void) {
  isConnected = true;
}

void disconnected(void) {
  isConnected = false;
}

/*Button Callbacks*/

void SelectPress (void) {
  currentState->execute3();
}

void SelectRelease (void) {
  currentState->execute6();
}


void EditPress (void) {
  currentState->execute4();
}

void EditRelease (void) {
  currentState->execute5();
}

void Stomp1ON(void) {
  if (buttOnOff[0] == buttOff) {
    CCbleTXmidi(0, 0x7F);
    buttOnOff[0] = buttOn;
  } else {
    CCbleTXmidi(0, 0x00);
    buttOnOff[0] = buttOff;
  }
  buttpressDisplayUpdate();
}

void Stomp2ON(void) {
  if (buttOnOff[1] == buttOff) {
    CCbleTXmidi(1, 0x7F);
    buttOnOff[1] = buttOn;
  } else {
    CCbleTXmidi(1, 0x00);
    buttOnOff[1] = buttOff;
  }
  buttpressDisplayUpdate();
}

void Stomp3ON(void) {
  if (buttOnOff[2] == buttOff) {
    CCbleTXmidi(2, 0x7F);
    buttOnOff[2] = buttOn;
  } else {
    CCbleTXmidi(2, 0x00);
    buttOnOff[2] = buttOff;
  }
  buttpressDisplayUpdate();
}

void Stomp4ON(void) {
  if (buttOnOff[3] == buttOff) {
    CCbleTXmidi(3, 0x7F);
    buttOnOff[3] = buttOn;
  } else {
    CCbleTXmidi(3, 0x00);
    buttOnOff[3] = buttOff;
  }
  buttpressDisplayUpdate();
}

/*Rotary Callbacks*/
void Right (void) {
  rcount++;
  if (rcount > 1) {
    lcount = 0;
    rcount = 0;
    currentState->execute2();
  }
}

void Left (void) {
  lcount++;
  if (lcount > 1) {
    rcount = 0;
    lcount = 0;
    currentState->execute1();
  }
}

/*Toggle callback*/
void  Tog1ON (void) {
    currentState->execute2();
  }

void  Tog2ON (void) {
    currentState->execute1();
  }

void CCbleTXmidi (int CC, int Value) {
  if (isConnected) {
    midi.send(0xB0 + (storedSettings.channel - 1), storedSettings.CCnumber[CC], Value);
  }
}

/*Fader Callbacks*/
void faderCallback (int index) {
  if (INIT2[index]) {
    INIT2[index] = false;
    return;
  }
  CCbleTXmidi(index + 4, faderValue[index]);
  fademoveDisplayUpdate();
}

void slider1Inc (int currentValue) {
  faderValue [0] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (0);
}

void slider1Dec (int currentValue) {
  faderValue [0] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (0);
}

void slider2Inc (int currentValue) {
  faderValue [1] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (1);
}
void slider2Dec (int currentValue) {
  faderValue [1] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (1);
}

void slider3Inc (int currentValue) {
  faderValue [2] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (2);
}

void slider3Dec (int currentValue) {
  faderValue [2] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (2);
}

void slider4Inc (int currentValue) {
  faderValue [3] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (3);
}

void slider4Dec (int currentValue) {
  faderValue [3] = map (currentValue, 0, 1023, 0, 127);
  faderCallback (3);
}

void slider1SAME (int currentValue) {
  faderValue [0] = map (currentValue, 0, 1023, 0, 127);
}

void slider2SAME (int currentValue) {
  faderValue [1] = map (currentValue, 0, 1023, 0, 127);
}

void slider3SAME (int currentValue) {
  faderValue [2] = map (currentValue, 0, 1023, 0, 127);
}

void slider4SAME (int currentValue) {
  faderValue [3] = map (currentValue, 0, 1023, 0, 127);
}

/*Menu Callbacks*/
void on_itemLED_selected(MenuItem * p_menu_item) {
  currentDataPointer = &led_brightness;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_itemGLOBAL_selected(MenuItem * p_menu_item) {
  currentDataPointer = &update_delay;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item0_selected(MenuItem * p_menu_item) {
  currentDataPointer = &midi_channel;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item1_selected(MenuItem * p_menu_item) {
  storedSettings.preset = 21;
  currentDataPointer = cPParray[0];
  my_flash_store.write(storedSettings);
  currentState = &stateFour;
  preset1.presetSelect();
  delay (2000);
  currentState = &stateFour;
  editMenuDisplayUpdate ();
}

void on_item2_selected(MenuItem * p_menu_item) {
  storedSettings.preset = 22;
  currentDataPointer = cPParray[1];
  currentState = &stateFour;
  my_flash_store.write(storedSettings);
  preset2.presetSelect();
  delay (2000);
  editMenuDisplayUpdate ();
}

void on_item3_selected(MenuItem * p_menu_item) {
  storedSettings.preset = 23;
  if (updateprogram >= 32) {
    updateprogram = 31;
    storedSettings.program = updateprogram;
  }
  currentDataPointer = cPParray[2];
  my_flash_store.write(storedSettings);
  currentState = &stateFour;
  preset3.presetSelect();
  delay (2000);
  currentState = &stateFour;
  editMenuDisplayUpdate ();
}

void on_itemLINE6_selected(MenuItem * p_menu_item) {
  storedSettings.preset = 24;
  currentDataPointer = cPParray[3];
  my_flash_store.write(storedSettings);
  currentState = &stateFour;
  preset4.presetSelect();
  delay (2000);
  editMenuDisplayUpdate ();
}

void on_itemAXE_selected(MenuItem * p_menu_item) {
  storedSettings.preset = 25;
  currentState = &stateFour;
  currentDataPointer = cPParray[4];
  my_flash_store.write(storedSettings);
  scene.presetSelect();
  delay (2000);
  editMenuDisplayUpdate ();
}

void on_item6_selected(MenuItem * p_menu_item) {
  currentDataPointer = &stomp1;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item7_selected(MenuItem * p_menu_item) {
  currentDataPointer = &stomp2;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item8_selected(MenuItem * p_menu_item) {
  currentDataPointer = &stomp3;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item9_selected(MenuItem * p_menu_item) {
  currentDataPointer = &stomp4;
  currentState = &stateSix;
  currentDataPointer->select();
}
void on_item10_selected(MenuItem * p_menu_item) {
  currentDataPointer = &fader1;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item11_selected(MenuItem * p_menu_item) {
  currentDataPointer = &fader2;
  currentState = &stateSix;
  currentDataPointer->select();
}
void on_item12_selected(MenuItem * p_menu_item) {
  currentDataPointer = &fader3;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item13_selected(MenuItem * p_menu_item) {
  currentDataPointer = &fader4;
  currentState = &stateSix;
  currentDataPointer->select();
}

void on_item14_selected(MenuItem * p_menu_item) {
  currentState = &stateFour;
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.printf("%s", "PRESET");
  display.setCursor(0, 12);
  display.printf("%s", "SCENE CC SELECTED:");
  display.setCursor(0, 34);
  display.setFont (&FreeMono12pt7b);
  display.println("34 AXE FX");
  display.display();
  storedSettings.sceneCC = 34;
  my_flash_store.write(storedSettings);
  delay (2000);
  editMenuDisplayUpdate ();
}

void on_item15_selected(MenuItem * p_menu_item) {
  currentState = &stateFour;
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.printf("%s", "PRESET");
  display.setCursor(0, 12);
  display.printf("%s", "SCENE CC SELECTED:");
  display.setCursor(0, 34);
  display.setFont (&FreeMono12pt7b);
  display.println("69 LINE 6");
  display.display();
  storedSettings.sceneCC = 69;
  my_flash_store.write(storedSettings);
  delay (2000);
  editMenuDisplayUpdate ();
}

