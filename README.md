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
