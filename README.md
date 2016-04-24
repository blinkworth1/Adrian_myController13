# Adrian_myController
1 Rotary encoder, with pushbutton switch (Adafruit 24 step)

1 Motorised fader, linear 10K from Sparkfun

1 Ardumoto controller (3.3V logic) from Sparkfun 
OR 
1 L298N HBridge (or similar) with 3.3V logic 
1 Joystick (4 switches) and 9 other switches / buttons

1 x 1 USB Midi interface (see pjrc.com/

A Teensy 3.x!

3 different modes, chosen on startup by button press. Also, chosen in the .ino: the mode select buttons (3,6,4 are default), hysteresis (fader deadspot), touch sensitivity (fader touch sensitivity), and HBridge mode.

MODE1.MidiController/USB Midi interface. 
MODE2.Faderport clone. 
MODE3.MAME controller. 

Because MAME uses keystrokes, a custom usb type of keyboard/midi is needed. To make a custom usb type, replace your current usb_desc.h file with the one in the repository. The current usb_desc.h is found in: 

Arduino/hardware/teensy/avr/cores/teensy3/

Then, use the MIDI type in the Arduino IDE dropdown USB Type menu for compiling.

Also, Arduino MIDI 4.2 library is required, if using the Midi interface.

See notes in the example .ino.

See the .txt file in the repository, and the .png for wiring and implementation nfo…





