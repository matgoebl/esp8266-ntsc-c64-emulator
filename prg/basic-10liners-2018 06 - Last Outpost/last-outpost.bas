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