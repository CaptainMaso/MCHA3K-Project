clear;clc;
%% Model Parameters
% Chassis Parameters
M = 800e-3;              % Mass of chassis (kg)
m = 0.08;          	% Mass of wheel (kg)
l = 0.1;            % Distance from wheel to CoG (m)
b = 0.001;          % Damping on chassis (N.s/m)
c = 0.005;          % Damping between wheel and chassis (N.m.s/rad)
J = 3.22*10^-6;     % Wheel Moment of Inertia
I = 78.125*10^-6;   % Chassis Moment of Inertia
r = 45*10^-3;       % Wheel Radius

% DC Motor Parameters
Vmax = 12;      % Maximum voltage (V)
R = 0.658;      % Armature Resistance (Ohms)
L = 0.001;       % Inductance of DC Motor (H)
K = 0.014;      % Motor Constant (N.m/A)
N = 50;         % Gearbox Ratio
Neta = 0.7;%0.588;   % Gearbox Efficiency (%/100)
Tm = 5.6e-3;    % Friction Torque (N.m)

% Physical World Parameters
g = 9.81;           % Acceleration due to gravity (m/s/s)
alpha = 0*pi/180;   % Ground slope (rad = deg*pi/180)

% Initial Conditions & other sim params
theta0 = 10 * pi/180;    % Initial Theta

tsim = 40;              % Simulation Time (s)
vref_step_point = 0.5;  % Set point of vref (m/s)
inpdist_step_point = 0.05;

%% SS Formulation
Mdiff = [J+(M+m)*r^2 + M*l*r*cos(alpha), J+(M+m)*r^2;
         J+(M+m)*r^2 + 2*M*l*r*cos(alpha) + M*l^2 + I, J+(M+m)*r^2 + M*l*r*cos(alpha)];
     
Bdiff = [0 0; b c];
Cdiff = [0, 0; -M*g*l, 0];
Udiff = [1;0];

Bss = -Mdiff \ Bdiff;
Css = -Mdiff \ Cdiff;
Uss =  Mdiff \ Udiff;

A = [0 0 1 0;
     0 0 0 1;
     Css(1,1), Css(1,2), Bss(1,1), Bss(1,2);
     Css(2,1), Css(2,2), Bss(2,1), Bss(2,2)];

B = [0;0;Uss(1);Uss(2)];

C = [0 0 1 0; 0 1 0 0];

D = [0;0];

poles = eig(A)

%% Controllability
% State Feedback Design
Ac = A([1,3:4], [1,3:4])
Bc = B([1, 3:4])
Cc = [0 1 0; 0 r r];
Dc = [0;0];

states = {'theta', 'theta_dot', 'phi_dot'};
inputs = {'tau'};
outputs = {'theta', 'velocity'};

sys_ss = ss(Ac,Bc,Cc,Dc, 'statename', states, 'inputname', inputs, 'outputname', outputs);

poles = eig(Ac)

co = ctrb(sys_ss);
controllability = rank(co)

Q = Cc'*Cc
%Q(1,1) = Q(1,1)*500;

R = 1;

Kc = lqr(Ac, Bc, Q, R)

Acl = (Ac-Bc*Kc);
Bcl = Bc;
Ccl = Cc;
Dcl = Dc;

FVT = dcgain(ss(Acl, Bcl, Ccl, Dcl, 'statename', states, 'inputname', inputs, 'outputname', outputs));
N = 1/FVT(2);

% State Observer Design
ob = obsv(sys_ss);
observability = rank(ob)

poles_cl = eig(Ac);

P = [-20, -21, -22, -23];
Lobs = place(A', C', P)';

%% Simulation
sim('model_sf');

%% Plot Results
h = gcf;
clf(gcf);
hold on;
subplot(3,2,1);
plot(time, theta*180/pi, 'b-', time, theta_obs*180/pi, 'g--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('\theta (\circ)', 'FontSize', 14);
legend('Actual', 'Observer');
grid on;

subplot(3,2,2);
plot(time, thetadot*180/pi, time, thetadot_obs*180/pi, 'g--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('d\theta/dt (\circ)', 'FontSize', 14);
legend('Actual', 'Observer');
grid on;

subplot(3,2,3);
plot(time, (phi + theta) * r, time, (phi_obs + theta_obs) * r, 'g--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('x (m)', 'FontSize', 14);
legend('Actual', 'Observer');
grid on;

subplot(3,2,4);
plot(time, (phidot + thetadot) .* r, 'b-', time, vref, 'r--', time, (phidot_obs + thetadot_obs) * r, 'g--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('dx/dt (m/s)', 'FontSize', 14);
legend('Actual', 'Demanded', 'Observer');
grid on;

subplot(3,2,5:6);
plot(time, Voltage);
xlabel('Time (s)', 'FontSize', 11);
ylabel('Control Voltage (V)', 'FontSize', 14);
grid on;

h.NextPlot = 'add';
a = axes;
ht = title(['IC: \theta = ' num2str(theta0*180/pi) '\circ'], 'FontSize', 16);
a.Visible = 'off';
ht.Visible = 'on';
hold off;