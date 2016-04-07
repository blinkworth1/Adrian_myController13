#include "myController13.h"

#define NUM_INPUT_SWITCHES 13
//The digital pins used are, in order, 2, 14, 7, 8, 15, 22, 23, 18, 3, 4, 5, 11, 6. 

//So, if you want 4 digital pins, use 2, 14, 7, and 8 (the first 4). 

//The rotary encoder is on digital 16 and 17.

#define MOTOR
//Comment out '#define MOTOR' if you DO NOT want to use the fader MOTOR.

//If using the MOTOR, the pwm / speed pin is 20, and the direction pin is 21.

//The fader wiper in is pin A14, and the capacative touch line is pin 19.

#define TOUCHTHRESH 2000     
// Threshold for fader capacitive touch. The fader is assumed to have capacative touch even if the MOTOR is not used. 

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
