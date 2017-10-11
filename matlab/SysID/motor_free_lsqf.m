function [R, L, K] = motor_free_lsqf(V, i, di, w, dw)

global GearRatio;

%% KVL equation LSQR
% V = R*i + L *di + K*N*w
% di = (V - R*i -K*N*w)/L

Y = di;
Phi = [V, -i, -GearRatio*w];
% Theta = [1/L; R/L; K/L]

Theta = lsqr(Phi, Y);
%R = Theta(1);
%L = Theta(2);
%K = Theta(3);
L = 1/Theta(1);
R = Theta(2)*L;
K = Theta(3)*L;

%% SUM OF TORQUES LSQR
% dw = (i*K*N*Neta - N*Neta*Tm)/J

Y = dw;
Phi = [K*N*i, -N];
% Theta = [Neta/J; Neta*TM/J]

