%% Diff -> SS Matrix Shit
clear;
clc;
syms M m l b c J I r g s t;

M_1 = [J+(M+m)*r^2+M*l*r, -(J+(M+m)*r^2);
       -(J+(M+m)*r^2+2*M*l*r +M*l^2+I), J+(M+m)*r^2+M*l*r]
m_1 = (J+(M+m)*r^2+M*l*r)^2-(J+(M+m)*r^2)*(J+(M+m)*r^2 + 2*M*l*r+M*l^2 + I);
Bdiff = [0 0; -b -c];
Cdiff = [0 0;M*g*l 0];
Ddiff = [1;0];

Bss = M_1*Bdiff
Css = M_1*Cdiff
Dss = M_1*Ddiff
m_1 = collect(expand(m_1))