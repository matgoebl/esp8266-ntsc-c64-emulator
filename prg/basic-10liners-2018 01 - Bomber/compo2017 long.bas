100 ?"{GREY 1}{CLR}"
    POKE53281,3
    FORI=0TO39
      POKE1984+I,160
      POKE56256+I,5
      H=INT(3+RND(1)*6)
200   FORJ=1TOH
        POKE1984+I-J*40,102
    NEXTJ,I
    B$="{LEFT}{LEFT}{LEFT} {GREY 2}{RVSON}{CBM-*}{RVSOFF}{CBM-I}{BLUE}{CBM-P}"
    B=0
    ?"{HOME}{DOWN}{DOWN}{RIGHT}{RIGHT}";B$;
    X=1104
300 S=54272
    POKES+24,15
    POKES,50
    POKES+1,2
    POKES+5,0
    POKES+6,255
    POKES+4,129
400 POKES+12,0
    POKES+13,255
    POKES+7,0
500 GETA$
    IFB=0ANDA$=" "THEN
      B=1
      Y=X
      POKES+8,200
      POKES+11,33
600 IFBTHEN
      POKEY,32
      B=1+(Y>1943)
      POKES+11,B*33
      IFBTHEN
        Y=Y+40
        POKEY,83
        POKES+8,255-Y/20
700 F=1-F
    ON-(F=1)GOTO500
800 X=X+1
    ?B$;
    IFPEEK(X+3)<>32THEN
      POKES+1,10
      POKES+11,0
      FORI=0TO254
        POKE53280,I
      NEXT
      RUN
900 ON2+(X<1978)GOTO500
    ?"{CLR}"SPC(7)"CONGRATULATIONS! YOU WON!!"


