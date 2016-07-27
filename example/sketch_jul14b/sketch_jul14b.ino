#include <myController.h> // that the name of my library

Rotary encoder1 (14, 16); //a new encoder with left and right pins

int rotary1mod = 0;
int pwm = 5; //pwm pin

void setup() {
  Serial.begin(34800);
  pinMode (pwm, OUTPUT);
  Serial.printf ("%s","HELP");
  encoder1.SetHandleLeft(left1);
  encoder1.SetHandleRight(right1);
}

void loop() {
  Rotary::ReadWrite(); // Sets the polling of the encoder in motion.  
  Serial.printf ("%s","HELP");
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

