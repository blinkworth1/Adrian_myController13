#include "myController13.h"

#define NUM_INPUT_SWITCHES 13

#define MOTOR
//Comment out '#define MOTOR' if you DO NOT want to use the fader MOTOR

//If using the MOTOR, the pwm / speed pin is 20, and the direction pin is 21.

//The fader wiper in is pin A14, and the capacative touch line is pin 19.

#define TOUCHTHRESH 2000     
// Threshold for fader capacitive touch ... the fader is assumed to have capacative touch even if the MOTOR is not used. 

#define HYST 2               
// Amount of hysteresis jitter suppresion for the fader. A capacitor across wiper and ground is otherwise needed.

void setup()
{
  myController13.Mode(3, 6, 4);
}

void loop()
{
  myController13.Loop();
}
