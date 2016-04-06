# Adrian_myController13
Midi interface, midi controller, FaderPort clone, and more!

1 Rotary Encoder, 1 Motorised fader (using native FaderPort protocol "FPP"), 1 Joystick (4 switches) and 9 other switches / buttons, and a 2 x 2 USB Midi interface, merger and splitter!
3 different modes choosable on startup by button press. MODE1.MidiController MODE2.Faderport clone MODE3.MAME controller. Because MAME uses keystrokes, a cutom usb type of keyboard/midi is needed
Obviously if you want to ditch MAME, you won't need a custom usb type
Also MIDI 4.2 library is required.

// The digital pins used are, in order, 2, 14, 7, 8, 15, 22, 23, 18, 3, 4, 5, 11, 6.
// SO ... if you want 4 digital pins, use 2, 14, 7, and 8 (the first 4).
// The rotary encoder is on digital 16 and 17 ...
// The fader wiper in is pin A14, and the touch line is pin 19.  See below for more on the fader MOTOR
/*

Switches        2       14      7       8      15      22     23     18     11     5      4     3      6

mode 1 USB      MMC     MMC     MMC     MMC    MMC     MMC    MMC    Note70 71     72     73    74     Note75
                STOP    PLAY    FFWD    RWD    PCHIN   PCHOUT PAUSE
       midi1       ""                  ""           ""               Note70         """"               Note75
       midi2       ""                  ""           ""               Note70         """"               Note75


mode 2 USB      AfterT  AfterT  AfterT  AfterT
                FPP     FPP     FPP     FPP    ...........                      ............           FPP
                bankL   bankR   ChL     ChR    PLAY    PCH    STOP   RW     FF     REC    TCH   WRTE   SHIFT
       midi1    N/A                                     ...                                            N/A
       midi2    N/A                                     ...                                            N/A


mode 3 USB      DOWN    UP      LEFT    RIGHT   A       B      C      D     E
       midi1    N/A                                     ...                                            N/A
       midi2    N/A                                     ...                                            N/A


Encoder   L         R

mode 1    CH 16-1   CH 1-16 (for swiches send)
mode 2    FPP       FPP
          PANL      PANR
mode 3    LEFT      RIGHT


Fader

mode 1     10 bit messages MSB CC 00 (3bits) LSB CC 32 (7bits)
mode 2     10 bit messages MSB CC 00 (3bits) LSB CC 32 (7bits)
mode 3     N/A


MIDI RX1 TX1 RX2 TX2
Midimerger / split / thru incl USB (but USB in /out has no thru...)

RX1 to TX1, TX2, and USB
RX2 to TX1, TX2, and USB
USB to TX1 and TX2

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
