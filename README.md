# Adrian_myController

3 Mode controller, with a motorized fader compatible with FaderPort. 

MODE1.MidiController/1x1 USB Midi interface. 
MODE2.Faderport clone. 
MODE3.MAME controller. 

Because MAME uses keystrokes, a custom usb type of keyboard/midi is needed. To make a custom usb type, replace your current usb_desc.h file with the one in the repository.

Also, Arduino MIDI 4.2 library is required, if using the USB Midi interface..... but the Midi interface is not working atm.

See the .txt file in the repository for installation instructions, wiring and implementation nfo… the USB Midi interface is still a work in progress at this stage.

Also, see notes in the example .ino.





