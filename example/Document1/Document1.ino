
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <myController.h>
//#define SDA_PIN 4
//#define SCL_PIN 5

/*******ATTEMPT TO USE i2c ADA326***************/
/*TO USE i2C you have to jumper the back of the display,
  as per the adafruit instructions.
  Pins are 18, 19, 4, as below
*/

#define OLED_DATA   18
#define OLED_CLK    19
#define OLED_RESET  4

//Adafruit_SSD1306 display(OLED_RESET);
enum Preset : uint8_t  {ZERO, ONE, BIASFX, AMPLITUDE};
Preset PRESET = ZERO; //initialise
Rotary encoder1 (2, 14); // 2 and 6 are Teensy pin numbers
int program = 0; // initial program
uint8_t lcount = 0;
uint8_t rcount = 0;

/*Pointer Assignments*/
const char Zero [] = "ZERO";
const char One [] = "ONE";
const char Biasfx [] = "BIASFX";
const char Amplitude [] = "AMPLITUDE";
const char * presetArrayDisplayUpdate [4] {
  Zero, One, Biasfx, Amplitude
};

void setup() {
  encoder1.SetHandleLeft (Left);
  encoder1.SetHandleRight (Right);
  //display.begin (SSD1306_SWITCHCAPVCC, 0x3D);
 // display.clearDisplay();
  //display.setTextSize(2);
  //display.setTextColor(WHITE);
  presetDisplayUpdate(); //initial display of "ZERO" PRESET
}

void loop () {
  Rotary::ReadWrite();
  presetDisplayUpdate();
}

/*Display functions*/
void presetDisplayUpdate (void) {
  //display.clearDisplay();
  //display.setCursor(0, 0);
  //Serial.println (presetArrayDisplayUpdate [PRESET] );
  //display.println(presetArrayDisplayUpdate [PRESET]);
  //display.println (program);
  Serial.println (program);
  //display.display();
}

/*Rotary Callbacks*/
void Left (void) {
  lcount++
  if (lcount > 3){
lcount = 0;
  program--;
  if (program <= -1) {
    program = 127;
  }
  presetDisplayUpdate ();
  }
}
void Right (void) {
  rcount++
  if (lcount > 3){
    rcount=0;
  program++;
  if (program >= 128) {
    program = 0;
  }
  presetDisplayUpdate ();
  }
}
