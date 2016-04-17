


#include <usb_midi.h>
#include <MIDI.h>
#include <myController13.h>
#include <Arduino.h>


elapsedMillis timer;


void Controller::begin(int one, int two, int three, int HYST, int TOUCHTHRESH, int NUM_INPUT_SWITCHES)
{

  pinMode(dirPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
  for (int i = 0; i < 15; i++)
  {
    delay (1);
    pinMode(switchesPinTable [i], INPUT_PULLUP );
  }
  do
  {
    if      (!digitalRead (one)) {
      choice = 1;
    }
    else if (!digitalRead (two)) {
      choice = 2;
    }
    else if (!digitalRead (three)) {
      choice = 3;
    }
   else  { 
    choice=0;
   }
  }
  while (choice == 0);
  delay(1000);
  //CHOICE [4] = choice;
  //midiA.sendSysEx (7, CHOICE, true);
myFader.HYST = HYST;
myFader.TOUCHTHRESH = TOUCHTHRESH;
mySwitches.NUM_INPUT_SWITCHES = NUM_INPUT_SWITCHES;

}


void Controller::Loop()
{

  channel = myRotary.rotaryUpdate (channel, choice);
  if (oldchannel != channel) {
  CHOICE [5] = channel;
  oldchannel = channel;
  //midiA.sendSysEx (7, CHOICE, true);
  }
 
  if ((choice == 1) || (choice == 2))
  {
    if (timer > 15) {
      timer = 0;
      myFader.RW(touchPin, wiperPin, dirPin, pwmPin, choice);
    }

     }
  mySwitches.RW (channel, choice);
}

void Fader::RW(int touchP, int wiperP, int dirP, int pwmP, uint8_t cho) // set up for capacitive touch
{
  PositionUSBRead ();
  PositionPinRead(wiperP);
  touchPinRead = touchRead (touchP);
  if (touchPinRead > TOUCHTHRESH)
  {
    if (TOUCHSENT == false) {
      usbMIDI.sendPolyPressure (127, 1, 1);
      TOUCHSENT = true;
    }
    PinWrite();

    Halt (dirP, pwmP);
  }
  else
  {
    if (TOUCHSENT == true) {
      usbMIDI.sendPolyPressure (127, 0, 1);
      TOUCHSENT = false;
    }
    Mota(dirP, pwmP);
  }
}

void Fader::PositionPinRead (int wiperP)
{
  hystPinRead = analogRead (wiperP);
  if  (currentPinRead > (hystPinRead + HYST) || hystPinRead > (currentPinRead + HYST))
  {
    currentPinRead = hystPinRead;
  }
}

void Fader::PositionUSBRead()
{
  if (  usbMIDI.read()  &&  (usbMIDI.getType() == 3)  )
  {
    if (usbMIDI.getData1() == 0 )
    {
      readMSB = (usbMIDI.getData2());
      delay (1);
      usbMIDI.read();
      readLSB = usbMIDI.getData2();
      currentUSBRead = (readMSB << 7) | (readLSB & 0x07F);
    }
  }
}

void Fader::Mota(int dirP, int pwmP)
{
  if (currentPinRead < (currentUSBRead - 150)) {
    digitalWrite(dirP, LOW);
    analogWrite(pwmP, 167);
  }
  else if ( (currentPinRead < (currentUSBRead - 5) ) && (currentPinRead >= (currentUSBRead - 150)) ) {
    digitalWrite(dirP, LOW);
    analogWrite(pwmP, 135);
  }
  else if (currentPinRead > (currentUSBRead + 150)) {
    digitalWrite(dirP, HIGH);
    analogWrite(pwmP, 167);
  }
  else if ( (currentPinRead > (currentUSBRead + 5) ) && (currentPinRead <= (currentUSBRead + 150)) ) {
    digitalWrite(dirP, HIGH);
    analogWrite(pwmP, 135);
  }
  else {
    Halt(dirP, pwmP);
  }
}

void Fader::Halt (int dirP, int pwmP)
{
  digitalWrite(dirP, HIGH);
  analogWrite(pwmP, 0);
}

void Fader::PinWrite ()
{
  sendMSB = currentPinRead >> 3;
  sendLSB = currentPinRead & 0x00F;
  usbMIDI.sendControlChange (0, sendMSB, 1);
  //delay(1); // 500 micros is probably better here??? is it even needed??
  usbMIDI.sendControlChange (32, sendLSB, 1);
}

void Switches::RW (uint8_t chan, uint8_t cho)
{
  noInterrupts();
  int pre = Switches::pressed;
  int rel = Switches::released;
  interrupts();
  for (int i = 0; i < NUM_INPUT_SWITCHES; i++)
  {
    if (pre & (1 << i))
    {
      if  (!(mask & (1 << i) ))
      {
        switch (cho)
        {
          case 1:
            if (i < 8) {
             //midiA.sendSysEx (6, MMCTransport[i], false);
             //midiB.sendSysEx (6, MMCTransport[i], false);
             usbMIDI.sendSysEx (6, MMCTransport[i]);
             mask |= (1 << i);
           }
           else 
            {
              usbMIDI.sendNoteOn (sendNotes[i], 127, chan);
              //midiA.sendNoteOn (sendNotes[i], 127, chan);
              //midiB.sendNoteOn (sendNotes[i], 127, chan);
              mask |= (1 << i);
            }
            break;
          case 2:
            if (i == 1) {
              usbMIDI.sendPolyPressure (20, 1, 1);
              usbMIDI.sendPolyPressure (21, 1, 1);
              mask |= (1 << i);
            }
            else if (i == 0) {
              usbMIDI.sendPolyPressure (20, 1, 1);
              usbMIDI.sendPolyPressure (19, 1, 1);
              mask |= (1 << i);
            }
            else if (i == 2) {
              usbMIDI.sendPolyPressure (19, 1, 1);
              mask |= (1 << i);
            }
            else if (i == 3) {
              usbMIDI.sendPolyPressure (21, 1, 1);
              mask |= (1 << i);
            }
            else    {
              usbMIDI.sendPolyPressure (sendFPPdata[i], 1, 1);
              mask |= (1 << i);
            }
            break;
          case 3:
            Keyboard.press(sendKeys [i] | (0x40 << 8));
            mask |= (1 << i);
            break;
        }
      }
    }
    else if (rel & (1 << i)) {
      if ((mask & (1 << i)))
      {
        switch (cho)
        {
          case 1:
           if (i < 8) {mask &= ~(1 << i); }
              
             else 
            {
              usbMIDI.sendNoteOff (sendNotes[i], 0, chan);
              //midiA.sendNoteOn (sendNotes[i], 0, chan);
              //midiB.sendNoteOn (sendNotes[i], 0, chan);
              mask &= ~(1 << i);
            }
            break;
          case 2:
            if (i == 1) {
              usbMIDI.sendPolyPressure (20, 0, 1);
              usbMIDI.sendPolyPressure (21, 0, 1);
              mask &= ~(1 << i);
            }
            else if (i == 0) {
              usbMIDI.sendPolyPressure (20, 0, 1);
              usbMIDI.sendPolyPressure (19, 0, 1);
              mask &= ~(1 << i);
            }
            else if (i == 2) {
              usbMIDI.sendPolyPressure (19, 0, 1);
              mask &= ~(1 << i);
            }
            else if (i == 3) {
              usbMIDI.sendPolyPressure (21, 0, 1);
              mask &= ~(1 << i);
            }
            else    {
              usbMIDI.sendPolyPressure (sendFPPdata[i], 0, 1);
              mask &= ~(1 << i);
            }
            break;
          case 3:
            Keyboard.release(sendKeys [i] | (0x40 << 8));
            mask &= ~(1 << i);
            break;
        }
      }
    }
  }
}

uint8_t Rotary::rotaryUpdate (uint8_t chan, uint8_t cho)
{
  noInterrupts();
  readin = ( enc_states[( readout & 0x0F )]);
  if (rotaryState) {
    rotaryState = false;
    count += 1;
  }
  interrupts();
  if (count == 2) {
    count = 0;
    chan = Rotary::RW (chan, cho);
  }
  return chan;
}

uint8_t Rotary::RW (uint8_t chan, uint8_t cho)
{
  if (readin > 0)
  {
    switch (cho)
    {
      case 1: chan++; if (chan > 16)
        {
          chan = 16;
        }
        break;
      case 2:  usbMIDI.sendPitchBend (-8192, 1);
        break;
      case 3:      Keyboard.press(KEY_LEFT | (0x40 << 8));
        Keyboard.release(KEY_LEFT | (0x40 << 8));
        break;
    }
  }
  if (readin < 0)
  {
    switch (cho)
    {
      case 1: chan--; if (chan < 1)
        {
          chan = 1;
        }
        break;
      case 2:  usbMIDI.sendPitchBend (0, 1);
        break;
      case 3: Keyboard.press(KEY_RIGHT | (0x40 << 8));
        Keyboard.release(KEY_RIGHT | (0x40 << 8));
        break;
    }
  }
  return chan;
}


