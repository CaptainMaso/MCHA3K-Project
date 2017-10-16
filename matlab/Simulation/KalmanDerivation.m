syms kw kt kb rw rt T Pww Pwt Pwb Ptt Ptb Pbb real;

GyroCorrection = [1-kw, 0, -kw;-kt, 1, -kt;-kb, 0, 1-kb]*[Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb] * ([1-kw, 0, -kw;-kt, 1, -kt;-kb, 0, 1-kb]') + 1/T*rw*[kw^2, kw*kt, kw*kb; kw*kt, kt^2, kt*kb; kw*kb, kt*kb, kb^2]
GyroCorrection = simplify(GyroCorrection);
GyroCorrection = collect(GyroCorrection, [Pww, Pwt, Pwb, Ptt, Ptb, Pbb]) 
GyroCorrection(3,1) - GyroCorrection(1,3)
GyroCorrection(2,1) - GyroCorrection(1,2)
GyroCorrection(3,2) - GyroCorrection(2,3)

Pww_eq = GyroCorrection(1,1)
Pwt_eq = GyroCorrection(1,2)
Pwb_eq = GyroCorrection(1,3)
Ptt_eq = GyroCorrection(2,2)
Ptb_eq = GyroCorrection(2,3)
Pbb_ew = GyroCorrection(3,3)

%%

AccCorrection = [1, -kw, 0;0, 1-kt, 0; -kb, 0, 1-kb]*[Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb] * ([1, -kw, 0;0, 1-kt, 0; -kb, 0, 1-kb]') + 1/T*rt*[kw^2, kw*kt, kw*kb; kw*kt, kt^2, kt*kb; kw*kb, kt*kb, kb^2]
AccCorrection = simplify(AccCorrection);
AccCorrection = collect(AccCorrection, [Pww, Pwt, Pwb, Ptt, Ptb, Pbb]) 
AccCorrection(3,1) - AccCorrection(1,3)
AccCorrection(2,1) - AccCorrection(1,2)
AccCorrection(3,2) - AccCorrection(2,3)

Pww_eq = AccCorrection(1,1)
Pwt_eq = AccCorrection(1,2)
Pwb_eq = AccCorrection(1,3)
Ptt_eq = AccCorrection(2,2)
Ptb_eq = AccCorrection(2,3)
Pbb_ew = AccCorrection(3,3)