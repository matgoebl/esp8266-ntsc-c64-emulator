# esp8266-ntsc-c64-emulator
C64 emulator on the ESP8266 with NTSC output and PS/2 keyboard input.

Original author: Jan Ostman.
Published November 12, 2018 on https://www.hackster.io/janost/esp8266-ntsc-c64-emulator-b91a35 under GPL3+.

Enhancements by Matthias Goebl at https://github.com/matgoebl/esp8266-ntsc-c64-emulator/
- PS/2 keyboard input with hotkeys for reset (press scroll-lock) and serial debugging
- Cursor display
- Wifi and OTA update (press scroll-lock for reset, followed by escape within one second to enter OTA mode)
- Load compiled-in PRGs (press F9 to load and run a hello-world basic program)

## How to compile-in PRGs
- create e.g. hello-world.bas
- compile basic source into token: `petcat -w2 -o hello-world.prg hello-world.bas` (petcat comes with [http://vice-emu.sourceforge.net/](VICE))
- convert binary into a C include file: `xxd -i hello-world.prg > hello-world.h`
- adapt hello-world.h: change `unsigned char` into `const PROGMEM unsigned char`

## Limitations
This simple emulator is a proof-of-concept.
It only emulates the 6502 CPU of a C64 with basic video output.
It lacks emulation of the SID chip for sound output and the VIC chip for sprites and graphics output.
Therefore it is only suitable for playing with commodore basic.

## Hardware circuitry

### Video signal

This adaptor generates the CVBS video output signal from ESP RX pi.
When connected, flashing via serial doesn'n work - that's the reason OTA has been added.

![adaptor for CVBS video signal](doc/esp8266_cbvs.jpg?raw=true "adaptor for CVBS video signal")

### PS/2 keyboard

This simple level shifter limits the 5V output from PS/2 keyboard to 3.3V ESP inputs:

    PS/2 Data  -----<|----- ESP D3
                  1N4148

    PS/2 Clock -----<|----- ESP D2
                  1N4148
    (some keyboards might need an additional pullup to 5V at keyboard side)

### Audio output

Audio output is so far only used for beep output at startup, no SID emulation yet.

    GND -------#####---------||----- ESP D1
            Mini-Speaker     4u7
