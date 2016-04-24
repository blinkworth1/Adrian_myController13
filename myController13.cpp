#include <myController13.h>

static Controller * Cptr;
static void SwitchesRead(void);
static void FaderRead(void);
static void RotaryRead(void);

void Controller::begin(int one, int two, int three, int hystin, int touchthreshin)
{
  touch = touchthreshin;
  hyst = hystin;
  pinMode(dirPin, OUTPUT);
#if defined HBRIDGE
  pinMode(dirPinU, OUTPUT)
#endif
  pinMode(pwmPin, OUTPUT);
  for (int i = 0; i < 15; i++)
  {
    delay (1);
    pinMode(switchesPinTable [i], INPUT_PULLUP );
  }
  do
  {
    if (!digitalRead (one)) {
      choice = 1;
    }
    else if (!digitalRead (two)) {
      choice = 2;
    }
    else if (!digitalRead (three)) {
      choice = 3;
    }
    else {
      choice = 0;
    }
  }
  while (choice == 0);

  delay(1000);


  /*
    //serial sysex out message of current choice and channel
    CHOICE [4] = choice;
    midiA.sendSysEx (7, CHOICE, true);

    if (oldchannel != channel) {
    CHOICE [5] = channel;
    oldchannel = channel;
    midiA.sendSysEx (7, CHOICE, true);
    }

  */
  Cptr = this;
  static IntervalTimer SwitchesTimer;
  static IntervalTimer FaderTimer;
  static IntervalTimer RotaryTimer;

  SwitchesTimer.begin (SwitchesRead, 151);
  FaderTimer.begin (FaderRead, 19043);
  RotaryTimer.begin (RotaryRead, 599);
}

static void SwitchesRead (void)
{
  Controller *o = Cptr;
  o->switchesRaw = (   (1 << 13) | digitalRead (6) << 12) | (digitalRead (3) << 11) | (digitalRead (4) << 10) | (digitalRead (5) << 9) | (digitalRead (11) << 8) | digitalRead(18) << 7 | ((GPIOC_PDIR & 0x007) << 4) | (GPIOD_PDIR & 0x00F);
}

static void FaderRead (void)
{
  Controller *o = Cptr;
  o->hystPinRead = analogRead (o->wiperPin);
  o->touchPinRead = touchRead (o->touchPin);
  o->faderCount++;
}

static void RotaryRead (void)
{
  Controller *o = Cptr;
  o->rotaryRaw = (GPIOB_PDIR  & 3) | (1 << 3) ;
  o->rotaryCount++;
}

void Controller::faderHalt ()
{
  digitalWrite(dirPin, HIGH);
  analogWrite(pwmPin, 0);
}

void Controller::faderWrite () {
  if (!(choice == 3))
  {
    noInterrupts();
    if (faderCount != oldFaderCount) {
      oldFaderCount = faderCount;
      if (currentPinRead > (hystPinRead + hyst) || hystPinRead > (currentPinRead + hyst)) {
        currentPinRead = hystPinRead;
      }
      if (touchPinRead > touch) {
        touchActive = true;
      }
      else {
        touchActive = false;
      }
      interrupts();

      if ( usbMIDI.read() && (usbMIDI.getType() == 3) ) {
        if (usbMIDI.getData1() == 0 ) {
          uint16_t readMSB = usbMIDI.getData2();
          while (!(usbMIDI.read())) {}
          uint16_t readLSB = usbMIDI.getData2();
          currentUSBRead = (readMSB << 7) | (readLSB & 0x07F);
        }
      }
    if (!touchActive) {Motor();}
    }
    if (touchActive) {faderHalt ();}
    if ((touchActive) && (TOUCHSENT == false)) {
      usbMIDI.sendPolyPressure (127, 1, 1);
      Serial.println(touchPinRead);
      TOUCHSENT = true;
    }
    if ((!(touchActive)) && (TOUCHSENT == true)) {
      usbMIDI.sendPolyPressure (127, 0, 1);
      TOUCHSENT = false;
    }
    if ((touchActive) && (currentPinRead != oldPinRead)) {
      oldPinRead = currentPinRead;
      sendMSB = currentPinRead >> 3;
      sendLSB = currentPinRead & 0x00F;
      usbMIDI.sendControlChange (0, sendMSB, 1);
      usbMIDI.sendControlChange (32, sendLSB, 1);
    }
  }
}

void Controller::Motor() {
    if (currentPinRead < (currentUSBRead - 150)) {
        digitalWrite(dirPin, LOW);
        Serial.println(String("a") + currentUSBRead);
#if defined HBRIDGE
        digitalWrite(dirPinU, HIGH)
#endif
        analogWrite(pwmPin, 167);
      }
      else if ( (currentPinRead < (currentUSBRead - hyst) ) && (currentPinRead >= (currentUSBRead - 150)) ) {
        digitalWrite(dirPin, LOW);
#if defined HBRIDGE
        digitalWrite(dirPinU, HIGH)
#endif
        analogWrite(pwmPin, 130);
      }
      else if (currentPinRead > (currentUSBRead + 150)) {
        digitalWrite(dirPin, HIGH);
#if defined HBRIDGE
        digitalWrite(dirPinU, LOW)
#endif
        analogWrite(pwmPin, 167);
      }
      else if ( (currentPinRead > (currentUSBRead + hyst) ) && (currentPinRead <= (currentUSBRead + 150)) ) {
        digitalWrite(dirPin, HIGH);
#if defined HBRIDGE
        digitalWrite(dirPinU, LOW)
#endif
        analogWrite(pwmPin, 130);
      }
      else {
        faderHalt();
      }
    }

void Controller::switchesWrite ()
{
  noInterrupts();
  if (switchesRaw & (1 << 13)) {
    switchesRaw &= ~(1 << 13);
    for (int i = 0; i < switchesNo; i++)
    {
      switchesArray[i] <<= 1;
      switchesArray[i] |= ((switchesRaw >> i) & 0x001);
      interrupts();
      if ((switchesArray[i] & 0x007) == 0x007) {
        switchesData &= ~(1 << i); // active low,
      }
      else if ((switchesArray[i] & 0x007) == 0x000) {
        switchesData |= (1 << i);
      }
    }
  }
  for (int i = 0; i < switchesNo; i++)
  {
    if (switchesData & (1 << i))
    {
      if (!(pressed & (1 << i) ))
      {
        switch (choice)
        {
          case 1:
            if (i < 8) {
              //midiA.sendSysEx (6, MMCTransport[i], false);
              //midiB.sendSysEx (6, MMCTransport[i], false);
              usbMIDI.sendSysEx (6, MMCTransport[i]);
              pressed |= (1 << i);
            }
            else
            {
              usbMIDI.sendNoteOn (sendNotes[i], 127, channel);
              //midiA.sendNoteOn (sendNotes[i], 127, channel);
              //midiB.sendNoteOn (sendNotes[i], 127, channel);
              pressed |= (1 << i);
            }
            break;
          case 2:
            if (i == 1) {
              usbMIDI.sendPolyPressure (20, 1, 1);
              usbMIDI.sendPolyPressure (21, 1, 1);
              pressed |= (1 << i);
            }
            else if (i == 0) {
              usbMIDI.sendPolyPressure (20, 1, 1);
              usbMIDI.sendPolyPressure (19, 1, 1);
              pressed |= (1 << i);
            }
            else if (i == 2) {
              usbMIDI.sendPolyPressure (19, 1, 1);
              pressed |= (1 << i);
            }
            else if (i == 3) {
              usbMIDI.sendPolyPressure (21, 1, 1);
              pressed |= (1 << i);
            }
            else {
              usbMIDI.sendPolyPressure (sendFPPdata[i], 1, 1);
              pressed |= (1 << i);
            }
            break;
          case 3:
            Keyboard.press(sendKeys [i] | (0x40 << 8));
            pressed |= (1 << i);
            break;
        }
      }
    }
    else {
      if ((pressed & (1 << i)))
      {
        switch (choice)
        {
          case 1:
            if (i < 8) {
              pressed &= ~(1 << i);
            }
            else
            {
              usbMIDI.sendNoteOff (sendNotes[i], 0, channel);
              //midiA.sendNoteOn (sendNotes[i], 0, channel);
              //midiB.sendNoteOn (sendNotes[i], 0, channel);
              pressed &= ~(1 << i);
            }
            break;
          case 2:
            if (i == 1) {
              usbMIDI.sendPolyPressure (20, 0, 1);
              usbMIDI.sendPolyPressure (21, 0, 1);
              pressed &= ~(1 << i);
            }
            else if (i == 0) {
              usbMIDI.sendPolyPressure (20, 0, 1);
              usbMIDI.sendPolyPressure (19, 0, 1);
              pressed &= ~(1 << i);
            }
            else if (i == 2) {
              usbMIDI.sendPolyPressure (19, 0, 1);
              pressed &= ~(1 << i);
            }
            else if (i == 3) {
              usbMIDI.sendPolyPressure (21, 0, 1);
              pressed &= ~(1 << i);
            }
            else {
              usbMIDI.sendPolyPressure (sendFPPdata[i], 0, 1);
              pressed &= ~(1 << i);
            }
            break;
          case 3:
            Keyboard.release(sendKeys [i] | (0x40 << 8));
            pressed &= ~(1 << i);
            break;
        }
      }
    }
  }

}

void Controller::rotaryWrite()
{
  noInterrupts();
  if (rotaryCount != oldRotaryCount) {
    oldRotaryCount = rotaryCount;
    rotaryData <<= 2; //remember previous state
    for (int i = 0; i < 2; i++)
    {
      rotaryArray[i] <<= 1;
      rotaryArray[i] |= ((rotaryRaw >> i) & 0x001);

      if ( (rotaryArray[i] & 0x01F ) == 0x01F) {
        rotaryData |= (1 << i);
      }
      else if ( (rotaryArray[i] & 0x01F) == 0x000) {
        rotaryData &= ~(1 << i);
      }
    }
    if (rotaryCount == 4) {
      rotaryCount = 0;
      interrupts();
      rotaryState = ( enc_states[(rotaryData & 0x0F )]);
      if (rotaryState > 0)
      {
        switch (choice)
        {
          case 1: channel++; if (channel > 16)
            {
              channel = 16;
            }
            break;
          case 2: usbMIDI.sendPitchBend (-8192, 1);
            break;
          case 3: Keyboard.press(KEY_LEFT | (0x40 << 8));
            Keyboard.release(KEY_LEFT | (0x40 << 8));
            break;
        }
      }
      if (rotaryState < 0)
      {
        switch (choice)
        {
          case 1: channel--; if (channel < 1)
            {
              channel = 1;
            }
            break;
          case 2: usbMIDI.sendPitchBend (0, 1);
            break;
          case 3: Keyboard.press(KEY_RIGHT | (0x40 << 8));
            Keyboard.release(KEY_RIGHT | (0x40 << 8));
            break;
        }
      }
    }
  }
}
