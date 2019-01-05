#!/bin/bash

[ $# = 0 ] && set \
 10print.bas \
 "MiniTetris/mini-tetris.bas" \
 "basic-10liners-2016 01SNAKE3/01SNAKE3.prg" \
 "basic-10liners-2017 06 - GorillaX by Naufr4g0/GorillaX_12.prg" \
 "basic-10liners-2017 52 - Beerhunter by Sander Alsema/beerhunter.prg" \
 "basic-10liners-2018 01 - Bomber/Compo2017.prg" \
 "basic-10liners-2018 06 - Last Outpost/last-outpost.prg" \
 'basic-10liners-2018 11 - Duck!!!/duck.prg' \
 'basic-10liners-2018 28 - Egg Collector/egg_collector.prg' \
 'basic-10liners-2018 53 - EasterShip 2018/EasterShip 2018.prg' \
 hello-world.bas \
 hello-world.bas \

n=1
for f in "$@"; do
 echo "$n. $f"
 prg="builtinprg_$n.prg"
 code="../ESP8266-NTSC-C64/builtinprg_$n.h"
 case "$f" in
  *.bas) petcat -w2 -o "$prg" "$f" ;;
  *.prg) cp "$f" "$prg" ;;
  *) echo "cannot convert $f"; exit 1 ;;
 esac
 xxd -i "$prg" | sed -e 's/unsigned char/const PROGMEM unsigned char/' > "$code"
 n=`expr "$n" + 1`
done

rm -f builtinprg_*
