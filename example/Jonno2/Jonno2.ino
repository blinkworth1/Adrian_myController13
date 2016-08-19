#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include "MyRenderer.h"
#include <myController.h>
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
Fader slider1 (A8, 5); //aref and jitter suppression amount
Fader slider2 (A3, 20);
Fader slider3 (A4, 20);
Fader slider4 (A7, 20);
Rotary encoder1 (15, 16); // left and right
Switches Buttons (6, 19, 11, 12, 13, 14); //6 and 19 are select and edit, respectively, and 11 thru 14 stomp pins, for Feather

Adafruit_BluefruitLE_SPI ble(8, 7, 4); //these are internal connections, don't worry about them.
Adafruit_BLEMIDI midi(ble);
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 * dptr = &display;
MyRenderer my_renderer (dptr);
MenuSystem ms(my_renderer);
elapsedMillis switchesPressTimer;
//MIDI_CREATE_INSTANCE (HardwareSerial, Serial1, midiA);

enum Preset : uint8_t  {ZERO, ONE, BIAS_FX, LINE_6, AXE_FX};
enum RotaryMode : uint8_t {PROG, EDITMENU, CC, CHANNEL, BUTTPRESS, GLOBAL, LED, FADEMOVE};
RotaryMode ENCMODE = PROG;
enum peripheral : uint8_t {Button1, Button2, Button3, Button4, Slider1, Slider2, Slider3, Slider4};
peripheral PERIPHERAL;
typedef struct {
  bool valid;
  int msdelay;
  int8_t channel;
  int16_t program;
  int16_t CCnumber [8];
  int rotary1mod;
  Preset PRESET;
} Settings;
Settings storedSettings = {true, 200, 1, 0, {21, 22, 23, 24, 31, 32, 33, 34}, 200, ONE};
Settings displayUpdate;
FlashStorage(my_flash_store, Settings);

uint8_t lcount = 0;
uint8_t rcount = 0;
bool INIT = true;
bool INIT2 []= {true, true,true, true};
bool isConnected = false;
bool EXIT = false;

/*Forward Declarations*/
void connected(void);
void disconnected(void);
void presetDisplayUpdate (void);
void on_itemGLOBAL_selected(MenuItem* p_menu_item);
void on_itemLED_selected(MenuItem* p_menu_item);
void on_item0_selected(MenuItem* p_menu_item);
void on_item1_selected(MenuItem* p_menu_item);
void on_item2_selected(MenuItem* p_menu_item);
void on_item3_selected(MenuItem* p_menu_item);
//void on_item4_selected(MenuItem* p_menu_item);
//void on_item5_selected(MenuItem* p_menu_item);
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

/*Pointer Assignments*/
const char ZERO$ [] = "000-127";
const char ONE$ [] = "001-128";
const char BIAS_FX$ [] = "1A-8D";
const char LINE_6$ [] = "1A-32D";
const char AXE_FX$ [] = "001-128    + SCENES";
const char * presetArrayDisplayUpdate [5] {
  ZERO$, ONE$, BIAS_FX$, LINE_6$, AXE_FX$
};
char Butt1 [] = "STOMP 1";
char Butt2 [] = "STOMP 2";
char Butt3 [] = "STOMP 3";
char Butt4 [] = "STOMP 4";
char S1 [] = "FADER 1";
char S2 [] = "FADER 2";
char S3 [] = "FADER 3";
char S4 [] = "FADER 4";
char *peripheralArrayDisplayUpdate [8] {
  Butt1, Butt2, Butt3, Butt4,
  S1, S2, S3, S4
};
const char buttOn [] = "ON";
const char buttOff [] = "OFF";
const char *buttOnOff [4] {buttOff, buttOff, buttOff, buttOff};
const int alpha [] {65, 66, 67, 68};
int faderValue [4] = {0,0,0,0};

/*Menu structure*/
Menu mu1("PRESET FORMAT");
Menu mu2("STOMP CC SELECT");
Menu mu3("FADER CC SELECT");
MenuItem mm_mi0 ("AUTO UPDATE DELAY", &on_itemGLOBAL_selected);
MenuItem mm_mi1 ("MIDI CHANNEL", &on_item0_selected);
MenuItem mm_mi2 ("LED BRIGHTNESS", &on_itemLED_selected);
MenuItem mu1_mi1(ZERO$, &on_item1_selected);
MenuItem mu1_mi2(ONE$, &on_item2_selected);
MenuItem mu1_mi3(BIAS_FX$, &on_item3_selected);
MenuItem mu1_mi6(LINE_6$, &on_itemLINE6_selected);
MenuItem mu1_mi7(AXE_FX$, &on_itemAXE_selected);
MenuItem mu2_mi1("STOMP 1", &on_item6_selected);
MenuItem mu2_mi2("STOMP 2", &on_item7_selected);
MenuItem mu2_mi3("STOMP 3", &on_item8_selected);
MenuItem mu2_mi4("STOMP 4", &on_item9_selected);
MenuItem mu3_mi1("FADER 1", &on_item10_selected);
MenuItem mu3_mi2("FADER 2", &on_item11_selected);
MenuItem mu3_mi3("FADER 3", &on_item12_selected);
MenuItem mu3_mi4("FADER 4", &on_item13_selected);

void setup() {
  /*BLE setup and debug*/
  Serial.begin(38400);
  delay(500);
 ble.begin(true); // If set to 'true' enables debug output
 ble.echo(false);
 midi.begin(true);
 ble.verbose(false);

  /*FlashStorage management*/
  displayUpdate = my_flash_store.read();
  if (!(displayUpdate.valid)) {
    displayUpdate = storedSettings;
  }
  else {
    storedSettings = displayUpdate;
  }
  analogWrite (pwm, storedSettings.rotary1mod);

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
  ms.get_root_menu().add_menu(&mu3);
  ms.get_root_menu().add_item(&mm_mi0);
  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);
  mu1.add_item(&mu1_mi1);
  mu1.add_item(&mu1_mi2);
  mu1.add_item(&mu1_mi3);
  //mu1.add_item(&mu1_mi4);
  //mu1.add_item(&mu1_mi5);
  mu1.add_item(&mu1_mi6);
  mu1.add_item(&mu1_mi7);
  mu2.add_item(&mu2_mi1);
  mu2.add_item(&mu2_mi2);
  mu2.add_item(&mu2_mi3);
  mu2.add_item(&mu2_mi4);
  mu3.add_item(&mu3_mi1);
  mu3.add_item(&mu3_mi2);
  mu3.add_item(&mu3_mi3);
  mu3.add_item(&mu3_mi4);
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
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.printf("%s","CURRENT");
  display.setCursor(0, 12);
  display.printf("%s","PRESET FORMAT:");
  display.setCursor(0, 34);
  display.setFont (&FreeMono9pt7b);
  display.println((presetArrayDisplayUpdate [storedSettings.PRESET]) );
  display.display();
  delay (2000);
  display.clearDisplay();
  delay (500);
  presetDisplayUpdate ();
}
        
void loop() {
 ble.update(500); // interval for each scanning ~ 500ms (non blocking)
  Rotary::ReadWrite();
  Switches::ReadWrite();
  Fader::ReadWrite();
}

/*Display functions*/
void presetDisplayUpdate (void) {
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 4);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("SELECT NEXT PRESET:");
  display.setCursor(0, 50);
  presetNumberDisplayUpdate(displayUpdate.program, 4);
  if (INIT == false) {
    display.setFont ();
    display.setCursor(85, 23);
    display.setTextSize(1);
    display.print ("current");
    display.setCursor(91, 33);
    display.setTextSize(1);
    display.print ("preset");
    display.setCursor(84, 55);
    presetNumberDisplayUpdate(storedSettings.program, 2);
  }
  display.display();                                               
}

void presetNumberDisplayUpdate (int prog, int txtsize) {
  if (txtsize == 2) {
    display.setFont (&FreeMono12pt7b);
  }
  if (txtsize == 4) {
    display.setFont (&FreeMonoBold24pt7b);
  }
  switch (storedSettings.PRESET) {
    case ZERO:
      display.printf ("%03d", prog);
      break;
    case ONE:
    case AXE_FX:
      display.printf ("%03d", (prog + 1));
      break;
    case BIAS_FX:
      if (prog >= 32) {
        prog = 31;
      }
      case LINE_6:
      int number = ((prog + 4) / 4);
      int letter = ((prog + 4) % 4);
      display.printf ("%d%c", number, alpha [letter]);
      break;
  }
}

void buttpressDisplayUpdate (void) {
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 3);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  for (int i = 0; i < 4; i++) {
    display.setFont ();
    display.printf("%s%d%s","STOMP ",i+1," : ");
    display.setFont (&FreeMono9pt7b);
    if (buttOnOff[i] == buttOff) {
      display.printf("%s%s\n","",buttOnOff[i]);
    }
    else {
      display.printf("%s%s\n","    ", buttOnOff[i]);
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
  display.printf("%s\n\n\n","- FADERS -");
  
    display.setFont ();
    display.setCursor(0, 27);
    display.printf("%s","1 -");
    display.setFont (&FreeMono9pt7b);
    display.setCursor(21, 35);
    display.printf("%03d",faderValue[0]);
    display.setCursor(72, 35);
    display.printf("%03d\n",faderValue[2]);
    display.setFont ();
    display.setCursor(109, 27);
    display.printf("%s","- 3");
    
    display.setFont ();
    display.setCursor(0, 55);
    display.printf("%s","2 -");
    display.setFont (&FreeMono9pt7b);
    display.setCursor(21, 63);
    display.printf("%03d",faderValue[1]);
    display.setCursor(72, 63);
    display.printf("%03d",faderValue[3]);
    display.setFont ();
    display.setCursor(109, 55);
    display.printf("%s","- 4");
display.display();
}

void peripheralDisplayUpdate (char * heading, char * description, char *format, int Update, int Stored, bool current) {
  display.clearDisplay();
  display.setFont ();
  display.setCursor(0, 4);
  display.setTextSize(1);
  display.printf("%s%s\n",heading, description);
  display.setCursor(0, 47);
  display.setFont (&FreeMonoBold24pt7b);
  display.printf (format, Update);
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

void editMenuDisplayUpdate (void) {
  display.clearDisplay();
  display.setFont ();
  display.setTextSize(1);
  display.setCursor(20, 0);
  if (ms._p_curr_menu == ms._p_root_menu) {
    display.printf ("%s","- SETUP MENU -");
  }
  ms.display();
  display.display();
}

/* BLE callbacks*/
void connected(void)
{
  isConnected = true;
  //Serial.println("CONNECTED!");
}
void disconnected(void)
{
  //Serial.println("disconnected");
  isConnected = false;
}

/*Button Callbacks*/
void SelectPress (void) {
  switch (ENCMODE) {
    case PROG:
      if (INIT == true) INIT = false;
      break;
    case EDITMENU:
    case CC:
    case CHANNEL:
    case BUTTPRESS:
    case FADEMOVE:
    case GLOBAL:
    case LED:
      break;
  }
}
void SelectRelease (void) {
  switch (ENCMODE) {
    case PROG:
      storedSettings.program = displayUpdate.program;
      if (isConnected) {
        midi.send(0xC0 + (storedSettings.channel - 1), storedSettings.program, storedSettings.program);
      }
      my_flash_store.write(storedSettings);
      display.clearDisplay();
      display.setFont ();
      display.setCursor(11, 4);
      display.setTextSize(1);
      display.println ("- CURRENT PRESET -");
      display.setCursor(22, 50);
      presetNumberDisplayUpdate (storedSettings.program, 4);
      display.display();
      delay (storedSettings.msdelay);
      for (int i = 0; i < 4; i++) {
      CCbleTXmidi (storedSettings.CCnumber [i + 4], faderValue[i]);
      }
      //global reset
      break;
    case EDITMENU:
      ms.select();
      if (ENCMODE == PROG) {
        display.clearDisplay();
        display.setFont ();
        display.setCursor(0, 0);
        display.setTextSize(1);
        display.printf("%s","CURRENT");
        display.setCursor(0, 12);
        display.printf("%s","PRESET FORMAT:");
        display.setCursor(0, 34);
        display.setFont (&FreeMono9pt7b);
        //display.setTextSize(2);
        display.println((presetArrayDisplayUpdate [storedSettings.PRESET]) );
        display.display();
        delay (2500);
        ENCMODE = EDITMENU;
      }
      if (ENCMODE == EDITMENU) {
        editMenuDisplayUpdate ();
      }
      break;
    case CC:
      storedSettings.CCnumber [PERIPHERAL] = displayUpdate.CCnumber[PERIPHERAL];
      my_flash_store.write(storedSettings);
      ENCMODE = EDITMENU;
      peripheralDisplayUpdate(
        peripheralArrayDisplayUpdate [PERIPHERAL], " CC STORED",
        "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], false
      );
      delay (2500);
      editMenuDisplayUpdate ();
      break;
    case CHANNEL:
      storedSettings.channel = displayUpdate.channel;
      my_flash_store.write(storedSettings);
      ENCMODE = EDITMENU;
      peripheralDisplayUpdate(
        "MIDI CHANNEL", " STORED",
        "%02d", displayUpdate.channel, storedSettings.channel, false
      );
      delay (2500);
      editMenuDisplayUpdate ();
      break;
    case GLOBAL:
      storedSettings.msdelay = displayUpdate.msdelay;
      my_flash_store.write(storedSettings);
      ENCMODE = EDITMENU;
      peripheralDisplayUpdate(
        "UPDATE DELAY", " STORED",
        "%02d", displayUpdate.msdelay / 10, storedSettings.msdelay / 10, false
      );
      delay (2500);
      editMenuDisplayUpdate ();
      break;
    case LED:
      storedSettings.rotary1mod = displayUpdate.rotary1mod;
      my_flash_store.write(storedSettings);
      ENCMODE = EDITMENU;
      peripheralDisplayUpdate(
        "LED BRIGHTNESS", " STORED",
        "%02d", displayUpdate.rotary1mod / 10, storedSettings.rotary1mod / 10, false
      );
      delay (2500);
      editMenuDisplayUpdate ();
      break;
    case BUTTPRESS:
    case FADEMOVE:
      ENCMODE = PROG;
      presetDisplayUpdate ();
      break;
  }
}
void EditPress (void) {
  switch (ENCMODE) {
    case PROG:
      switchesPressTimer = 0;
      display.clearDisplay();
      display.display();
      break;
    case EDITMENU:
      if ((ms._p_curr_menu == ms._p_root_menu)) {
        ENCMODE = PROG;
        EXIT = true;
        ms.reset();
        presetDisplayUpdate ();
      }
      else {
        ms.back();
        editMenuDisplayUpdate();
      }
      break;
    case CC:
    case CHANNEL:
    case GLOBAL:
    case LED:
      ENCMODE = EDITMENU;
      ms.reset();
      editMenuDisplayUpdate();
      break;
    case BUTTPRESS:
    case FADEMOVE:
      ENCMODE = PROG;
      ms.reset();
      EXIT = true;
      presetDisplayUpdate ();
      break;
  }
}
void EditRelease (void) {
  switch (ENCMODE) {
    case PROG: {
        int time = switchesPressTimer - 2500;
        if (EXIT == true) {
          EXIT = false;
        }
        else if (time < 0) {
          delay (200);
        for (int i = 0; i < 4; i++) {
      CCbleTXmidi (storedSettings.CCnumber [i + 4], faderValue[i]);
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
          presetDisplayUpdate();
        }
        else {
          ENCMODE = EDITMENU;
          ms.reset();
          editMenuDisplayUpdate();
        }
      }
      break;
    case EDITMENU:
    case CC:
    case CHANNEL:
    case GLOBAL:
    case LED:
    case BUTTPRESS:
    case FADEMOVE:
      break;
  }
}
void CCbleTXmidi (int CC, int Value) {
  if (isConnected) {
  midi.send(0xB0 + (storedSettings.channel - 1), storedSettings.CCnumber[CC], Value);
  }
}
void Stomp1ON(void) {
  if (buttOnOff[0] == buttOff) {
      CCbleTXmidi(0, 0x7F);
    buttOnOff[0] = buttOn;
  } else {
      CCbleTXmidi(0, 0x00);
    buttOnOff[0] = buttOff;
  }
  ENCMODE = BUTTPRESS;
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
  ENCMODE = BUTTPRESS;
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
  ENCMODE = BUTTPRESS;
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
  ENCMODE = BUTTPRESS;
  buttpressDisplayUpdate();
}

/*Rotary Callbacks*/
void Left (void) {
  lcount++;
  if (lcount > 1) {
    lcount = 0;
    rcount = 0;
    switch (ENCMODE) {
      case PROG:
        displayUpdate.program--;
        if (storedSettings.PRESET == BIAS_FX) {
          if (displayUpdate.program <= -1) {
            displayUpdate.program = 31;
          }
        }
        else if (displayUpdate.program <= -1) {
          displayUpdate.program = 127;
        }
        presetDisplayUpdate ();
        break;
      case EDITMENU:
        ms.prev ();
        editMenuDisplayUpdate();
        break;
      case CC:
        displayUpdate.CCnumber[PERIPHERAL]--;
        if (displayUpdate.CCnumber [PERIPHERAL] <= -1) {
          displayUpdate.CCnumber[PERIPHERAL] = 127;
        }
        peripheralDisplayUpdate(
          peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
          "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
        );
        break;
      case CHANNEL:
        displayUpdate.channel--;
        if (displayUpdate.channel <= 0) {
          displayUpdate.channel = 16;
        }
        peripheralDisplayUpdate(
          "MIDI CHANNEL", " SELECT",
          "%02d", displayUpdate.channel, storedSettings.channel, true
        );
        break;
      case GLOBAL:
        displayUpdate.msdelay -= 100;
        if (displayUpdate.msdelay <= 0) {
          displayUpdate.msdelay = 0;
        }
        peripheralDisplayUpdate(
          "UPDATE DELAY (ms x10)", "",
          "%02d", displayUpdate.msdelay / 10, storedSettings.msdelay / 10, true
        );
        break;
      case BUTTPRESS:
      case FADEMOVE:
        ENCMODE = PROG;
        presetDisplayUpdate();
        break;
      case LED:
        displayUpdate.rotary1mod -= 10;
        if (displayUpdate.rotary1mod <= 0) {
          displayUpdate.rotary1mod = 0;
        }
        peripheralDisplayUpdate(
          "LED BRIGHTNESS", " SELECT",
          "%02d", displayUpdate.rotary1mod / 10, storedSettings.rotary1mod / 10, true
        );
       analogWrite (pwm, displayUpdate.rotary1mod);
        break;
    }
  }
}
void Right (void) {
  rcount++;
  if (rcount > 1) {
    rcount = 0;
   lcount = 0;
    switch (ENCMODE) {
      case PROG:
        displayUpdate.program++;
        if (storedSettings.PRESET == BIAS_FX) {
          if (displayUpdate.program >= 32) {
            displayUpdate.program = 0;
          }
        }
        else if (displayUpdate.program >= 128) {
          displayUpdate.program = 0;
        }
        presetDisplayUpdate ();
        break;
      case EDITMENU:
        ms.next ();
        editMenuDisplayUpdate();
        break;
      case CC:
        displayUpdate.CCnumber[PERIPHERAL]++;
        if (displayUpdate.CCnumber[PERIPHERAL] >= 128) {
          displayUpdate.CCnumber[PERIPHERAL] = 0;
        }
        peripheralDisplayUpdate(
          peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
          "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
        );
        break;
      case CHANNEL:
        displayUpdate.channel++;
        if (displayUpdate.channel >= 17) {
          displayUpdate.channel = 1;
        }
        peripheralDisplayUpdate(
          "MIDI CHANNEL", " SELECT",
          "%02d", displayUpdate.channel, storedSettings.channel, true
        );
        break;
      case GLOBAL:
        displayUpdate.msdelay += 100;
        if (displayUpdate.msdelay >= 1000) {
          displayUpdate.msdelay = 1000;
        }
        peripheralDisplayUpdate(
          "UPDATE DELAY (ms x10)", "",
          "%02d", displayUpdate.msdelay / 10, storedSettings.msdelay / 10, true
        );
        break;
      case BUTTPRESS:
      case FADEMOVE:
        ENCMODE = PROG;
        presetDisplayUpdate();
        break;
      case LED:
        displayUpdate.rotary1mod += 10;
        if (displayUpdate.rotary1mod >= 200) {
          displayUpdate.rotary1mod = 200;
        }
        analogWrite (pwm, displayUpdate.rotary1mod);
        peripheralDisplayUpdate(
          "LED BRIGHTNESS", " SELECT",
          "%02d", displayUpdate.rotary1mod / 10, storedSettings.rotary1mod / 10, true
        );
        break;
    }
  }
}

/*Fader Callbacks*/
void faderCallback (int index) {
  if (INIT2[index]) {INIT2[index] = false; return;}
     CCbleTXmidi(index + 4, faderValue[index]);
     ENCMODE = FADEMOVE;
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
void on_itemLED_selected(MenuItem * p_menu_item)
{
  ENCMODE = LED;
  peripheralDisplayUpdate(
    "LED BRIGHTNESS", " SELECT",
    "%02d", displayUpdate.rotary1mod / 10, storedSettings.rotary1mod / 10, true
  );
}
void on_itemGLOBAL_selected(MenuItem * p_menu_item)
{
  ENCMODE = GLOBAL;
  peripheralDisplayUpdate(
    "UPDATE DELAY (ms x10)", "",
    "%02d", displayUpdate.msdelay / 10, storedSettings.msdelay / 10, true
  );
}
void on_item0_selected(MenuItem * p_menu_item)
{
  ENCMODE = CHANNEL;
  peripheralDisplayUpdate(
    "MIDI CHANNEL", " SELECT",
    "%02d", displayUpdate.channel, storedSettings.channel, true
  );
}
void on_item1_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = ZERO;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item2_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = ONE;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item3_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = BIAS_FX;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_itemLINE6_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = LINE_6;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_itemAXE_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = AXE_FX;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item6_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button1;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}
void on_item7_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button2;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}
void on_item8_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button3;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}
void on_item9_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button4;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}
void on_item10_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider1;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}
void on_item11_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider2;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}
void on_item12_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider3;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}
void on_item13_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider4;
  peripheralDisplayUpdate(
    peripheralArrayDisplayUpdate [PERIPHERAL], " CC SELECT",
    "%03d", displayUpdate.CCnumber[PERIPHERAL], storedSettings.CCnumber[PERIPHERAL], true
  );
}


