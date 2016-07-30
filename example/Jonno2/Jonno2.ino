#include <Arduino.h>
#include <SPI.h>
//#include <MIDI.h> // MIDI 4.2 library
#include <Wire.h>
#include "MyRenderer.h"
#include <myController.h>
#include "FlashStorage.h"
#include "elapsedMillis.h"
#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BLEMIDI.h"

const unsigned char mybitmap [] PROGMEM = {
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xc0, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0xe0, 0x0f, 0xf0, 0x07, 0xff, 0xff, 0xfe, 0x03, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xd8, 0x07, 0xff, 0x80, 0x07, 0xe0, 0x01, 0xff, 0xff, 0xf0, 0x00, 0x7f, 0xff,
  0xff, 0xff, 0x00, 0x18, 0x07, 0xfe, 0x00, 0x07, 0xe0, 0x00, 0x7f, 0xff, 0xc0, 0x00, 0x1f, 0xff,
  0xff, 0xfc, 0x00, 0x18, 0x07, 0xfc, 0x00, 0x07, 0xe0, 0x00, 0x3f, 0xff, 0x80, 0x00, 0x0f, 0xff,
  0xff, 0xf8, 0x00, 0x18, 0x07, 0xf8, 0x00, 0x07, 0xe0, 0x00, 0x1f, 0xfe, 0x00, 0x00, 0x07, 0xff,
  0xff, 0xf0, 0x00, 0x18, 0x07, 0xf0, 0x00, 0x07, 0xe0, 0x00, 0x0f, 0xfe, 0x00, 0x00, 0x03, 0xff,
  0xff, 0xc0, 0x00, 0x18, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xfc, 0x00, 0x00, 0x01, 0xff,
  0xff, 0xc0, 0x00, 0x18, 0x07, 0xc0, 0x00, 0x07, 0xe0, 0x00, 0x03, 0xf8, 0x00, 0x00, 0x01, 0xff,
  0xff, 0x80, 0x00, 0x18, 0x07, 0xc0, 0x07, 0xff, 0xff, 0xe0, 0x03, 0xf8, 0x00, 0x00, 0x00, 0xff,
  0xff, 0x00, 0x00, 0x18, 0x07, 0x80, 0x1f, 0xff, 0xff, 0xf8, 0x01, 0xf0, 0x01, 0xfc, 0x00, 0xff,
  0xff, 0x00, 0x1f, 0x98, 0x07, 0x80, 0x3f, 0xff, 0xff, 0xfc, 0x01, 0xf0, 0x03, 0xfe, 0x00, 0x7f,
  0xfe, 0x00, 0x7f, 0xf8, 0x07, 0x80, 0x7f, 0xff, 0xff, 0xfe, 0x01, 0xf0, 0x07, 0xff, 0x00, 0x7f,
  0xfe, 0x00, 0xff, 0xf8, 0x07, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0xe0, 0x0f, 0xff, 0x80, 0x7f,
  0xfe, 0x00, 0xff, 0xf8, 0x07, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xe0, 0x0f, 0xff, 0x80, 0x7f,
  0xfe, 0x01, 0xff, 0xf8, 0x07, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0x80, 0x3f,
  0xfc, 0x01, 0xff, 0xf8, 0x07, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0xc0, 0x3f,
  0xfc, 0x03, 0xff, 0xf8, 0x07, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0xc0, 0x3f,
  0xfc, 0x03, 0xff, 0xf8, 0x07, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0xc0, 0x3f,
  0xfc, 0x03, 0xff, 0xf8, 0x07, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0x80, 0x3f,
  0xfc, 0x01, 0xff, 0xf8, 0x07, 0x00, 0x00, 0x07, 0xe0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0x80, 0x7f,
  0xfc, 0x01, 0xff, 0xf0, 0x07, 0x00, 0x00, 0x0f, 0xf0, 0x00, 0x00, 0xe0, 0x1f, 0xff, 0x80, 0x7f,
  0xfe, 0x01, 0xff, 0xf0, 0x07, 0x00, 0xff, 0xff, 0xff, 0xff, 0x00, 0xe0, 0x1f, 0xff, 0x00, 0x7f,
  0xfe, 0x00, 0xff, 0xf0, 0x07, 0x00, 0x7f, 0xff, 0xff, 0xfe, 0x00, 0xe0, 0x1f, 0xfe, 0x00, 0x7f,
  0xfe, 0x00, 0xff, 0xe0, 0x0f, 0x80, 0x3f, 0xff, 0xff, 0xfc, 0x01, 0xe0, 0x19, 0xfc, 0x00, 0xff,
  0xfe, 0x00, 0x3f, 0xc0, 0x0f, 0x80, 0x1f, 0xff, 0xff, 0xf8, 0x01, 0xe0, 0x18, 0x00, 0x00, 0xff,
  0xff, 0x00, 0x0f, 0x00, 0x0f, 0x80, 0x0f, 0xff, 0xff, 0xf0, 0x03, 0xe0, 0x18, 0x00, 0x01, 0xff,
  0xff, 0x00, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x0f, 0xf0, 0x00, 0x03, 0xe0, 0x18, 0x00, 0x01, 0xff,
  0xff, 0x80, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x07, 0xe0, 0x18, 0x00, 0x03, 0xff,
  0xff, 0xc0, 0x00, 0x00, 0x3f, 0xe0, 0x00, 0x07, 0xe0, 0x00, 0x0f, 0xe0, 0x18, 0x00, 0x07, 0xff,
  0xff, 0xc0, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x07, 0xe0, 0x00, 0x0f, 0xe0, 0x18, 0x00, 0x0f, 0xff,
  0xff, 0xe0, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x07, 0xe0, 0x00, 0x1f, 0xe0, 0x18, 0x00, 0x1f, 0xff,
  0xff, 0xf8, 0x00, 0x01, 0xff, 0xfe, 0x00, 0x07, 0xe0, 0x00, 0x7f, 0xe0, 0x18, 0x00, 0x7f, 0xff,
  0xff, 0xfc, 0x00, 0x07, 0xff, 0xff, 0x00, 0x07, 0xe0, 0x00, 0xff, 0xe0, 0x18, 0x03, 0xff, 0xff,
  0xff, 0xff, 0x00, 0x1f, 0xff, 0xff, 0xe0, 0x07, 0xe0, 0x07, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x1b, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe0, 0x03, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
  0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

#define pwm 9 // pwm pin for leds, for Feather
#define OLED_DATA   20 //i2c pins for Feather, for display
#define OLED_CLK    21
#define OLED_RESET  5
Fader slider1 (A0, 3); //Feather pins and jitter suppression amount
Fader slider2 (A1, 3);
Fader slider3 (A4, 3);
Fader slider4 (A5, 3);
Rotary encoder1 (0, 1); // 0 and 1 are Feather pin numbers, left and right, for the rotary encoder
Switches Buttons (16, 17, 10, 11, 12, 13); //16 and 17 are select and edit, respectively, and 10 thru 13 stomp pins, for Feather

Adafruit_BluefruitLE_SPI ble(8, 7, 6); //these are internal connections, don't worry about them.
Adafruit_BLEMIDI midi(ble);
Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 * dptr = &display;
MyRenderer my_renderer (dptr);
MenuSystem ms(my_renderer);
elapsedMillis switchesPressTimer;
//MIDI_CREATE_INSTANCE (HardwareSerial, Serial1, midiA);

enum Preset : uint8_t  {TONESTACK_onSTAGE, TONESTACK_PRESET_MGR, BIASFX, AMPLITUBE, GUITAR_RIG};
enum RotaryMode : uint8_t {PROG, EDITMENU, CC, CHANNEL, BUTTPRESS, GLOBAL, LED};
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
Settings storedSettings = {true, 50, 1, 9, {0, 1, 2, 3, 4, 5, 6, 7}, 600, TONESTACK_onSTAGE};
Settings displayUpdate;
FlashStorage(my_flash_store, Settings);

uint8_t lcount = 0;
uint8_t rcount = 0;
bool GLOBALRESET [4] = {true, true, true, true};
bool INIT = true;
bool isConnected = false;
bool EXIT = false;

/*Forward Declarations*/
void connected(void);
void disconnected(void);
void on_itemGLOBAL_selected(MenuItem* p_menu_item);
void on_itemLED_selected(MenuItem* p_menu_item);
void on_item0_selected(MenuItem* p_menu_item);
void on_item1_selected(MenuItem* p_menu_item);
void on_item2_selected(MenuItem* p_menu_item);
void on_item3_selected(MenuItem* p_menu_item);
void on_item4_selected(MenuItem* p_menu_item);
void on_item5_selected(MenuItem* p_menu_item);
void on_back1_item_selected (MenuItem* p_menu_item);
void on_item6_selected(MenuItem* p_menu_item);
void on_item7_selected(MenuItem* p_menu_item);
void on_item8_selected(MenuItem* p_menu_item);
void on_item9_selected(MenuItem* p_menu_item);
void on_back2_item_selected (MenuItem* p_menu_item);
void on_item10_selected(MenuItem* p_menu_item);
void on_item11_selected(MenuItem* p_menu_item);
void on_item12_selected(MenuItem* p_menu_item);
void on_item13_selected(MenuItem* p_menu_item);
void on_back3_item_selected (MenuItem* p_menu_item);
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
const char ZERODisplayUpdate [] = "TONESTACK onSTAGE";
const char ONEDisplayUpdate [] = "TONESTACK MANAGER";
const char BIASFXDisplayUpdate [] = "BIAS FX";
const char AMPLITUDEDisplayUpdate [] = "AMPLITUBE";
const char NIDisplayUpdate [] = "GUITAR RIG";
const char * presetArrayDisplayUpdate [5] {
  ZERODisplayUpdate, ONEDisplayUpdate, BIASFXDisplayUpdate,
  AMPLITUDEDisplayUpdate, NIDisplayUpdate
};
const char B1DisplayUpdate [] = "STOMP 1";
const char B2DisplayUpdate [] = "STOMP 2";
const char B3DisplayUpdate [] = "STOMP 3";
const char B4DisplayUpdate [] = "STOMP 4";
const char S1DisplayUpdate [] = "FADER 1";
const char S2DisplayUpdate [] = "FADER 2";
const char S3DisplayUpdate [] = "FADER 3";
const char S4DisplayUpdate [] = "FADER 4";
const char *peripheralArrayDisplayUpdate [8] {
  B1DisplayUpdate, B2DisplayUpdate, B3DisplayUpdate, B4DisplayUpdate,
  S1DisplayUpdate, S2DisplayUpdate, S3DisplayUpdate, S4DisplayUpdate
};
const char buttOn [] = "ON";
const char buttOff [] = "OFF";
const char *buttOnOff [4] {buttOff, buttOff, buttOff, buttOff};
const int alpha [] {65, 66, 67, 68};

/*Menu structure*/
Menu mu1("* EXTERNAL FX HOST");
Menu mu2("* STOMP CC ASSIGNMENT");
Menu mu3("* FADER CC ASSIGNMENT");
MenuItem mm_mi0 ("* SNAPSHOT DELAY", &on_itemGLOBAL_selected);
MenuItem mm_mi1 ("* GLOBAL MIDI CHANNEL", &on_item0_selected);
MenuItem mm_mi2 ("* LED BRIGHTNESS", &on_itemLED_selected);
MenuItem mu1_mi1("TONESTACK onSTAGE", &on_item1_selected);
MenuItem mu1_mi2("TONESTACK MANAGER", &on_item2_selected);
MenuItem mu1_mi3("BIAS FX          ", &on_item3_selected);
MenuItem mu1_mi4("AMPLITUBE        ", &on_item4_selected);
MenuItem mu1_mi5("GUITAR RIG    ", &on_item5_selected);
//BackMenuItem mu1_mi0("... back to menu ", &on_back1_item_selected, &ms);
MenuItem mu2_mi1("STOMP 1   ", &on_item6_selected);
MenuItem mu2_mi2("STOMP 2   ", &on_item7_selected);
MenuItem mu2_mi3("STOMP 3   ", &on_item8_selected);
MenuItem mu2_mi4("STOMP 4   ", &on_item9_selected);
//BackMenuItem mu2_mi0("... back to menu ", &on_back2_item_selected, &ms);
MenuItem mu3_mi1("FADER 1   ", &on_item10_selected);
MenuItem mu3_mi2("FADER 2   ", &on_item11_selected);
MenuItem mu3_mi3("FADER 3   ", &on_item12_selected);
MenuItem mu3_mi4("FADER 4   ", &on_item13_selected);
//BackMenuItem mu3_mi0("... back to menu ", &on_back3_item_selected, &ms);

void setup() {
  /*BLE setup and debug*/
  Serial.begin(38400);
  delay(1000);
  if ( !ble.begin(true) ) // If set to 'true' enables debug output
  {
    Serial.println("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?");
  }
  ble.echo(false);
  if ( ! midi.begin(true) ) {
    Serial.println("Could not enable MIDI");
  }
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
  slider4.SetHandleSame (slider3SAME);
  ms.get_root_menu().add_menu(&mu1);
  ms.get_root_menu().add_menu(&mu2);
  ms.get_root_menu().add_menu(&mu3);
  ms.get_root_menu().add_item(&mm_mi0);
  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);
  mu1.add_item(&mu1_mi1);
  mu1.add_item(&mu1_mi2);
  mu1.add_item(&mu1_mi3);
  mu1.add_item(&mu1_mi4);
  mu1.add_item(&mu1_mi5);
  //mu1.add_item(&mu1_mi0);
  mu2.add_item(&mu2_mi1);
  mu2.add_item(&mu2_mi2);
  mu2.add_item(&mu2_mi3);
  mu2.add_item(&mu2_mi4);
  //mu2.add_item(&mu2_mi0);
  mu3.add_item(&mu3_mi1);
  mu3.add_item(&mu3_mi2);
  mu3.add_item(&mu3_mi3);
  mu3.add_item(&mu3_mi4);
  //mu3.add_item(&mu3_mi0);
  display.begin (SSD1306_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, mybitmap, 128, 64, 1);
  display.display();
  delay (1500);
  display.clearDisplay();
  delay (500);
  display.setCursor(1, 23);
  display.setFont();
  display.setTextSize(1);
  display.println("- fx control system -");
  display.display();
  delay (1000);
  display.clearDisplay();
  delay (500);
  display.setCursor(0, 4);
  display.println("current host:");
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
  display.setCursor(4, 4);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.println("-SELECT NEXT PRESET-");
  display.setCursor(0, 47);
  presetNumberDisplayUpdate(displayUpdate.program, 4);
  if (INIT == false) {
    display.setCursor(81, 23);
    display.setTextSize(1);
    display.print ("current");
    display.setCursor(81, 47);
    presetNumberDisplayUpdate(storedSettings.program, 2);
  }
  display.display();
}

void presetNumberDisplayUpdate (int prog, int txtsize) {
  if (txtsize == 2) {
    display.setFont (&FreeMono12pt7b);
  }
  if (txtsize == 4) {
    display.setFont (&FreeMono24pt7b);
  }
  switch (storedSettings.PRESET) {
    case TONESTACK_PRESET_MGR:
      display.printf ("%03d", prog);
      break;
    case TONESTACK_onSTAGE:
    case AMPLITUBE:
    case GUITAR_RIG:
      display.printf ("%03d", (prog + 1));
      break;
    case BIASFX:
      if (prog >= 32) {
        prog = 31;
      }
      int number = ((prog + 4) / 4);
      int letter = ((prog + 4) % 4);
      display.printf ("%d%c", number, alpha [letter]);
      break;
  }
  display.setFont ();
}

void buttpressDisplayUpdate (void) {
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextColor(WHITE);
  display.setTextSize(1);
  for (int i = 0; i < 4; i++) {
    display.printf("%s", peripheralArrayDisplayUpdate [i]);
    display.setFont (&FreeMono9pt7b);
    display.printf("%s\n", buttOnOff[i]);
    display.setFont ();
  }
  if (INIT == false) {
    display.setCursor(81, 23);
    display.print ("current");
    display.setCursor(81, 47);
    presetNumberDisplayUpdate(storedSettings.program, 2);
  }
  display.display();
}

void peripheralDisplayUpdate (void) {
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1);
  display.printf("%s%s\n", "* ", (peripheralArrayDisplayUpdate [PERIPHERAL]) );
  display.printf ("%s%03d", "current: ", storedSettings.CCnumber [PERIPHERAL] );
  display.setCursor(0, 43);
  display.print ("new:     ");
  display.setCursor(55, 43);
  display.setFont (&FreeMono12pt7b);
  display.printf ("%03d", displayUpdate.CCnumber [PERIPHERAL]);
  display.display();
  display.setFont ();
}

void editMenuDisplayUpdate (void) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  if (ms._p_curr_menu == ms._p_root_menu) {
    display.printf ("%s\n", "   --SETUP MENU--");
  }
  ms.display();
  display.display();
  display.setFont ();
}

void channelDisplayUpdate(void) {
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1);
  display.println("* GLOBAL MIDI CHANNEL");
  display.printf ("%s%02d", "current: ", storedSettings.channel);
  display.setCursor(0, 43);
  display.print ("new:     ");
  display.setCursor(55, 43);
  display.setFont (&FreeMono12pt7b);
  display.printf ("%02d", displayUpdate.channel);
  display.display();
  display.setFont ();
}

void globalDisplayUpdate(void) {
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1);
  display.println("* SNAPSHOT DELAY");
  display.printf ("%s%d", "current: ", storedSettings.msdelay);
  display.setCursor(0, 43);
  display.print ("new:     ");
  display.setCursor(55, 43);
  display.setFont (&FreeMono12pt7b);
  display.printf ("%02d", displayUpdate.msdelay);
  display.display();
  display.setFont ();
}

void ledDisplayUpdate(void) {
  display.clearDisplay();
  display.setCursor(0, 4);
  display.setTextSize(1);
  display.println("* LED BRIGHTNESS");
  display.display();
  display.setFont ();
}

/* BLE callbacks*/
void connected(void)
{
  isConnected = true;
  Serial.println(" CONNECTED!");
}
void disconnected(void)
{
  Serial.println("disconnected");
  isConnected = false;
}

/*Button Callbacks*/
void SelectPress (void) {
  switch (ENCMODE) {
    case PROG:
      //switchesPressTimer = 0;
      if (INIT == true) INIT = false;
      break;
    case EDITMENU:
    case CC:
    case CHANNEL:
    case BUTTPRESS:
    case GLOBAL:
    case LED:
      break;
  }
}
void SelectRelease (void) {
  switch (ENCMODE) {
    case PROG:
      storedSettings.program = displayUpdate.program;
      //  midiA.sendProgramChange (storedSettings.program, storedSettings.channel);
      if (isConnected) {
        midi.send(0xC0 + (storedSettings.channel - 1), storedSettings.program, storedSettings.program);
      }
      my_flash_store.write(storedSettings);
      display.clearDisplay();
      display.setCursor(10, 4);
      display.setTextSize(1);
      display.println ("- CURRENT PRESET -");
      display.setCursor(0, 47);
      presetNumberDisplayUpdate (storedSettings.program, 4);
      display.display();
      delay (storedSettings.msdelay * 10);
      GLOBALRESET [0] = true;
      GLOBALRESET [1] = true;
      GLOBALRESET [2] = true;
      GLOBALRESET [3] = true;
      break;
    case EDITMENU:
      ms.select();
      if (ENCMODE == PROG) {
        display.clearDisplay();
        display.setCursor(0, 4);
        display.setTextSize(1);
        display.println("HOST selected:");
        display.printf("%s\n", presetArrayDisplayUpdate [storedSettings.PRESET]);
        display.setCursor(0, 43);
        display.setFont (&FreeMono9pt7b);
        display.println("- STORED -");
        display.display();
        display.setFont();
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
      display.clearDisplay();
      display.setCursor(0, 4);
      display.setTextSize(1);
      display.printf("%s%s\n", "* ", (peripheralArrayDisplayUpdate [PERIPHERAL]) );
      display.printf ("%s%03d", "current: ", storedSettings.CCnumber [PERIPHERAL]);
      display.setCursor(0, 43);
      display.setFont (&FreeMono9pt7b);
      //display.setTextSize(2);
      display.println("- STORED -");
      display.display();
      display.setFont();
      delay (2500);
      editMenuDisplayUpdate ();
      break;
    case CHANNEL:
      storedSettings.channel = displayUpdate.channel;
      my_flash_store.write(storedSettings);
      ENCMODE = EDITMENU;
      display.clearDisplay();
      display.setCursor(0, 4);
      display.setTextSize(1);
      display.println("* GLOBAL MIDI CHANNEL");
      display.printf ("%s%02d", "current: ", storedSettings.channel);
      display.setCursor(0, 43);
      display.setFont (&FreeMono9pt7b);
      display.println("- STORED -");
      display.display();
      display.setFont();
      display.display();
      delay (2500);
      editMenuDisplayUpdate ();
      break;
    case GLOBAL:
      storedSettings.msdelay = displayUpdate.msdelay;
      my_flash_store.write(storedSettings);
      ENCMODE = EDITMENU;
      display.clearDisplay();
      display.setCursor(0, 4);
      display.setTextSize(1);
      display.println("* SNAPSHOT DELAY");
      display.printf ("%s%d", "current: ", storedSettings.msdelay);
      display.setCursor(0, 43);
      display.setFont (&FreeMono9pt7b);
      display.println("- STORED -");
      display.display();
      display.setFont();
      delay (2500);
      editMenuDisplayUpdate ();
      break;
    case BUTTPRESS:
      ENCMODE = PROG;
      presetDisplayUpdate ();
      break;
    case LED:
      storedSettings.rotary1mod = displayUpdate.rotary1mod;
      my_flash_store.write(storedSettings);
      ENCMODE = EDITMENU;
      display.clearDisplay();
      display.setCursor(0, 4);
      display.setTextSize(1);
      display.println("* LED BRIGHTNESS");
      display.setCursor(0, 43);
      display.setFont (&FreeMono9pt7b);
      display.println("- STORED -");
      display.display();
      display.setFont();
      delay (2500);
      editMenuDisplayUpdate ();
  }
}
void EditPress (void) {
  switch (ENCMODE) {
    case PROG:
      switchesPressTimer = 0;
      display.clearDisplay();
      display.display();
      display.setFont();
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
        int time = switchesPressTimer - 4000;
        if (EXIT == true) {
          EXIT = false;
        }
        else if (time < 0) {
          display.clearDisplay();
          display.setCursor(1, 26);
          display.setFont();
          display.setTextSize(1);
          display.println("- faders updated -");
          display.display();
          delay(500);
          presetDisplayUpdate();
          GLOBALRESET [0] = true;
          GLOBALRESET [1] = true;
          GLOBALRESET [2] = true;
          GLOBALRESET [3] = true;
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
      break;
  }
}
void Stomp1ON(void) {
  if (buttOnOff[0] == buttOff) {
    // midiA.sendControlChange (storedSettings.CCnumber[0], 0, storedSettings.channel);
    buttOnOff[0] = buttOn;
  } else {
    // midiA.sendControlChange (storedSettings.CCnumber[0], 127, storedSettings.channel);
    buttOnOff[0] = buttOff;
  }
  ENCMODE = BUTTPRESS;
  buttpressDisplayUpdate();
}
void Stomp2ON(void) {
  if (buttOnOff[1] == buttOff) {
    // midiA.sendControlChange (storedSettings.CCnumber[1], 0, storedSettings.channel);
    buttOnOff[1] = buttOn;
  } else {
    //  midiA.sendControlChange (storedSettings.CCnumber[1], 127, storedSettings.channel);
    buttOnOff[1] = buttOff;
  }
  ENCMODE = BUTTPRESS;
  buttpressDisplayUpdate();
}
void Stomp3ON(void) {
  if (buttOnOff[2] == buttOff) {
    // midiA.sendControlChange (storedSettings.CCnumber[2], 0, storedSettings.channel);
    buttOnOff[2] = buttOn;
  } else {
    // midiA.sendControlChange (storedSettings.CCnumber[2], 127, storedSettings.channel);
    buttOnOff[2] = buttOff;
  }
  ENCMODE = BUTTPRESS;
  buttpressDisplayUpdate();
}
void Stomp4ON(void) {
  if (buttOnOff[3] == buttOff) {
    // midiA.sendControlChange (storedSettings.CCnumber[3], 0, storedSettings.channel);
    buttOnOff[3] = buttOn;
  } else {
    //midiA.sendControlChange (storedSettings.CCnumber[3], 127, storedSettings.channel);
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
        if (storedSettings.PRESET == BIASFX) {
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
        peripheralDisplayUpdate();
        break;
      case CHANNEL:
        displayUpdate.channel--;
        if (displayUpdate.channel <= 0) {
          displayUpdate.channel = 16;
        }
        channelDisplayUpdate();
        break;
      case GLOBAL:
        displayUpdate.msdelay -=100;
        if (displayUpdate.msdelay <= 0) {
          displayUpdate.msdelay = 0;
        }
        globalDisplayUpdate();
        break;
      case BUTTPRESS:
        ENCMODE = PROG;
        presetDisplayUpdate();
        break;
      case LED:
        displayUpdate.rotary1mod -=7;
        if (displayUpdate.rotary1mod <= 0) {
          displayUpdate.rotary1mod = 0;
        }
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
        if (storedSettings.PRESET == BIASFX) {
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
        peripheralDisplayUpdate();
        break;
      case CHANNEL:
        displayUpdate.channel++;
        if (displayUpdate.channel >= 17) {
          displayUpdate.channel = 1;
        }
        channelDisplayUpdate();
        break;
      case GLOBAL:
        displayUpdate.msdelay += 100;
        if (displayUpdate.msdelay >= 1000) {
          displayUpdate.msdelay = 1000;
        }
        globalDisplayUpdate();
        break;
      case BUTTPRESS:
        ENCMODE = PROG;
        presetDisplayUpdate();
        break;
      case LED:
        displayUpdate.rotary1mod += 7;
        if (displayUpdate.rotary1mod >= 1020) {
          displayUpdate.rotary1mod = 1020;
        }
        analogWrite (pwm, displayUpdate.rotary1mod);
        break;
    }
  }
}

/*Fader Callbacks*/
void slider1Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  // midiA.sendControlChange (storedSettings.CCnumber[4], Value, storedSettings.channel);
}
void slider1Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  // midiA.sendControlChange (storedSettings.CCnumber[4], Value, storedSettings.channel);
}
void slider2Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  // midiA.sendControlChange (storedSettings.CCnumber[5], Value, storedSettings.channel);
}
void slider2Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  //midiA.sendControlChange (storedSettings.CCnumber[5], Value, storedSettings.channel);
}
void slider3Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  //midiA.sendControlChange (storedSettings.CCnumber[6], Value, storedSettings.channel);
}
void slider3Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  //midiA.sendControlChange (storedSettings.CCnumber[6], Value, storedSettings.channel);
}
void slider4Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  //midiA.sendControlChange (storedSettings.CCnumber[7], Value, storedSettings.channel);
}
void slider4Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  //midiA.sendControlChange (storedSettings.CCnumber[7], Value, storedSettings.channel);
}
void slider1SAME (int currentValue) {
  if (GLOBALRESET [0]) {
    GLOBALRESET [0] = false;
    int Value = map (currentValue, 0, 1023, 0, 127);
    // midiA.sendControlChange (storedSettings.CCnumber[4], Value, storedSettings.channel);
  }
} void slider2SAME (int currentValue) {
  if (GLOBALRESET[1]) {
    GLOBALRESET[1] = false;
    int Value = map (currentValue, 0, 1023, 0, 127);
    // midiA.sendControlChange (storedSettings.CCnumber[5], Value, storedSettings.channel);
  }
}
void slider3SAME (int currentValue) {
  if (GLOBALRESET[2]) {
    GLOBALRESET[2] = false;
    int Value = map (currentValue, 0, 1023, 0, 127);
    // midiA.sendControlChange (storedSettings.CCnumber[6], Value, storedSettings.channel);
  }
}
void slider4SAME (int currentValue) {
  if (GLOBALRESET[3]) {
    GLOBALRESET[3] = false;
    int Value = map (currentValue, 0, 1023, 0, 127);
    // midiA.sendControlChange (storedSettings.CCnumber[7], Value, storedSettings.channel);
  }
}

/*Menu Callbacks*/
void on_itemLED_selected(MenuItem * p_menu_item)
{
  ENCMODE = LED;
  ledDisplayUpdate();
}
void on_itemGLOBAL_selected(MenuItem * p_menu_item)
{
  ENCMODE = GLOBAL;
  globalDisplayUpdate();
}
void on_item0_selected(MenuItem * p_menu_item)
{
  ENCMODE = CHANNEL;
  channelDisplayUpdate();
}
void on_item1_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = TONESTACK_onSTAGE;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item2_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = TONESTACK_PRESET_MGR;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item3_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = BIASFX;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item4_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = AMPLITUBE;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item5_selected(MenuItem * p_menu_item)
{
  storedSettings.PRESET = GUITAR_RIG;
  my_flash_store.write(storedSettings);
  ENCMODE = PROG;
}
void on_item6_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button1;
  peripheralDisplayUpdate();
}
void on_item7_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button2;
  peripheralDisplayUpdate();
}
void on_item8_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button3;
  peripheralDisplayUpdate();
}
void on_item9_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button4;
  peripheralDisplayUpdate();
}
void on_item10_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider1;
  peripheralDisplayUpdate();
}
void on_item11_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider2;
  peripheralDisplayUpdate();
}
void on_item12_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider3;
  peripheralDisplayUpdate();
}
void on_item13_selected(MenuItem * p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Slider4;
  peripheralDisplayUpdate();
}


