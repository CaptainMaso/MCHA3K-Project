clear;clc;
%% Model Parameters
% Chassis Parameters
M = 552e-3;              % Mass of chassis (kg)
m = 452e-3;          	% Mass of wheel (kg)
l = 95e-3;            % Distance from wheel to CoG (m)
b = 0;%0.001;          % Damping on chassis (N.s/m)
c = 0;%0.005;          % Damping between wheel and chassis (N.m.s/rad)
ML_J = 3.22*10^-6;     % Wheel Moment of Inertia
J = ML_J;
I = 0.007361487 - M*l^2;   % Chassis Moment of Inertia
r = 82*10^-3/2;       % Wheel Radius

% DC Motor Parameters
ML_R       = 7.9821;    % Armature Resistance (Ohms)
ML_L       = 0.5973;    % Inductance of DC Motor (H)
ML_K       = 0.0109;   % Motor Constant (N.m/A)
ML_N       = 30;       % Gearbox Ratio
ML_Eta    = 0.4;  %0.588;   % Gearbox Efficiency (%/100)
ML_Tm      = 0.000022;        % Friction Torque (N.m)

% Programmable Parameters
T_motor = 1/1000;
T_pend = 1/100;

Vmax = 12;
CPR = 660;
ADC_GAIN = 10;
ADC_MAX  = 512;
ADC_VREF = 5;
ADC_RESISTOR = 0.1;
N = 30;

DPS = 500;
Bias = 0.1;
MAX_G = 4;
l_imu = 100e-3;

% Physical World Parameters
g = 9.81;           % Acceleration due to gravity (m/s/s)
alpha = 0*pi/180;   % Ground slope (rad = deg*pi/180)

% Initial Conditions & other sim params
theta0 = 30 * pi/180;    % Initial Theta

tsim = 40;              % Simulation Time (s)
vref_step_point = 1000e-3;  % Set point of vref (m/s)
inpdist_step_point = 0.05;

%% Chassis SS Formulation
Mdiff = [ML_J+(M+m)*r^2 + 2*M*l*r + M*l^2 + I,   ML_J+(M+m)*r^2 + M*l*r;
         ML_J+(M+m)*r^2 + M*l*r,                 ML_J+(M+m)*r^2];
     
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
ss_chassis = ss(A_chassis, B_chassis, C_chassis, D_chassis);
chassis_zeros = tzero(ss_chassis)
tf(ss_chassis)

%% DC Motor SSM
ML_A = -ML_R/ML_L;
ML_B = [1/ML_L];%, -ML_K*ML_N/ML_L];
ML_C = ML_N*ML_Eta*ML_K;
ML_D = 0;

motor_sys = ss(ML_A, ML_B, ML_C, ML_D, 'StateName', 'current', 'OutputName', 'Torque', 'InputName', 'Voltage');

%% Controllability
% State Feedback Design
Chassis_Ac = A_chassis([1,3:4], [1,3:4]);
Chassis_Bc = B_chassis([1, 3:4]);
Chassis_Cc = [1 0 0; 0 r r];
Chassis_Dc = [0;0];

states = {'theta', 'theta_dot', 'phi_dot'};
inputs = {'tau'};
outputs = {'theta', 'velocity'};

chassis_sys = ss(Chassis_Ac,Chassis_Bc,Chassis_Cc,Chassis_Dc, 'statename', states, 'inputname', inputs, 'outputname', outputs);

poles_chassis = eig(Chassis_Ac);

chassis_co = ctrb(chassis_sys);
chassis_controllability = rank(chassis_co);

Chassis_Q = Chassis_Cc'*Chassis_Cc;

Chassis_R = 1.5;

Chassis_Kc = lqr(Chassis_Ac, Chassis_Bc, Chassis_Q, Chassis_R);

K_Theta = Chassis_Kc(1);
K_dTheta = Chassis_Kc(2);
K_dPhi = Chassis_Kc(3);
K_Error = 1;

Chassis_Acl = (Chassis_Ac-Chassis_Bc*Chassis_Kc);
Chassis_Bcl = Chassis_Bc;
Chassis_Ccl = Chassis_Cc;
Chassis_Dcl = Chassis_Dc;

%% DC motor control
ML_Tau = 2e-2;
Kp = 2*ML_L/ML_Tau - ML_R;
Ti = ML_L/ML_Tau^2;

motor_ctrl = tf(-Ti, [Kp^2, Kp*Ti]);
motor_ctrl_ss = ss(motor_ctrl);
motor_ctrl_dss = c2d(motor_ctrl_ss, T_motor);

%% Simulation
sim('model_sf');

%% Plot Results
h = gcf;
clf(gcf);
hold on;
subplot(4,2,1);
plot(time, theta*180/pi, 'b-');
xlabel('Time (s)', 'FontSize', 11);
ylabel('\theta (\circ)', 'FontSize', 14);
legend('Actual');
grid on;

subplot(4,2,3);
plot(time, dtheta*180/pi);
xlabel('Time (s)', 'FontSize', 11);
ylabel('d\theta/dt (\circ)', 'FontSize', 14);
legend('Actual');
grid on;

subplot(4,2,5);
plot(time, (phi + theta) * r);
xlabel('Time (s)', 'FontSize', 11);
ylabel('x (m)', 'FontSize', 14);
legend('Actual');
grid on;

subplot(4,2,7);
plot(time, (dphi + dtheta) .* r, 'b-', time, vref, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('dx/dt (m/s)', 'FontSize', 14);
legend('Actual', 'Demanded');
grid on;

subplot(4,2,2);
plot(time, Torque, 'b-', time, DesiredTorque, 'r--');
xlabel('Time (s)', 'FontSize', 11);
ylabel('Control Torque (N.m)', 'FontSize', 14);
legend('Actual', 'Desired');
grid on;

subplot(4,2,6);
plot(time, Current*10^3, 'r-');
xlabel('Time (s)', 'FontSize', 11);
ylabel('Current (mA)', 'FontSize', 14);
legend('Current');
grid on;

subplot(4,2,4);
plot(Voltage.Time, Voltage.Data, 'b-', ActualVoltage.Time, ActualVoltage.Data, 'g--');
legend('Desired Voltage', 'Actual Voltage');
ylabel('Control Voltage (V)', 'FontSize', 14);
xlabel('Time (s)', 'FontSize', 11);
ylim([-20, 20]);
grid on;

h.NextPlot = 'add';
a = axes;
ht = title(['IC: \theta = ' num2str(theta0*180/pi) '\circ'], 'FontSize', 16);
a.Visible = 'off';
ht.Visible = 'on';
hold off;