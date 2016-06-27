
#include "MyRenderer.h"
#include <MIDI.h> // MIDI 4.2 library
#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
//#include <Adafruit_ssd1306syp.h>
//#include <MenuSystem.h>
#include <myController.h>

//#define SDA_PIN 4
//#define SCL_PIN 5

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
//Adafruit_ssd1306syp display(SDA_PIN, SCL_PIN);
MIDI_CREATE_INSTANCE (HardwareSerial, Serial1, midiA);
enum Preset : uint8_t  {TONESTACK_onSTAGE, TONESTACK_PRESET_MGR, BIASFX, AMPLITUBE, NI_GUITAR_RIG};
enum RotaryMode : uint8_t {PROG, EDITMENU, CC};
enum peripheral : uint8_t {Button1, Button2, Button3, Button4, Slider1, Slider2, Slider3, Slider4};
elapsedMillis switchesPressTimer;
bool editMenuPressed = false;
bool stomp1 = false;
bool stomp2 = false;
bool stomp3 = false;
bool stomp4 = false;
int program = 0;
uint8_t CCnumber = 0;
uint8_t lcount = 0;
uint8_t rcount = 0;
uint8_t storedCCnumber [] {0, 1, 2, 3, 4, 5, 6, 7};
RotaryMode ENCMODE = PROG;
Preset PRESET = TONESTACK_onSTAGE;
peripheral PERIPHERAL;
Fader slider1 (A1, 3); //Teensy pin and jitter suppression amount
Fader slider2 (A2, 3);
Fader slider3 (A3, 3);
Fader slider4 (A6, 3);
Rotary encoder1 (2, 6); // 2 and 6 are Teensy pin numbers, left and right
Switches Buttons (6);// 6 is the number of switches ... in the following order ...

/************************
  ____________PIN_______
  Button1     23  select
  Button2     22  edit
  Button3     9   stomp1
  Button4     10  stomp2
  Button5     7   stomp3
  Button6     11  stomp4
   etc.      etc.
 ************************/

/*Forward Declarations*/
void on_item0_selected(MenuItem* p_menu_item);
void on_item1_selected(MenuItem* p_menu_item);
void on_item2_selected(MenuItem* p_menu_item);
void on_item3_selected(MenuItem* p_menu_item);
void on_item4_selected(MenuItem* p_menu_item);
void on_back1_item_selected (MenuItem* p_menu_item);
void on_item5_selected(MenuItem* p_menu_item);
void on_item6_selected(MenuItem* p_menu_item);
void on_item7_selected(MenuItem* p_menu_item);
void on_item8_selected(MenuItem* p_menu_item);
void on_item9_selected(MenuItem* p_menu_item);
void on_back2_item_selected (MenuItem* p_menu_item);
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

/*Pointer Assignments*/
const char * ZERODisplayUpdate = "TONESTACK_onSTAGE";
const char * ONEDisplayUpdate = "TONESTACK_PRESET_MGR";
const char * BIASFXDisplayUpdate = "BIASFX";
const char * AMPLITUDEDisplayUpdate = "AMPLITUBE";
const char * NIDisplayUpdate = "NI_GUITAR_RIG";
const char * presetArrayDisplayUpdate [5] {
  ZERODisplayUpdate, ONEDisplayUpdate, BIASFXDisplayUpdate, AMPLITUDEDisplayUpdate, NIDisplayUpdate
};
const char *B1DisplayUpdate = "B1";
const char *B2DisplayUpdate = "B2";
const char *B3DisplayUpdate = "B3";
const char *B4DisplayUpdate = "B4";
const char *S1DisplayUpdate = "S1";
const char *S2DisplayUpdate = "S2";
const char *S3DisplayUpdate = "S3";
const char *S4DisplayUpdate = "S4";
const char *peripheralArrayDisplayUpdate [8] {
  B1DisplayUpdate, B2DisplayUpdate, B3DisplayUpdate, B4DisplayUpdate,
  S1DisplayUpdate, S2DisplayUpdate, S3DisplayUpdate, S4DisplayUpdate
};
const char alpha [] {65, 66, 67, 68};

/*Menu structure*/
Menu mu1("PRESET");
Menu mu2("STOMP");
MenuItem mm_mi1 ("CHANNEL", &on_item0_selected);
MenuItem mu1_mi1("TONESTACK_onSTAGE", &on_item1_selected);
MenuItem mu1_mi2("TONESTACK_PRESET_MGR", &on_item2_selected);
MenuItem mu1_mi3("BIASFX", &on_item3_selected);
MenuItem mu1_mi4("AMPLITUBE", &on_item4_selected);
MenuItem mu1_mi5("NI_GUITAR_RIG", &on_item5_selected);
BackMenuItem mu1_mi0("back", &on_back1_item_selected, &ms);
MenuItem mu2_mi1("STOMP1", &on_item6_selected);
MenuItem mu2_mi2("STOMP2", &on_item7_selected);
MenuItem mu2_mi3("STOMP3", &on_item8_selected);
MenuItem mu2_mi4("STOMP4", &on_item9_selected);
BackMenuItem mu2_mi0("back", &on_back2_item_selected, &ms);
//TODO ... SLIDERMENU and SLIDERMENU callbacks

void setup() {
  midiA.begin();
  encoder1.SetHandleLeft (Left);
  encoder1.SetHandleRight (Right);
  Buttons.SetHandleB1ON (SelectPress);
  Buttons.SetHandleB1OFF (SelectRelease);
  Buttons.SetHandleB2ON (EditPress);
  Buttons.SetHandleB2OFF (EditRelease);
  Buttons.SetHandleB3ON (Stomp1ON);
  //Buttons.SetHandleB3OFF (Stomp1OFF);
  Buttons.SetHandleB4ON (Stomp2ON);
  //Buttons.SetHandleB4OFF (Stomp2OFF);
  Buttons.SetHandleB5ON (Stomp3ON);
  //Buttons.SetHandleB5OFF (Stomp3OFF);
  Buttons.SetHandleB6ON (Stomp4ON);
  //Buttons.SetHandleB6OFF (Stomp4OFF);
  slider1.SetHandleIncrease (slider1Inc);
  slider1.SetHandleDecrease (slider1Dec);
  slider2.SetHandleIncrease (slider2Inc);
  slider2.SetHandleDecrease (slider2Dec);
  slider3.SetHandleIncrease (slider3Inc);
  slider3.SetHandleDecrease (slider3Dec);
  slider4.SetHandleIncrease (slider4Inc);
  slider4.SetHandleDecrease (slider4Dec);
  //display.initialize();
  display.begin (SSD1306_SWITCHCAPVCC, 0x3D);
  //display.clear();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  presetDisplayUpdate (); //initial display of PRESET, hopefully!
  ms.get_root_menu().add_menu(&mu1);
  ms.get_root_menu().add_menu(&mu2);
  ms.get_root_menu().add_item(&mm_mi1);
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
  display.println(*(presetArrayDisplayUpdate [PRESET]) );
  switch (PRESET) {
    case TONESTACK_onSTAGE:
      display.println (program);
      break;
    case TONESTACK_PRESET_MGR:
    case AMPLITUBE:
    case NI_GUITAR_RIG:
      display.println (program + 1);
      break;
    case BIASFX:
      int letter = ((program + 4) % 4);
      int number = (program + 4) / 4;
  
      display.print(number); display.println (letter);
      break;
  }
  display.display();
}

void peripheralDisplayUpdate (void) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println(*(peripheralArrayDisplayUpdate [PERIPHERAL]) );
  display.println (CCnumber);
  display.display();
}

/*Button Callbacks*/
void SelectPress (void) {
  switch (ENCMODE) {
    case PROG:
      switchesPressTimer = 0;
      break;
    case EDITMENU:
      ms.select();
      display.clearDisplay();
      ms.display();
      display.display();
      break;
    case CC:
      storedCCnumber [PERIPHERAL] = CCnumber;
      ENCMODE = EDITMENU;
      display.clearDisplay();
      display.setCursor(0, 0);
      display.println("STORED");
      display.display();
      delay (200);
      display.clearDisplay();
      ms.display();
      display.display();
      break;
  }
}
void SelectRelease (void) {
  switch (ENCMODE) {
    case PROG:
      if ((switchesPressTimer - 1200) > 0) {
        midiA.sendProgramChange (program, 1);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.println("SENT");
        display.display();
        delay (200);
        presetDisplayUpdate ();
      }
      break;
    case EDITMENU:
    case CC:
      break;
  }
}
void EditPress (void) {
  switchesPressTimer = 0;
}
void EditRelease (void) {
  if ((switchesPressTimer - 3500) > 0) {
    ENCMODE = EDITMENU;
    display.clearDisplay();
    ms.display();
    display.display();
  }
  else {
    ENCMODE = PROG;
    presetDisplayUpdate();
  }
}
void Stomp1ON(void) {
  if (stomp1 == false) {
    midiA.sendControlChange (storedCCnumber[0], 0, 1);
    stomp1 = true;
  } else {
    midiA.sendControlChange (storedCCnumber[0], 127, 1);
    stomp1 = false;
  }
}
//void Stomp1OFF(void) {}
void Stomp2ON(void) {
  if (stomp2 == false) {
    midiA.sendControlChange (storedCCnumber[1], 0, 1);
    stomp2 = true;
  } else {
    midiA.sendControlChange (storedCCnumber[1], 127, 1);
    stomp2 = false;
  }
}
//void Stomp2OFF(void) {}
void Stomp3ON(void) {
  if (stomp3 == false) {
    midiA.sendControlChange (storedCCnumber[2], 0, 1);
    stomp3 = true;
  } else {
    midiA.sendControlChange (storedCCnumber[2], 127, 1);
    stomp3 = false;
  }
}
//void Stomp3OFF(void) {}
void Stomp4ON(void) {
  if (stomp4 == false) {
    midiA.sendControlChange (storedCCnumber[3], 0, 1);
    stomp4 = true;
  } else {
    midiA.sendControlChange (storedCCnumber[3], 127, 1);
    stomp4 = false;
  }
}
//void Stomp4OFF(void) {}

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
        presetDisplayUpdate ();
        break;
      case EDITMENU:
        ms.prev ();
        display.clearDisplay();
        ms.display ();
        display.display();
        break;
      case CC:
        CCnumber--;
        if (CCnumber <= -1) {
          CCnumber = 127;
        }
        peripheralDisplayUpdate();
        break;
    }
  }
}
void Right (void) {
  lcount++;
  if (rcount > 5) {
    rcount = 0;
    switch (ENCMODE) {
      case PROG:
        program++;
        if (program >= 128) {
          program = 0;
        }
        presetDisplayUpdate ();
        break;
      case EDITMENU:
        ms.next ();
        display.clearDisplay();
        ms.display ();
        display.display();
        break;
      case CC:
        CCnumber++;
        if (CCnumber >= 128) {
          CCnumber = 0;
        }
        peripheralDisplayUpdate();
        break;
    }
  }
}

/*Fader Callbacks*/

void slider1Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[4], Value, 1);
}
void slider1Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[4], Value, 1);
}
void slider2Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[5], Value, 1);
}
void slider2Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[5], Value, 1);
}
void slider3Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[6], Value, 1);
}
void slider3Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[6], Value, 1);
}
void slider4Inc (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[7], Value, 1);
}
void slider4Dec (int currentValue) {
  int Value = map (currentValue, 0, 1023, 0, 127);
  midiA.sendControlChange (storedCCnumber[7], Value, 1);
}
/*Menu Callbacks*/
void on_item0_selected(MenuItem* p_menu_item)
{
}

void on_item1_selected(MenuItem* p_menu_item)
{
  PRESET = TONESTACK_onSTAGE;
  presetDisplayUpdate ();
}
void on_item2_selected(MenuItem* p_menu_item)
{
  PRESET = TONESTACK_PRESET_MGR;
  presetDisplayUpdate ();
}
void on_item3_selected(MenuItem* p_menu_item)
{
  PRESET = BIASFX;
  presetDisplayUpdate ();
}
void on_item4_selected(MenuItem* p_menu_item)
{
  PRESET = AMPLITUBE;
  presetDisplayUpdate ();
}
void on_item5_selected(MenuItem* p_menu_item)
{
  PRESET = NI_GUITAR_RIG;
  presetDisplayUpdate ();
}
void on_back1_item_selected(MenuItem* p_menu_item)
{
}
void on_item6_selected(MenuItem* p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button1;
  peripheralDisplayUpdate () ;
}
void on_item7_selected(MenuItem* p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button2;
  peripheralDisplayUpdate () ;
}
void on_item8_selected(MenuItem* p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button3;
  peripheralDisplayUpdate () ;
}
void on_item9_selected(MenuItem* p_menu_item)
{
  ENCMODE = CC;
  PERIPHERAL = Button4;
  peripheralDisplayUpdate () ;
}
void on_back2_item_selected(MenuItem* p_menu_item)
{
}

//TODO SLIDER CALLBACKS
