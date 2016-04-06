

#include <usb_midi.h>
#include <MIDI.h>
#include "myController13.h"

#define NUM_INPUT_SWITCHES 13

#define MOTOR
//Comment out '#define MOTOR' above if you DO NOT want to use the fader MOTOR on mode 1 and 2.
//If using the MOTOR, the pwm / speed pin is 20, and the direction pin is 21.
//The fader wiper in is pin A14, and the capacative touch line is pin 19.

#define TOUCHTHRESH 2000     // Threshold for fader capacitive touch ... the fader is assumed to have capacative touch even if the MOTOR is not used. 
#define HYST 2               // Amount of hysteresis jitter suppresion for the fader. A capacitor across wiper and ground is otherwise needed.

/*In your setup, call ...
myController.Mode (mode1 pin, mode2 pin, mode3 pin)
substituting your own pin numbers for the relevant mode
3 modes ...1.MidiController 2.Faderport clone 3.MAME controller
So ...
myController.Mode (3,4,6)
makes pin 3 the switch for mode 1, pin 4 for mode 2, pin 6 for mode 3
Once a selection is made via the switches, there is no way to change it unless you power cycle... after selection the pins have their 'usual functions' (above)
*/

//Finally, in your loop, call myController.Loop();

Controller myController13;

MIDI_CREATE_INSTANCE (HardwareSerial, Serial1, midiA);
MIDI_CREATE_INSTANCE (HardwareSerial, Serial2, midiB);

elapsedMillis timer;

void MidiThru ()
{
  if (usbMIDI.read() &&  usbMIDI.getType() < 7)
  {
    midi::MidiType type = (midi::MidiType) usbMIDI.getType();
    byte d1 = usbMIDI.getData1();
    byte  d2 = usbMIDI.getData2();
    byte Chan = usbMIDI.getChannel();
    midiA.send(type, d1, d2, Chan);
    midiB.send(type, d1, d2, Chan);
  }

  if (midiA.read())
  {
    midiB.send(midiA.getType(),
               midiA.getData1(),
               midiA.getData2(),
               midiA.getChannel());
    midiA.setHandleNoteOn(OnNoteOn);
    midiA.setHandleNoteOff(OnNoteOff);
    midiA.setHandleControlChange(OnControlChange);
    midiA.setHandleAfterTouchPoly (OnVelocityChange);
    midiA.setHandleProgramChange(OnProgramChange);
    midiA.setHandleAfterTouchChannel(OnAfterTouch);
    midiA.setHandlePitchBend(OnPitchChange);
  }
  if (midiB.read())
  {
    midiA.send(midiB.getType(),
               midiB.getData1(),
               midiB.getData2(),
               midiB.getChannel());
    midiB.setHandleNoteOn(OnNoteOn);
    midiB.setHandleNoteOff(OnNoteOff);
    midiB.setHandleControlChange(OnControlChange);
    midiB.setHandleAfterTouchPoly(OnVelocityChange);
    midiB.setHandleProgramChange(OnProgramChange);
    midiB.setHandleAfterTouchChannel(OnAfterTouch);
    midiB.setHandlePitchBend(OnPitchChange);
  }
}

void OnNoteOn (byte channel, byte note, byte velocity)
{
  usbMIDI.sendNoteOn(note, velocity, channel);
  usbMIDI.send_now();
}

void OnNoteOff (byte channel, byte note, byte velocity)
{
  usbMIDI.sendNoteOff(note, velocity, channel);
  usbMIDI.send_now();
}

void OnControlChange (byte channel, byte number, byte value)
{
  usbMIDI.sendControlChange(number, value, channel);
  usbMIDI.send_now();
}

void OnVelocityChange(byte channel, byte note, byte pressure)
{
  usbMIDI.sendPolyPressure(note, pressure, channel);
  usbMIDI.send_now();
}

void OnProgramChange (byte channel, byte program)
{
  usbMIDI.sendProgramChange(program, channel);
  usbMIDI.send_now();
}

void OnAfterTouch(byte channel, byte pressure)
{
  usbMIDI.sendAfterTouch(pressure, channel);
  usbMIDI.send_now();
}

void OnPitchChange(byte channel, int bend)
{
  usbMIDI.sendPitchBend(bend, channel);
  usbMIDI.send_now();
}
/*
void OnNoteOnB (byte channel, byte note, byte velocity)
{
  usbMIDI.sendNoteOn(note, velocity, channel);
  usbMIDI.send_now();
}

void OnNoteOffB (byte channel, byte note, byte velocity)
{
  usbMIDI.sendNoteOff(note, velocity, channel);
  usbMIDI.send_now();
}

void OnControlChangeB (byte channel, byte number, byte value)
{
  usbMIDI.sendControlChange(number, value, channel);
  usbMIDI.send_now();
}

void OnVelocityChangeB(byte channel, byte note, byte pressure)
{
  usbMIDI.sendPolyPressure(note, pressure, channel);
  usbMIDI.send_now();
}

void OnProgramChangeB (byte channel, byte program)
{
  usbMIDI.sendProgramChange(program, channel);
  usbMIDI.send_now();
}

void OnAfterTouchB(byte channel, byte pressure)
{
  usbMIDI.sendAfterTouch(pressure, channel);
  usbMIDI.send_now();
}

void OnPitchChangeB(byte channel, int bend)
{
  usbMIDI.sendPitchBend(bend, channel);
  usbMIDI.send_now();
}

*/
IntervalTimer RotaryTimer;
volatile uint8_t rotaryArray [] = {0x01F, 0x01F};
volatile uint16_t Rotary::readout = 0x000;
volatile bool Rotary::rotaryState = false;

void RotaryRead (void)
{ Rotary::rotaryState = true;
  Rotary::readout <<= 2;                   //remember previous state
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

IntervalTimer SwitchesTimer;
volatile uint8_t switchArray [] = {0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007, 0x007};
volatile uint16_t Switches::pressed = 0;
volatile uint16_t Switches::released = 0;

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

void Controller::Mode(int one, int two, int three)
{
  midiA.begin(MIDI_CHANNEL_OMNI);
  midiB.begin(MIDI_CHANNEL_OMNI);
#ifdef MOTOR
  pinMode(dirPin, OUTPUT);
  pinMode(pwmPin, OUTPUT);
#endif
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
  CHOICE [4] = choice;
  midiA.sendSysEx (7, CHOICE, true);
  SwitchesTimer.begin (SwitchesRead, 312);
  RotaryTimer.begin (RotaryRead, 567);

}

void Controller::Loop()
{

  channel = myRotary.rotaryUpdate (channel, choice);
  if (oldchannel != channel) {
  CHOICE [5] = channel;
  oldchannel = channel;
  midiA.sendSysEx (7, CHOICE, true);
  }
 
  if ((choice == 1) || (choice == 2))
  {
    if (timer > 15) {
      timer = 0;
      myFader.RW(touchPin, wiperPin, dirPin, pwmPin, choice);
    }

    MidiThru();
  }
  mySwitches.RW (channel, choice);
}

void Fader::RW(int touchP, int wiperP, int dirP, int pwmP, uint8_t cho) // set up for capacitive touch
{
#ifdef MOTOR
  PositionUSBRead ();
#endif
  PositionPinRead(wiperP);
  touchPinRead = touchRead (touchP);
  if (touchPinRead > TOUCHTHRESH)
  {
    if (TOUCHSENT == false) {
      usbMIDI.sendPolyPressure (127, 1, 1);
      TOUCHSENT = true;
    }
    PinWrite();
#ifdef MOTOR
    Halt (dirP, pwmP);
#endif
  }
  else
  {
    if (TOUCHSENT == true) {
      usbMIDI.sendPolyPressure (127, 0, 1);
      TOUCHSENT = false;
    }
#ifdef MOTOR
    Mota(dirP, pwmP);
#endif
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

#ifdef MOTOR
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
#endif

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
             midiA.sendSysEx (6, MMCTransport[i], false);
             midiB.sendSysEx (6, MMCTransport[i], false);
             usbMIDI.sendSysEx (6, MMCTransport[i]);
             mask |= (1 << i);
           }
           else 
            {
              usbMIDI.sendNoteOn (sendNotes[i], 127, chan);
              midiA.sendNoteOn (sendNotes[i], 127, chan);
              midiB.sendNoteOn (sendNotes[i], 127, chan);
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
              midiA.sendNoteOn (sendNotes[i], 0, chan);
              midiB.sendNoteOn (sendNotes[i], 0, chan);
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
        //usbMIDI.sendPolyPressure (125, 0, 1);
        break;
      case 3: Keyboard.press(KEY_RIGHT | (0x40 << 8));
        Keyboard.release(KEY_RIGHT | (0x40 << 8));
        break;
    }
  }
  return chan;
}

void setup()
{
  myController13.Mode(3, 6, 4);
}

void loop()
{
  myController13.Loop();
}


