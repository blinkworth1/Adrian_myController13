# Adrian_myController13
Midi interface, FaderPort clone (a single motorised fader with transport / automation controls), MAME controller, all in one!  But the midi interface and is borked atm ... next week, maybe ...

This is a project for the Teensy 3.x.  All hail PaulStoffregen, master of the MCU singularity.

1 Rotary Encoder, 1 Motorised fader (using native FaderPort protocol), 1 Joystick (4 switches) and 9 other switches / buttons, and a 2 x 2 USB Midi interface, merger and splitter (BUT THE MIDI INTERFACE IS BORKED ATM!!!)

3 different modes choosable on startup by button press. MODE1.Midi interface MODE2.Faderport clone MODE3.MAME controller. BUT MODE 1 DOESN'T WORK ATM !!!!!!!!

Because MAME uses keystrokes, a cutom usb type of 'keyboard/midi' is needed, so usb_desc.h in the repositiory has to go in your teensy cores folder, to replace the current usb_desc.h file

Also arduino MIDI 4.2 library is required.... well it would be, if the Midi interface worked ...

The example in the repository has further instructions on usage.  The .h and .cpp files go in your arduino library folder (assuming you are using the arduino ide), and the example should compile sweet as...

The digital pins used are, in order, 2, 14, 7, 8, 15, 22, 23, 18, 3, 4, 5, 11, 6. SO ... if you want 4 digital pins, use 2, 14, 7, and 8 (the first 4).  They are the pins for the joystick.

The rotary encoder is on digital 16 and 17 ...

The fader wiper in is pin A14, and the capacitive touch line is pin 19.

The motor controller for the fader is 'standard' with a direction and a speed pin ... PWM speed pin is 20, and the direction pin is 21.

The 2 x 2 Midi interface attaches to Serial1 and Serial2, using standard Midi wiring (optocoupler etc etc)... but guess what ... the code is borked at the moment ... sigh.



