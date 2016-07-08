
#include "MyRenderer.h"
#include <MIDI.h> // MIDI 4.2 library
#include <Wire.h>
#include <myController.h>
#include <EEPROM.h>
const unsigned char mybitmap [] PROGMEM = {
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x1f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x03, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0x80, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xc0, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xf0, 0x00, 0x00, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x00, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x00, 0x00, 0x3f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x0f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf1, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x7f, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xfc, 0x00, 0x3f, 0xc3, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xe7, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 

};

/*******TO USE i2c ADA326***************/
/*TO USE i2C you have to jumper the back of the display,
  as per the adafruit instructions.
  Pins are 18, 19, 4, as below
*/

#define OLED_DATA   18
#define OLED_CLK    19
#define OLED_RESET  4

Adafruit_SSD1306 display(OLED_RESET);
Adafruit_SSD1306 * dptr = &display;
MyRenderer my_renderer (dptr);
MenuSystem ms(my_renderer);
MIDI_CREATE_INSTANCE (HardwareSerial, Serial1, midiA);
enum Preset : uint8_t  {TONESTACK_onSTAGE, TONESTACK_PRESET_MGR, BIASFX, AMPLITUBE, NI_GUITAR_RIG};
enum RotaryMode : uint8_t {PROG, EDITMENU, CC, CHANNEL};
enum peripheral : uint8_t {Button1, Button2, Button3, Button4, Slider1, Slider2, Slider3, Slider4};
elapsedMillis switchesPressTimer;
bool stomp1 = false;
bool stomp2 = false;
bool stomp3 = false;
bool stomp4 = false;
int channel = 1;
int program = 0;
int CCnumber = 0;
int bfxprogram = 0;
uint8_t lcount = 0;
uint8_t rcount = 0;
uint8_t storedCCnumber [8] {0, 1, 2, 3, 4, 5, 6, 7};
RotaryMode ENCMODE = PROG;
Preset PRESET = TONESTACK_onSTAGE;
peripheral PERIPHERAL;
Fader slider1 (A1, 3); //Teensy pin and jitter suppression amount
Fader slider2 (A2, 3);
Fader slider3 (A3, 3);
Fader slider4 (A6, 3);
Rotary encoder1 (2, 14); // 2 and 6 are Teensy pin numbers, left and right
Switches Buttons (23, 22, 9, 10, 7, 11); //pins

/************************
  ____________PIN_______
  Button1     23  select
  Button2     22  edit
  Button3     9   stomp1
  Button4     10  stomp2
  Button5     7   stomp3
  Button6     11  stomp4
 ************************/
bool GLOBALRESET;
bool INIT;

/*Forward Declarations*/
void on_itemEXIT_selected(MenuItem* p_menu_item);
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
const char ZERODisplayUpdate [] = "TONESTACK_onSTAGE";
const char ONEDisplayUpdate [] = "TONESTACK_MGR";
const char BIASFXDisplayUpdate [] = "BIASFX";
const char AMPLITUDEDisplayUpdate [] = "AMPLITUBE";
const char NIDisplayUpdate [] = "NI_GUITAR_RIG";
const char * presetArrayDisplayUpdate [5] {
  ZERODisplayUpdate, ONEDisplayUpdate, BIASFXDisplayUpdate, AMPLITUDEDisplayUpdate, NIDisplayUpdate
};
const char B1DisplayUpdate [] = "STOMP1";
const char B2DisplayUpdate [] = "STOMP2";
const char B3DisplayUpdate [] = "STOMP3";
const char B4DisplayUpdate [] = "STOMP4";
const char S1DisplayUpdate [] = "FADER1";
const char S2DisplayUpdate [] = "FADER2";
const char S3DisplayUpdate [] = "FADER3";
const char S4DisplayUpdate [] = "FADER4";
const char *peripheralArrayDisplayUpdate [8] {
  B1DisplayUpdate, B2DisplayUpdate, B3DisplayUpdate, B4DisplayUpdate,
  S1DisplayUpdate, S2DisplayUpdate, S3DisplayUpdate, S4DisplayUpdate
};
const int alpha [] {65, 66, 67, 68};

/*Menu structure*/
Menu mu1("* choose EXT HOST");
Menu mu2("* set STOMP CC#'s");
Menu mu3("* set FADER CC#'s");
MenuItem mm_mi1 ("* global MIDI CH#", &on_item0_selected);
MenuItem mm_mi2 ("-------EXIT------", &on_itemEXIT_selected);
MenuItem mu1_mi1("TONESTACK onSTAGE", &on_item1_selected);
MenuItem mu1_mi2("TONESTACK manager", &on_item2_selected);
MenuItem mu1_mi3("BIAS FX          ", &on_item3_selected);
MenuItem mu1_mi4("AMPLITUBE        ", &on_item4_selected);
MenuItem mu1_mi5("NI GUITAR RIG    ", &on_item5_selected);
BackMenuItem mu1_mi0("... back to menu ", &on_back1_item_selected, &ms);
MenuItem mu2_mi1("STOMP - 1   ", &on_item6_selected);
MenuItem mu2_mi2("STOMP - 2   ", &on_item7_selected);
MenuItem mu2_mi3("STOMP - 3   ", &on_item8_selected);
MenuItem mu2_mi4("STOMP - 4   ", &on_item9_selected);
BackMenuItem mu2_mi0("... back to menu ", &on_back2_item_selected, &ms);
MenuItem mu3_mi1("FADER - 1   ", &on_item10_selected);
MenuItem mu3_mi2("FADER - 2   ", &on_item11_selected);
MenuItem mu3_mi3("FADER - 3   ", &on_item12_selected);
MenuItem mu3_mi4("FADER - 4   ", &on_item13_selected);
BackMenuItem mu3_mi0("... back to menu ", &on_back3_item_selected, &ms);

void setup() {
  for (int i = 0; i < 117; i++){
  if (EEPROM.read(i) > 127) {EEPROM.write (i, 1);}
  }
  PRESET = (Preset)EEPROM.read(111);
  bfxprogram = EEPROM.read(113);
  program = EEPROM.read(115);
  init = true;
  for (int i = 0; i < 8; i++) {
    storedCCnumber[i] = EEPROM.read(i);
  }
  midiA.begin();
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
  ms.get_root_menu().add_item(&mm_mi1);
  ms.get_root_menu().add_item(&mm_mi2);
  mu1.add_item(&mu1_mi1);
  mu1.add_item(&mu1_mi2);
  mu1.add_item(&mu1_mi3);
  mu1.add_item(&mu1_mi4);
  mu1.add_item(&mu1_mi5);
  mu1.add_item(&mu1_mi0);
  mu2.add_item(&mu2_mi1);
  mu2.add_item(&mu2_mi2);
  mu2.add_item(&mu2_mi3);
  mu2.add_item(&mu2_mi4);
  mu2.add_item(&mu2_mi0);
  mu3.add_item(&mu3_mi1);
  mu3.add_item(&mu3_mi2);
  mu3.add_item(&mu3_mi3);
  mu3.add_item(&mu3_mi4);
  mu3.add_item(&mu3_mi0);
  display.begin (SSD1306_SWITCHCAPVCC, 0x3D);
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.drawBitmap(0, 0, mybitmap, 128, 64, 1);
  display.display();
  delay (3000);
  display.clearDisplay();
  delay (500);
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.println("effect control system");
  display.display();
  delay (2000);
  display.clearDisplay();
  delay (500);
  presetDisplayUpdate ();
  GLOBALRESET = true;
  INIT = true;
}

void loop() {
  Rotary::ReadWrite();
  Switches::ReadWrite();
  Fader::ReadWrite();
}

  /*Display functions*/
  void presetDisplayUpdate (void) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(1);
    display.println("select a preset..."); //((presetArrayDisplayUpdate [PRESET]) );
    display.setCursor(0, 12);
    presetNumberDisplayUpdate(program, 3);
    if (init = false) {
    display.setCursor(73, 12);
    display.setTextSize(1);
    display.print ("*");
    presetNumberDisplayUpdate(EEPROM.read(115), 1);
    }
    else {INIT = false;}
    display.display();
  }

  void presetNumberDisplayUpdate (int prog, int txtsize) {
    display.setTextSize(txtsize);
    switch (PRESET) {
      case TONESTACK_onSTAGE:
          //display.print (prog);
          display.printf ("%03d", prog);
        break;
      case TONESTACK_PRESET_MGR:
      case AMPLITUBE:
      case NI_GUITAR_RIG:
          //display.print ((prog + 1));
          display.printf ("%03d", (prog + 1));
        break;
      case BIASFX:
        display.setTextSize(txtsize);
        int letter = ((bfxprogram + 4) / 4);
        int number = ((bfxprogram + 4) % 4) + 1;
        display.print (number);
        display.printf ("%c\r", alpha [letter]);
        break;
    }
  }

  void peripheralDisplayUpdate (void) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.print((peripheralArrayDisplayUpdate [PERIPHERAL]) );
    display.setTextSize(1);
    display.printf ("%s %d \n", " CC# ", storedCCnumber [PERIPHERAL] );
    display.setTextSize(3);
    display.println (CCnumber);
    display.display();
  }

  void editMenuDisplayUpdate (void) {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    ms.display();
    display.display();
  }

  void channelDisplayUpdate(void) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextSize(2);
    display.println("CHANNEL");
    display.setTextSize(3);
    display.println (channel);
    display.display();
  }

  /*Button Callbacks*/
  void SelectPress (void) {
    switch (ENCMODE) {
      case PROG:
        switchesPressTimer = 0;
        break;
      case EDITMENU:
      case CC:
      case CHANNEL:
        break;
    }
  }
  void SelectRelease (void) {
    switch (ENCMODE) {
      case PROG:
        if (PRESET == BIASFX) {
          midiA.sendProgramChange  (bfxprogram, channel);
          EEPROM.write (113, bfxprogram);
        }
        else {
          midiA.sendProgramChange (program, channel);
          EEPROM.write (115, program);
        }
        display.clearDisplay();
        display.setCursor(55, 0);
        display.setTextSize(1);
        display.println("preset SENT!");
        display.setCursor(73, 12);
        presetNumberDisplayUpdate (program, 3);
        display.display();
        globalReset();
        break;
      case EDITMENU:
        ms.select();
        if (ENCMODE == PROG) {
          EEPROM.write (111, PRESET);
          display.clearDisplay();
          display.setCursor(0, 0);
          display.setTextSize(2);
          display.printf("%s \n %s \n",  presetArrayDisplayUpdate [PRESET], "SELECTED" );
          display.display();
          delay (2500);
          editMenuDisplayUpdate ();
        }
        else if (ENCMODE == CC) {
          peripheralDisplayUpdate();
        }
        else if (ENCMODE == CHANNEL) {
          channelDisplayUpdate();
        }
        else {
          editMenuDisplayUpdate ();
        }
        break;
      case CC:
        storedCCnumber [PERIPHERAL] = CCnumber;
        EEPROM.write (PERIPHERAL, CCnumber);
        ENCMODE = EDITMENU;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.println("CC STORED");
        display.setTextSize(3);
        display.println(CCnumber);
        display.display();
        delay (2500);
        editMenuDisplayUpdate ();
        break;
      case CHANNEL:
        ENCMODE = EDITMENU;
        display.clearDisplay();
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.println("CHANNEL STORED");
        display.setTextSize(3);
        display.println(channel);
        display.display();
        delay (2500);
        editMenuDisplayUpdate ();
        break;
    }
  }
  void EditPress (void) {
    switch (ENCMODE) {
      case PROG:
        switchesPressTimer = 0;
        break;
      case EDITMENU:
      case CC:
      case CHANNEL:
        break;
    }
  }
  void EditRelease (void) {
    switch (ENCMODE) {
      case PROG: {
          int time = switchesPressTimer - 1000;
          if (time < 0) {
            presetDisplayUpdate();
             globalReset();
          }
          else {
            ENCMODE = EDITMENU;
          }
        }
        break;
      case EDITMENU:
      case CC:
      case CHANNEL:
        ENCMODE = EDITMENU;
        ms.reset();
        editMenuDisplayUpdate();
        break;
    }
  }
  void Stomp1ON(void) {
    if (stomp1 == false) {
      midiA.sendControlChange (storedCCnumber[0], 0, channel);
      stomp1 = true;
    } else {
      midiA.sendControlChange (storedCCnumber[0], 127, channel);
      stomp1 = false;
    }
  }
  void Stomp2ON(void) {
    if (stomp2 == false) {
      midiA.sendControlChange (storedCCnumber[1], 0, channel);
      stomp2 = true;
    } else {
      midiA.sendControlChange (storedCCnumber[1], 127, channel);
      stomp2 = false;
    }
  }
  void Stomp3ON(void) {
    if (stomp3 == false) {
      midiA.sendControlChange (storedCCnumber[2], 0, channel);
      stomp3 = true;
    } else {
      midiA.sendControlChange (storedCCnumber[2], 127, channel);
      stomp3 = false;
    }
  }
  void Stomp4ON(void) {
    if (stomp4 == false) {
      midiA.sendControlChange (storedCCnumber[3], 0, channel);
      stomp4 = true;
    } else {
      midiA.sendControlChange (storedCCnumber[3], 127, channel);
      stomp4 = false;
    }
  }

  /*Rotary Callbacks*/
  void Left (void) {
    lcount++;
    if (lcount > 5) {
      lcount = 0;
      switch (ENCMODE) {
        case PROG:
          program--;
          if (program <= -1) {
            program = 127;
          }
          if (PRESET == BIASFX) {
            bfxprogram = map(program, 0, 127, 0, 31);
          }
          presetDisplayUpdate ();
          break;
        case EDITMENU:
          ms.prev ();
          editMenuDisplayUpdate();
          break;
        case CC:
          CCnumber--;
          if (CCnumber <= -1) {
            CCnumber = 127;
          }
          peripheralDisplayUpdate();
          break;
        case CHANNEL:
          channel--;
          if (channel <= 0) {
            channel = 16;
          }
          break;
      }
    }
  }
  void Right (void) {
    rcount++;
    if (rcount > 5) {
      rcount = 0;
      switch (ENCMODE) {
        case PROG:
          program++;
          if (program >= 128) {
            program = 0;
            if (PRESET == BIASFX) {
              bfxprogram = map(program, 0, 127, 0, 31);

            }
          }
          presetDisplayUpdate ();
          break;
        case EDITMENU:
          ms.next ();
          editMenuDisplayUpdate();
          break;
        case CC:
          CCnumber++;
          if (CCnumber >= 128) {
            CCnumber = 0;
          }
          peripheralDisplayUpdate();
          break;
        case CHANNEL:
          channel++;
          if (channel >= 17) {
            channel = 1;
          }
          break;
      }
    }
  }

  /*Fader Callbacks*/
  void slider1Inc (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[4], Value, channel);
  }
  void slider1Dec (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[4], Value, channel);
  }
  void slider2Inc (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[5], Value, channel);
  }
  void slider2Dec (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[5], Value, channel);
  }
  void slider3Inc (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[6], Value, channel);
  }
  void slider3Dec (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[6], Value, channel);
  }
  void slider4Inc (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[7], Value, channel);
  }
  void slider4Dec (int currentValue) {
    int Value = map (currentValue, 0, 1023, 0, 127);
    midiA.sendControlChange (storedCCnumber[7], Value, channel);
  }
  void slider1SAME (int currentValue) {
    if (GLOBALRESET) {
      GLOBALRESET = false;
      int Value = map (currentValue, 0, 1023, 0, 127);
      midiA.sendControlChange (storedCCnumber[4], Value, channel);
    }
  } void slider2SAME (int currentValue) {
    if (GLOBALRESET) {
      GLOBALRESET = false;
      int Value = map (currentValue, 0, 1023, 0, 127);
      midiA.sendControlChange (storedCCnumber[5], Value, channel);
    }
  }
  void slider3SAME (int currentValue) {
    if (GLOBALRESET) {
      GLOBALRESET = false;
      int Value = map (currentValue, 0, 1023, 0, 127);
      midiA.sendControlChange (storedCCnumber[6], Value, channel);
    }
  }
  void slider4SAME (int currentValue) {
    if (GLOBALRESET) {
      GLOBALRESET = false;
      int Value = map (currentValue, 0, 1023, 0, 127);
      midiA.sendControlChange (storedCCnumber[7], Value, channel);
    }
  }


  /*Menu Callbacks*/
  void on_itemEXIT_selected(MenuItem * p_menu_item)
  {
    ms.reset();
    ENCMODE = PROG;

  }
  void on_item0_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CHANNEL;
  }
  void on_item1_selected(MenuItem * p_menu_item)
  {
    PRESET = TONESTACK_onSTAGE;
    ENCMODE = PROG;
  }
  void on_item2_selected(MenuItem * p_menu_item)
  {
    PRESET = TONESTACK_PRESET_MGR;
    ENCMODE = PROG;
  }
  void on_item3_selected(MenuItem * p_menu_item)
  {
    PRESET = BIASFX;
    ENCMODE = PROG;
  }
  void on_item4_selected(MenuItem * p_menu_item)
  {
    PRESET = AMPLITUBE;
    ENCMODE = PROG;
  }
  void on_item5_selected(MenuItem * p_menu_item)
  {
    PRESET = NI_GUITAR_RIG;
    ENCMODE = PROG;
  }
  void on_back1_item_selected(MenuItem * p_menu_item)
  {
  }
  void on_item6_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Button1;
  }
  void on_item7_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Button2;
  }
  void on_item8_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Button3;
  }
  void on_item9_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Button4;
  }
  void on_back2_item_selected(MenuItem * p_menu_item)
  {
  }
  void on_item10_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Slider1;
  }
  void on_item11_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Slider2;
  }
  void on_item12_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Slider3;
  }
  void on_item13_selected(MenuItem * p_menu_item)
  {
    ENCMODE = CC;
    PERIPHERAL = Slider4;
  }
  void on_back3_item_selected(MenuItem * p_menu_item)
  {
  }


