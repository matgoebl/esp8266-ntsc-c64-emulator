1 a$="efijefijefijefijbfjnhijkbfjnhijkijfgaefjijfgaefjefjkiefbefjkiefbbfjidefj" 
2 a$=a$+"abeieijkaeijijkgabfjiefgehijebfj@abe@dhe":o=207:dime(o):forx=0to50
3 print,"   {CBM-M}","    {CBM-G}":p=asc(mid$(a$,x+1)):e(x)=(pand3)+(pand12)*10:next:m=2024 
4 print,"    {CBM-T}{CBM-T}{CBM-T}{CBM-T}{CBM-T}{CBM-T}{CBM-T}{CBM-T}{CBM-T}{CBM-T}":gosub6:goto7 
5 pokei+e(r),c:pokei+e(r+1),c:pokei+e(r+2),c:pokei+e(r+3),c:c=160:r$="d":return 
6 i=1152:r=h:c=32:gosub5:j=int(rnd(1)*7)*16:r=j:gosub5:r=h:h=j:i=i+9:return 
7 gosub6:w=i:t=i:g=r:k=240:l=1278 
8 gosub15:c=32:gosub5:r=-r*b-g*notb:g=r 
9 i=w:w=i+40:gosub5:gosub15:ifbthen12 
10 getk$:g=randkor((r-4*(k$="s"))and15) 
11 t=w:w=w+(k$=l$)-(k$=r$):l=l+40:goto8 
12 c=o:gosub5:m=m-(l<m)*(l-m):fory=0to3:q=1:forx=0to9:q=q*(peek(l+x)=o):next 
13 fors=ltol+(m-160-l)*qstep-1:pokes+11,peek(s-29):next:y=y-q:m=m+q*40:z=z+q 
14 l=l+40*not-q:next:print"{home}";z:goto7 
15 b=0:forx=0to3:b=bor1andpeek(w+e(g+x)):next:b=b>0:w=-t*b-w*notb:l$="a":return
