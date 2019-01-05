BAS=hello-world.bas 10print.bas
#BAS=hello-world.bas mini-tetris_v2.bas print10.bas
#PRG=Compo2017.prg GorillaX_12.prg last-outpost.prg tetrispd.prg mazo.prg bitcoinminer.prg

all:	$(BAS) $(PRG)
	n=1; for f in $^; do \
		echo $$n: $$f; \
		echo make $^.h; \
		echo cp $$f preloaded_$$n.$(suffix $f); \
		echo make preloaded_$$n.h; \
		n=`expr $$n + 1`; \
	done


%.h: %.prg
	xxd -i $< | sed -e 's/unsigned char/const PROGMEM unsigned char/' > $@

%.prg: %.bas
	petcat -w2 -o $@ $<

clean:
	rm -f *.h preloaded_* #.prg
