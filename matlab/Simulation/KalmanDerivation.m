clc; clear;
syms kw kt kb rw rt T Pww Pwt Pwb Ptt Ptb Pbb real;
Gcorr = [1-kw, 0, -kw; -kt, 1, -kt; -kb, 0, 1-kb]*[Pww, Pwt, Pwb ; Pwt , Ptt,Ptb;Pwb, Ptb, Pbb]
Pww_eq = Gcorr(1,1)
Pwt_eq = Gcorr(1,2)
Pwb_eq = Gcorr(1,3)
Ptt_eq = Gcorr(2,2)
Ptb_eq = Gcorr(2,3)
Pbb_ew = Gcorr(3,3)

%%
k = [kw; kt; kb];
c = [0,1,0];
kc = k*c;
Ikc = eye(3)-kc;

Acorr = Ikc*[Pww, Pwt, Pwb ; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb]

Pww_eq = Acorr(1,1)
Pwt_eq = Acorr(1,2)
Pwb_eq = Acorr(1,3)
Ptt_eq = Acorr(2,2)
Ptb_eq = Acorr(2,3)
Pbb_ew = Acorr(3,3)