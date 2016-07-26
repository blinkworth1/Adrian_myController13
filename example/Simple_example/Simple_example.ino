#include <myController.h> // that the name of my library

Rotary encoder1 (4, 6); //a new encoder with left and right pins
Rotary encoder2 (5, 7);
Fader slider1 (A5, 4); // a new slider with an analog read pin, and "4" as the jitter-suppression setting
Switches Buttons (8,9,10,11,14, 22); // pins for buttons ... note 8 is button 1, 9 is button 2, etc.
int encoder1mod = 0;
int encoder2mod = 0;
int pitch = 0;

void setup() {
  encoder1.SetHandleLeft(left1); //set callback function, to execute when encoder is turned left, to user function: void left1()
  encoder1.SetHandleRight(right1);

  encoder2.SetHandleLeft(left2);
  encoder2.SetHandleRight(right2);

slider1.SetHandleIncrease(up); //set callback function, to execute when slider is moved up, to user function: void up(int)
slider1.SetHandleDecrease(down);

  Buttons.SetHandleB1ON (but1ON); //set callback function, to execute when button 1 is pressed, to user function: void but1ON()
  Buttons.SetHandleB1OFF (but1OFF); //set callback function, to execute when button 1 is released, to user function: void but1OFF()
  Buttons.SetHandleB2ON (but1ON);
  Buttons.SetHandleB2OFF (but2OFF);
  Buttons.SetHandleB3ON (but3ON);
  Buttons.SetHandleB3OFF (but3OFF);
  Buttons.SetHandleB4ON (but4ON);
  Buttons.SetHandleB4OFF (but4OFF);
  Buttons.SetHandleB5ON (but5ON);
  Buttons.SetHandleB5OFF (but5OFF);
  Buttons.SetHandleB6ON (but6ON);
  Buttons.SetHandleB6OFF (but6OFF);
}

void loop() {
  Rotary::ReadWrite(); // Sets the polling of both the encoders in motion.  No need for delays.
  Fader::ReadWrite(); // Sets the polling of the faderboth in motion.
  Switches::ReadWrite();
  }

void left1 (void) {
  encoder1mod ++;
  if (encoder1mod >= 128) {
    encoder1mod = 127;
  }
  usbMIDI.sendControlChange (1, encoder1mod, 1);
}

void left2 (void) {
  encoder2mod ++;
  if (encoder2mod >= 128) {
    encoder2mod = 127;
  }
  usbMIDI.sendControlChange (1, encoder2mod, 1);
}

void right1 (void) {
  encoder1mod --;
  if (encoder1mod <= -1) {
    encoder1mod = 0;
  }
  usbMIDI.sendControlChange (1, encoder1mod, 1);
}

void right2 (void) {
  encoder2mod --;
  if (encoder2mod <= -1) {
    encoder2mod = 0;
  }
  usbMIDI.sendControlChange (1, encoder2mod, 1);
}

void up (int increase) {
  pitch = map (increase, 0, 1023, -8191, 8192);
  usbMIDI.sendPitchBend (pitch, 1); 
  //"increase" is the value the slider has been moved up to, between 0 -1023.
}

void down (int decrease) {
  pitch = map (decrease, 0, 1023, -8191, 8192);
  usbMIDI.sendPitchBend (pitch, 1);
  //"decrease" is the value the slider has been moved down to, between 0 -1023.
}

void but1ON(void) {
  usbMIDI.sendNoteOn (64, 127, 1);
}
void but2ON(void) {
  usbMIDI.sendNoteOn (65, 127, 1);
}
void but3ON(void) {
  usbMIDI.sendNoteOn (66, 127, 1);
}
void but4ON(void) {
  usbMIDI.sendNoteOn (67, 127, 1);
}
void but5ON(void) {
  usbMIDI.sendNoteOn (68, 127, 1);
}
void but6ON(void) {
  usbMIDI.sendNoteOn (69, 127, 1);
}
void but1OFF(void) {
  usbMIDI.sendNoteOff (64, 127, 1);
}
void but2OFF(void) {
  usbMIDI.sendNoteOff (65, 127, 1);
}
void but3OFF(void) {
  usbMIDI.sendNoteOff (66, 127, 1);
}
void but4OFF(void) {
  usbMIDI.sendNoteOff (67, 127, 1);
}
void but5OFF(void) {
  usbMIDI.sendNoteOff (68, 127, 1);
}
void but6OFF(void) {
  usbMIDI.sendNoteOff (69, 127, 1);
}



