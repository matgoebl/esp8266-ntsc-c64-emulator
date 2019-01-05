Last Outpost by 5ace
--------------------

"PUR-80", Commodore64, Microsoft BASIC V2


Story:
------

In a distant future, humans were attacked and almost exterminated by an alien race.
On Planet X there are only a handful of survivors left.
You were chosen as commander of the last outpost to secure the survival of the rest of humanity.
Are you in a position to navigate through your skill and your ability to react the protective screen and thus avert the extraterrestrial threat?

Humanity is counting on you!



Instructions:
-------------

Use cursor left and cursor right to navigate the protective shield.

Every successfully repelled shot adds a score of 100 points.
After a random number of attacks your level and so the speed of the attack is increased.
You win if you survive level 10.
You loose if you miss a shot.

Space restarts the game then.




How to start this game in the emulator:
---------------------------------------
1.) Download "vice" emulator from http://vice-emu.sourceforge.net/
2.) Execute "vice" and Drag and Drop "last-outpost.prg" into the "vice" window.
This will autostart the game.

Alternatively select "File"->"Autostart Disk/Tape Image" from the menu.
Select the file "last-outpost.prg" and click on attach.




Code:
-------------

0 a0=rnd(-ti):x=20:y=16:s=-100:l=1:a=781:b=782:c=783:d=65520:v=53280:pokev,6
1 g$=" {cyan}{cm @}F{sh asterisk}-{sh asterisk}F{cm @} ":x$=" {down}{left}{yellow}:":print"{clear}":a$="{236}{234}":b$="{236}{160}":c$="{236}{251}{160}{234}":d$="{down}{left*10}"
2 POKEa,19:POKEb,15:POKEc,0:SYSd:print"{light gray}{reverse on}"a$"{down}{left}{left}"b$"{right}{right}{reverse off}{cm p}{cm i}{cm i}{cm f}{reverse on}{236}{246}"d$b$a$b$a$"{236}{231}"d$c$c$b$d$"{up}{up}"
3 p=int(rnd(1)*38+1):q=1:s=s+100:l=l+int(rnd(1)*2):t=0:tm=int((10-l)/2):on-(l=11)goto8:
4 k=15:si=54272:pokesi+24,k:pokesi+3,k:pokesi+2,k:gosub9:print"{white}{home}score: ",s,"level: ",l
5 t=t+1:ift>tmthenq=q+1:t=0:pokea,q:pokeb,p:pokec,0:sysd:printx$:on-(q=15andx+8>pandx<p)goto3:on-(q>y)goto7
6 pokea,y:pokeb,x:pokec,0:sysd:printg$:geta$:x=x+(a$="{left}"andx>0)+((a$="{right}"andx<31)*-1):goto5
7 print"{home}{down}{down}{red} game over! hit space to retry":geta$:on-(a$=" ")goto0:pokev,(peek(v)+1)andk:goto7
8 print"{home}{down}{down}{green} congratulation you won! hit space":geta$:on-(a$=" ")goto0:pokev,(peek(v)+1)andk:goto8
9 pokesi+5,75:pokesi+6,k:pokesi+4,0:pokesi+1,27-tm:pokesi,103:pokesi+4,129:return


Code-Description:
-------------
line 0-2: intialize variables and draw background

line 3: add a score of 100 and potentially increase level. if level 10 (l=11) is over goto the congratulations screen (line 8)

line 4: intitalize some of the sid-soundchip registers. Jump to Subroutine in line 9 to play a sound. print score and level on screen.

line 5: draws the bomb and checks if it caught (jump to line 3 "increase level") or it is missed (if so jump to line 7 "game over")

line 6: Read the keyboard, moves the shield between the boundaries. then goes back to line 5

line 7: displays game over and waits for space to jump to line 0.

line 8: displays "you have won!" and waits for space to jump to line 0.

line 9: Subroutine: Plays a sound and returns
