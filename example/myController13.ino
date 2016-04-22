
#include <myController13.h>

//The digital input pins used are, in order, 2, 14, 7, 8, 15, 22, 23, 18, 11, 5, 4, 3, 6.
//So, if you want 4 digital pins, use 2, 14, 7, and 8 (the first 4), and change the .h file. 
//Default is the full '13'.
//There is a picture in the github repository showing how its all hooked up, 
//And there is a .rtf file of wiring and what gets written out in the 3 different modes. 

//The rotary encoder is on digital 16 and 17. I use a pushbutton encoder, so it also has one of the pins listed above.

//For the fader, PWM speed pin is 20, and the direction pin is 21. I use an ardumoto controller from sparkfun.  
//PWM speed has to be adjusted in the .cpp file ... . 

//The fader wiper is pin A14, and the capacative touch line is pin 19.
//Sparkfun has a really good capacative touch enabled motorized fader

#define TOUCHTHRESH 2000 // Threshold for fader capacitive touch. Change it down to make it more sensitive, up for less sensitive.

#define HYST 5 // Amount of jitter suppression for the fader. A capacitor across the wiper and gnd might be a better idea.

#define HBRIDGE // comment this out if using the ardumoto controller from sparkfun.
                // leave it in if you are using an HBRIDGE

Controller myController13; //a new instance.

void setup()
{
myController13.begin (3, 6, 4, HYST, TOUCHTHRESH);
// This is called with the three pins that will be used to select a mode.
// After selection, the pins have their 'normal' functions (see the repo .txt file)
// Also the reading of the inputs starts (after selection)
}
void loop()
{
myController13.switchesWrite (); // writes out switches data in accordance with the mode selected
myController13.faderWrite(); // writes out fader data in accordance with the mode selected
myController13.rotaryWrite(); //writes out rotary data in accordance with the mode selected 

//comment out lines you don't want ... so, no rotary? ... comment out that line
}
