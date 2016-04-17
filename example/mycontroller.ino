#include <myController13.h>
//#include <MIDI.h>




//MIDI_CREATE_INSTANCE(HardwareSerial, Serial2,     midiB);
//MIDI_CREATE_INSTANCE(HardwareSerial, Serial1,     midiA);


#define NUM_INPUT_SWITCHES 13
//The digital pins used are, in order, 2, 14, 7, 8, 15, 22, 23, 18, 3, 4, 5, 11, 6. 
//So, if you want 4 digital pins, use 2, 14, 7, and 8 (the first 4). 

//The rotary encoder is on digital 16 and 17.

//PWM speed pin is 20, and the direction pin is 21.
//The fader wiper in is pin A14, and the capacative touch line is pin 19.

#define TOUCHTHRESH 2000     
// Threshold for fader capacitive touch. 

#define HYST 5             
// Amount of hysteresis jitter suppresion for the fader.  A capacitor across the wiper and gnd might be a better idea.
IntervalTimer RotaryTimer;
IntervalTimer SwitchesTimer;
Controller myController13; 
 
volatile uint8_t rotaryArray [] = {0x01F, 0x01F};
volatile uint16_t Rotary::readout = 0x000;
volatile bool Rotary::rotaryState = false;

volatile uint8_t switchArray [] = {0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007};
volatile uint16_t Switches::pressed = 0;
volatile uint16_t Switches::released = 0;

void RotaryRead (void)
{ Rotary::rotaryState = true;
  Rotary::readout <<= 2;          //remember previous state
  volatile uint16_t newRead = GPIOB_PDIR;
  for (int i = 0; i < 2; i++)
  {
    rotaryArray[i] <<= 1;
    rotaryArray[i] |= (newRead >> i) & 0x001;
    if ( (rotaryArray[i] & 0x01F ) == 0x01F) {
      Rotary::readout |= (1 << i);
    }
    else if ( (rotaryArray[i] & 0x1F) == 0x000) {
      Rotary::readout &= ~(1 << i);
    }
  }
}

//IntervalTimer SwitchesTimer;

void SwitchesRead (void)
{
  Switches::pressed = 0;
  Switches::released = 0;
  volatile uint16_t newRead = (digitalRead (6) << 12) | (digitalRead (3) << 11) | (digitalRead (4) << 10) | (digitalRead (5) << 9) | (digitalRead (11) << 8) | digitalRead(18) << 7 | ((GPIOC_PDIR & 7) << 4) | (GPIOD_PDIR & 0x00F);
  for (int i = 0; i < NUM_INPUT_SWITCHES; i++)
  {
    switchArray[i] <<= 1;
    switchArray[i] |= (newRead >> i) & 0x001;
    if ((switchArray[i] & 0x007) == 0x007) {
      Switches::released |= (1 << i);
    }
    else if ((switchArray[i] & 0x007) == 0x000) {
      Switches::pressed |= (1 << i);
    }
  }
}

void setup()
{
 
  SwitchesTimer.begin (SwitchesRead, 312);
  RotaryTimer.begin (RotaryRead, 567);
  myController13.begin (3, 6, 4, HYST, TOUCHTHRESH, NUM_INPUT_SWITCHES);  
// called with the three pins used to select mode (before reverting to their normal mode functions

}

void loop()
{
 myController13.Loop();
}
