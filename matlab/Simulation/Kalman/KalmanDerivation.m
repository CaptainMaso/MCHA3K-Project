% clc; clear;
% syms kw kt kb rw rt T Pww Pwt Pwb Ptt Ptb Pbb real;
% Gcorr = [1-kw, 0, -kw; -kt, 1, -kt; -kb, 0, 1-kb]*[Pww, Pwt, Pwb ; Pwt , Ptt,Ptb;Pwb, Ptb, Pbb]
% Pww_eq = Gcorr(1,1)
% Pwt_eq = Gcorr(1,2)
% Pwb_eq = Gcorr(1,3)
% Ptt_eq = Gcorr(2,2)
% Ptb_eq = Gcorr(2,3)
% Pbb_ew = Gcorr(3,3)
% 
% %%
% k = [kw; kt; kb];
% c = [0,1,0];
% kc = k*c;
% Ikc = eye(3)-kc;
% 
% Acorr = Ikc*[Pww, Pwt, Pwb ; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb]
% 
% Pww_eq = Acorr(1,1)
% Pwt_eq = Acorr(1,2)
% Pwb_eq = Acorr(1,3)
% Ptt_eq = Acorr(2,2)
% Ptb_eq = Acorr(2,3)
% Pbb_ew = Acorr(3,3)
% 
% %%
syms ks kp T Pss Psp Ppp qs rp t real;

% X = [phi, dphi (psi)]
A = [0 1; 0 0];
C = [1 0];
Ad = expm(A*T)
w = [0; 1];
Q = (w*w')*qs;
Qd = int(expm(A*t)*Q*expm(A'*t), t, 0, T)

v = [1;0];
R = (v*v')*rp;
Rd = R/T

k = [kp; ks];
P = [Ppp, Psp; Psp, Pss];

Pupdate = Ad*P*Ad'+Qd

K = (P*C')/(C*P*C'+1/T*rp)
Pcorr = (eye(2) - k*C)*P