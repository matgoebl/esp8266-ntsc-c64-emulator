#!/bin/bash

[ $# = 0 ] && set \
 hello-world.bas \
 10print.bas \
 hello-world.bas \
 hello-world.bas \
 hello-world.bas \
 hello-world.bas \
 hello-world.bas \
 hello-world.bas \
 hello-world.bas \
 hello-world.bas \
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
