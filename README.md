# esp8266-ntsc-c64-emulator
C64 emulator running on the ESP8266 with NTSC video output and PS/2 keyboard input.

Original author: Jan Ostman.
Published November 12, 2018 on https://www.hackster.io/janost/esp8266-ntsc-c64-emulator-b91a35 under GPL3+.

Enhancements by Matthias Goebl at https://github.com/matgoebl/esp8266-ntsc-c64-emulator/
- PS/2 keyboard input with hotkeys
- Cursor display
- Wifi and OTA update
- Load builtin PRGs with hotkeys


## Limitations

This simple emulator is a proof-of-concept.

It only emulates the 6502 CPU of a C64 with basic video output for [PETSCII](https://en.wikipedia.org/wiki/PETSCII) characters.  
It lacks emulation of the [SID chip](https://en.wikipedia.org/wiki/MOS_Technology_6582) for sound output and
the [VIC chip](https://en.wikipedia.org/wiki/MOS_Technology_VIC) for sprites and graphics output.  
It provides only 16KB of RAM (C64 has 64KB), it shows 14335 basic bytes free (C64: 38911 bytes free).  
There is no joystick support, only keyboard input into the basic keyboard buffer,
so many assembly programs that access hardware keyboard registers wont work.  
Therefore it is only usable for short commodore basic programs with PETSCII output and no PEEKs and POKEs.


## Used libraries

The following arduino esp8266 packages are needed:
- WiFiManager
- ArduinoOTA
- PS2Keyboard

PS2Keyboard requires a small fix: Add the following lines to `packages/esp8266/hardware/esp8266/2.4.2/variants/generic/common.h`:

    #ifndef CORE_INT_EVERY_PIN
    #define CORE_INT_EVERY_PIN
    #endif /* CORE_INT_EVERY_PIN */


## OTA mode

Add your wifi ssid and psk to `wifi_credentials.h`.
Press escape within 1 second after ESP full reset (power-on or shift-escape).
ESP will then enter wifi OTA mode.
Press shift-escape again to boot to C64 mode.


## Keyboard layout

| Key         | Function       |
|-------------|----------------|
| Scroll-lock | Pause on/off   |
| Esc         | C64 reset      |
| Shift-Esc   | ESP full reset |
| Tab         | Run            |
| Shift-Tab   | Stop           |
| Shift-F1 .. Shift-F12 | Load builtin PRG #1 .. #12 |


## How to compile-in PRGs

- create e.g. hello-world.bas
- compile basic source into token: `petcat -w2 -o hello-world.prg hello-world.bas` (petcat comes with [VICE](http://vice-emu.sourceforge.net/)
- convert binary into a C include file: `xxd -i hello-world.prg > hello-world.h`
- adapt hello-world.h: change `unsigned char` into `const PROGMEM unsigned char`

For extracting PRGs from a D64 file use `c1541 -attach *.d64 -extract`.

This is automatically performed by the script `prg/prg2h.sh`, that creates `ESP8266-NTSC-C64/builtinprg_X.h`.


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



## Builtin PRGs for demonstation


When looking for suitable demonstration programs, I found the [BASIC 10Liners contest](http://gkanold.wixsite.com/homeputerium/basic-10liners-2018):
- It allows only short basic programs with no assembler and limited POKEs.
- I found several programs with keyboard input and PETSCII output.

There is no well-known open source license given for the 10liners, however the authors agreed on publication of the source code.

I added my selection of working games into the [folder prg/](prg/).  
This set is automatically built into the emulator and quickly loaded by pressing function keys (shift + F1..F12).  



### 1. 10 Print

A one-liner that prints a labyrinth. [https://10print.org/](more)


### 2. Mini Tetris

by marcosj, Buenos Aires, Argentina

Keys: A S D

posted on https://www.lemon64.com/forum/viewtopic.php?t=30998 ( public domain according to [gamebase64](http://www.gamebase64.com/game.php?id=24258&d=45) )

edited: changed `rnd(0)` to `rnd(1)`


### 3. Snake

by Davide Fichera @Naufr4g0 (assumed, as this is also available [here](goo.gl/JGWFqs) pointed from https://twitter.com/naufr4g0

Keys: A D

Downloaded from [basic 10liners 2016 for C64](http://gkanold.wixsite.com/homeputerium/blog) and [NOMAM2016 Dropbox](https://www.dropbox.com/sh/vigwx5pko1mbies/AAAH81tgcGzhhUlHEakmUXS6a/C64)


### 4. GorillaX

by Davide Fichera @Naufr4g0

Keys: enter number, return

Downloaded from [basic 10liners 2017 for C64](http://gkanold.wixsite.com/homeputerium/games-list-2017) and [Dropbox](https://www.dropbox.com/sh/6osm1dr46eev4wj/AABCwVsPRGhMffYNZyAAT8e-a/C64)


### 5. Beer Hunter

by Sander Alsema

Keys: , . space

Downloaded from [basic 10liners 2017 for C64](http://gkanold.wixsite.com/homeputerium/games-list-2017) and [Dropbox](https://www.dropbox.com/sh/6osm1dr46eev4wj/AABCwVsPRGhMffYNZyAAT8e-a/C64)


### 6. Bomber

by Georg "Endurion" Rottensteiner

Keys: Space

Downloaded from [basic 10liners 2017 for C64](http://gkanold.wixsite.com/homeputerium/kopie-von-games-list-2018-work) and [Dropbox](https://www.dropbox.com/sh/uufbtdvy6ipqds3/AAAaM_yW_Ifyk_EqFkeNtLDCa/C64)


### 7. Last Outpost

by 5ace

Keys: Left Right

Downloaded from [basic 10liners 2017 for C64](http://gkanold.wixsite.com/homeputerium/kopie-von-games-list-2018-work) and [Dropbox](https://www.dropbox.com/sh/uufbtdvy6ipqds3/AAAaM_yW_Ifyk_EqFkeNtLDCa/C64)


### 8. Duck!!!

by Sander Alsema

Keys: J D

Downloaded from [basic 10liners 2017 for C64](http://gkanold.wixsite.com/homeputerium/kopie-von-games-list-2018-work) and [Dropbox](https://www.dropbox.com/sh/uufbtdvy6ipqds3/AAAaM_yW_Ifyk_EqFkeNtLDCa/C64)


### 9. Egg Collector

by Sander Alsema

Keys: , .

Downloaded from [basic 10liners 2017 for C64](http://gkanold.wixsite.com/homeputerium/kopie-von-games-list-2018-work) and [Dropbox](https://www.dropbox.com/sh/uufbtdvy6ipqds3/AAAaM_yW_Ifyk_EqFkeNtLDCa/C64)


### 10. EasterShip

by Logiker

Keys: none, just watch demo


### 11. Hello World

just to fill empty slots.

