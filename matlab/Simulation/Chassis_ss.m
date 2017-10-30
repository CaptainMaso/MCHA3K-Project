%% Model Parameters
% Chassis Parameters
M = 552e-3;              % Mass of chassis (kg)
m = 452e-3;          	% Mass of wheels + motors (kg)
l = 95e-3;            % Distance from wheel to CoG (m)
b = 0;%0.001;          % Damping on chassis (N.s/m)
c = 0;%0.005;          % Damping between wheel and chassis (N.m.s/rad)

I = 0.007358820323616 - M*l^2;   % Chassis Moment of Inertia
r = (81.16*10^-3)/2;       % Wheel Radius
ML_J = pi/32 * (r^4-(51.2e-3)^4);     % Wheel Moment of Inertia
MR_J = ML_J;
J = ML_J + MR_J;

%% Chassis SS Formulation
Mdiff = [J+(M+m)*r^2 + 2*M*l*r + M*l^2 + I,   J+(M+m)*r^2 + M*l*r;
         J+(M+m)*r^2 + M*l*r,                 J+(M+m)*r^2];
     
Bdiff = [b c; 0 0];
Cdiff = [-M*g*l, 0; 0, 0];
Udiff = [0;1];

Bss = -Mdiff \ Bdiff;
Css = -Mdiff \ Cdiff;
Uss =  Mdiff \ Udiff;

A_chassis = [0 0 1 0;
     0 0 0 1;
     Css(1,1), Css(1,2), Bss(1,1), Bss(1,2);
     Css(2,1), Css(2,2), Bss(2,1), Bss(2,2)];

B_chassis = [0;0;Uss(1);Uss(2)];

C_chassis = [1 0 0 0; 0 0 r r];

D_chassis = [0;0];

chassis_poles = eig(A_chassis)
chassis = ss(A_chassis, B_chassis, C_chassis, D_chassis);