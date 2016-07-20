#include <myController.h> // that the name of my library

Rotary encoder1 (4, 6); //a new encoder with left and right pins

int rotary1mod = 0;
int pwm = 4; //pwm pin

void setup() {
  pinMode (pwm, OUTPUT);
  encoder1.SetHandleLeft(left1);
  encoder1.SetHandleRight(right1);
}

void loop() {
  Rotary::ReadWrite(); // Sets the polling of the encoder in motion.  
  
  }

void left1 (void) {
  rotary1mod ++;
  if (rotary1mod >= 1023) {
    rotary1mod = 1023;
  }
  analogWrite (pwm, rotary1mod); 
}


void right1 (void) {
  rotary1mod --;
  if (rotary1mod <= 0) {
    rotary1mod = 0;
  }
analogWrite (pwm, rotary1mod); 
}

