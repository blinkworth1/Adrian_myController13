
#include <myController.h>
#include "elapsedMillis.h"

uint8_t Fader::objectIndex = 0;
Switches *Sobj;
Fader * FobjArray[4] {NULL, NULL, NULL, NULL};
const int8_t encState [16] {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
//elapsedMicros RotaryTimer;
elapsedMillis FaderTimer;
//elapsedMicros SwitchesTimer;

Switches::Switches (uint8_t pin1) {
datapin = pin1; 
numberOfSwitches = 8;
}
void Switches::SetHandleB1ON (void (*fptr) (void)) {
  switchesPointersON[0] = fptr;
}
void Switches::SetHandleB1OFF (void (*fptr) (void)) {
  switchesPointersOFF[0] = fptr;
}
void Switches::SetHandleB2ON (void (*fptr) (void)) {
  switchesPointersON[1] = fptr;
}
void Switches::SetHandleB2OFF (void (*fptr) (void)) {
  switchesPointersOFF[1] = fptr;
}
void Switches::SetHandleB3ON (void (*fptr) (void)) {
  switchesPointersON[2] = fptr;
}
void Switches::SetHandleB3OFF (void (*fptr) (void)) {
  switchesPointersOFF[2] = fptr;
}
void Switches::SetHandleB4ON (void (*fptr) (void)) {
  switchesPointersON[3] = fptr;
}
void Switches::SetHandleB4OFF (void (*fptr) (void)) {
  switchesPointersOFF[3] = fptr;
}
void Switches::SetHandleB5ON (void (*fptr) (void)) {
  switchesPointersON[4] = fptr;
}
void Switches::SetHandleB5OFF (void (*fptr) (void)) {
  switchesPointersOFF[4] = fptr;
}
void Switches::SetHandleB6ON (void (*fptr) (void)) {
  switchesPointersON[5] = fptr;
}
void Switches::SetHandleB6OFF (void (*fptr) (void)) {
  switchesPointersOFF[5] = fptr;
}
void Switches::SetHandleB7ON(void(*fptr) (void)) {
	switchesPointersON[6] = fptr;
}
void Switches::SetHandleB7OFF(void(*fptr) (void)) {
	switchesPointersOFF[6] = fptr;
}
void Switches::SetHandleB8ON(void(*fptr) (void)) {
	switchesPointersON[7] = fptr;
}
void Switches::SetHandleB8OFF(void(*fptr) (void)) {
	switchesPointersOFF[7] = fptr;
}
void Switches::SetHandleB9ON(void(*fptr) (void)) {
	switchesPointersON[8] = fptr;
}
void Switches::SetHandleB9OFF(void(*fptr) (void)) {
	switchesPointersOFF[8] = fptr;
}
void Switches::SetHandleB10ON(void(*fptr) (void)) {
	switchesPointersON[9] = fptr;
}
void Switches::SetHandleB10OFF(void(*fptr) (void)) {
	switchesPointersOFF[9] = fptr;
}
void Switches::SetHandleB11ON(void(*fptr) (void)) {
	switchesPointersON[10] = fptr;
}
void Switches::SetHandleB11OFF(void(*fptr) (void)) {
	switchesPointersOFF[10] = fptr;
}
void Switches::SetHandleB12ON(void(*fptr) (void)) {
	switchesPointersON[11] = fptr;
}
void Switches::SetHandleB12OFF(void(*fptr) (void)) {
	switchesPointersOFF[11] = fptr;
}
void Switches::SetHandleB13ON(void(*fptr) (void)) {
	switchesPointersON[12] = fptr;
}
void Switches::SetHandleB13OFF(void(*fptr) (void)) {
	switchesPointersOFF[12] = fptr;
}
void Switches::ReadWrite() {
	//if ((SwitchesTimer - 500) > 0) {
		//SwitchesTimer = 0;
		
switchesRaw = 0;	 

for (int i = 0; i < numberOfSwitches; i++)
		{
Acmd = i & 0x01;
      Bcmd = (i >> 1) & 0x01;
     Ccmd = (i >> 2) & 0x01;
     digitalWrite(15, Acmd);
 digitalWrite(11, Bcmd);
 digitalWrite(12, Ccmd);
switchesRaw |= (digitalRead(datapin)) << i  ;
switchesArray[i] <<= 1;
switchesArray[i] |= ((switchesRaw >> i) & 0x001);
			if ((switchesArray[i] & 0x007) == 0x007) {
				switchesData &= ~(1 << i);
			}
			if ((switchesArray[i] & 0x007) == 0x000) {
				switchesData |= (1 << i);
			}
			if (switchesData & (1 << i)) {
				if (!(pressed & (1 << i))) {
					if (switchesPointersON[i]) {
						switchesPointersON[i]();
					}
					pressed |= (1 << i);
				}
			}
			else {
				if (!(switchesData & (1 << i))){
					if (pressed & (1 << i)) {
						if (switchesPointersOFF[i]) {
						switchesPointersOFF[i]();
						}
						pressed &= ~(1 << i);
					}
				}
			}
		}
	//}
}

Rotary::Rotary (uint8_t left, uint8_t right) {
  //RobjArray[Rotary::objectIndex] = this;
  //Rotary::objectIndex++;
  leftPin = left;
  rightPin = right;
  //begin=true;
}

void Rotary::SetHandleLeft(void (*Left) (void)) {
  pLeft = Left;
}
void Rotary::SetHandleRight (void (*Right) (void)) {
  pRight = Right;
}

void Rotary::ReadWrite() {
		//for (int i = 0; i < Rotary::objectIndex; i++) {
rotaryAraw = digitalRead (leftPin);
      	rotaryBraw = digitalRead (rightPin);
		   
rotaryData <<= 2;

rotaryData |= (rotaryBraw <<1) | rotaryAraw;
				
rotaryState = (encState[(rotaryData & 0x0F)]);
				
if (rotaryState > 0) {
					if (pLeft) {pLeft();}
				}
				if (rotaryState < 0) {
					if (pRight) {pRight();}
				}
			
		//}
}


void Fader::SetHandleIncrease(void(*ptr) (int)) {
	pIncrease = ptr;
}
void Fader::SetHandleDecrease(void(*ptr) (int)) {
	pDecrease = ptr;
}
void Fader::SetHandleSame(void(*ptr) (int)) {
	pSame = ptr;
}
void Fader::SetHandleTouchON(void(*ptr) (int)) {
	pTouchON = ptr;
}
void Fader::SetHandleTouchOFF(void(*ptr) (int)) {
	pTouchOFF = ptr;
}

Fader::Fader(uint8_t wiper, int hysteresis) {
	FobjArray[Fader::objectIndex] = this;
	Fader::objectIndex++;
	wiperPin = wiper;
	hyst = hysteresis;
}

void Fader::begin(uint8_t touch, int touchthresh, uint8_t pwm, uint8_t dirD, uint8_t dirU) {
	pwmPin = pwm;
	pinMode (pwm, OUTPUT);
	pinMode (dirD, OUTPUT);
	pinMode (dirU, OUTPUT);
	dirPin = dirD;
	dirPinU = dirU;
	touchPin = touch;
	touchthreshold = touchthresh;
	MOTOR = true;
}

void Fader::ReadWrite() {
	if ((FaderTimer - 100) >= 0) {
		FaderTimer = 0;
		for (int i = 0; i < 4; i++) {
			FobjArray[i]->FaderRead(); 
				if (FobjArray[i]->currentPinRead > (FobjArray[i]->hystPinRead + FobjArray[i]->hyst))
				{
					FobjArray[i]->currentPinRead = FobjArray[i]->hystPinRead;
					//call decrease
					if (FobjArray[i]->pDecrease) {
						FobjArray[i]->pDecrease(FobjArray[i]->currentPinRead);
					}
				}
					else if (FobjArray[i]->hystPinRead > (FobjArray[i]->currentPinRead + FobjArray[i]->hyst))
					{
						FobjArray[i]->currentPinRead = FobjArray[i]->hystPinRead;
						// call increase
						if (FobjArray[i]->pIncrease) {
							FobjArray[i]->pIncrease(FobjArray[i]->currentPinRead);
						}
					}
				else {
					//call same
					if (FobjArray[i]->pSame) {
						FobjArray[i]->pSame(FobjArray[i]->currentPinRead);
					}
				}
				if (FobjArray[i]->touchPinRead > FobjArray[i]->touchthreshold){
					if (FobjArray[i]->pTouchON) {
						FobjArray[i]->pTouchON(FobjArray[i]->currentPinRead);
					}
					
				}
				else {
					if (FobjArray[i]->pTouchOFF) {
						FobjArray[i]->pTouchOFF(FobjArray[i]->currentPinRead);
					}
				}
			}
	
	}
}


void Fader::FaderRead() {
	hystPinRead = analogRead(wiperPin);
#if defined (__MK20DX128__)
		if (MOTOR) {
touchPinRead = touchRead(touchPin);
			}
#endif
}

void Fader::Motor(int moveNo) {
	if (currentPinRead < (moveNo - 150)) {
		digitalWrite(dirPin, HIGH);
		digitalWrite(dirPinU, LOW);
		analogWrite(pwmPin, 167);
	}
	else if ((currentPinRead < (moveNo - 5)) && (currentPinRead >= (moveNo - 150))) {
		digitalWrite(dirPin, HIGH);
		digitalWrite(dirPinU, LOW);
		analogWrite(pwmPin, 100);
	}
	else if (currentPinRead >(moveNo + 150)) {
		digitalWrite(dirPin, LOW);
		digitalWrite(dirPinU, HIGH);
		analogWrite(pwmPin, 167);
	}
	else if ((currentPinRead > (moveNo + 5)) && (currentPinRead <= (moveNo + 150))) {
		digitalWrite(dirPin, LOW);
		digitalWrite(dirPinU, HIGH);
		analogWrite(pwmPin, 100);
	}
	else {
		Halt();
	}
}

void Fader::Halt()
{
	digitalWrite(dirPin, HIGH);
	analogWrite(pwmPin, 0);
}
