% Programmable Parameters
T_motor = 1/100;
T_pend = 1/100;

T_main = 1/lcm(1/T_pend, 1/T_motor);

T_kfcorr = 1/100;

CPR = 660;
Vmax = 12;

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

Chassis_Q = Chassis_Cc.'*Chassis_Cc;

Chassis_R = 2;

Chassis_Kc = lqrd(Chassis_Ac, Chassis_Bc, Chassis_Q, Chassis_R, T_pend);

K_Theta = Chassis_Kc(1);
K_dTheta = Chassis_Kc(2);
K_dPhi = Chassis_Kc(3);
K_Error = 1;

Chassis_Acl = (Chassis_Ac-Chassis_Bc*Chassis_Kc);
Chassis_Bcl = Chassis_Bc;
Chassis_Ccl = Chassis_Cc;
Chassis_Dcl = Chassis_Dc;

% States = integrator
% Inputs = [Theta, dTheta, dPhi, Vref]
% Outputs = [Torque]

% Only 1 integrator
Chassis_ctrl_A = 0;

% Error = vref - r*dTheta - r*dPhi
Chassis_ctrl_B = [0, -r, -r, 1];

Chassis_ctrl_C = -K_Error;

Chassis_ctrl_D = [-K_Theta, -K_dTheta, -K_dPhi, 0];


Chassis_ctrl_ss = ss(Chassis_ctrl_A, Chassis_ctrl_B, Chassis_ctrl_C, Chassis_ctrl_D);
Chassis_ctrl_dss = c2d(Chassis_ctrl_ss, T_pend, 'zoh');

%% DC motor control
% Motor Left Parameters
ML_R       = 2.95;        % Armature Resistance (Ohms)
ML_L       = 2.3e-3;           % Inductance of DC Motor (H)
ML_K       = 0.0109;        % Motor Constant (N.m/A)
ML_N       = 30;            % Gearbox Ratio
ML_Eta    = 0.7;  %0.588;   % Gearbox Efficiency (%/100)
ML_Tm_pos      = 1.5e-3;      % Friction Torque w+ (N.m)
ML_Tm_neg      = -1.5e-3;      % Friction Torque w- (N.m)

% Motor Right Parameters
MR_R       = 2.95;        % Armature Resistance (Ohms)
MR_L       = 2.3e-3;           % Inductance of DC Motor (H)
MR_K       = 0.015;        % Motor Constant (N.m/A)
MR_N       = 30;            % Gearbox Ratio
MR_Eta    = 0.7;            % Gearbox Efficiency (%/100)
MR_Tm_pos      = 1.5e-3;      % Friction Torque w+ (N.m)
MR_Tm_neg      = -1.5e-3;      % Friction Torque w- (N.m)

% Motor Left
ML_Tau = ML_L/ML_R*1.5;
ML_Kp = 2*ML_L/ML_Tau - ML_R;
ML_Ti = ML_L/ML_Tau^2;

ML_ctrl = tf(-ML_Ti, [ML_Kp^2, ML_Kp*ML_Ti]);
ML_ctrl_ss = ss(ML_ctrl);
ML_ctrl_dss = c2d(ML_ctrl_ss, T_motor);

ML_T2C = 1/(ML_K*ML_N*ML_Eta);
ML_HFG = ML_Kp;
ML_OFFS_POS = ML_Tm_pos/ML_K;
ML_OFFS_NEG = ML_Tm_neg/ML_K;

ML_PARAM = struct('T2C', ML_T2C, 'HFG', ML_HFG, 'OFFS_POS', ML_OFFS_POS, 'OFFS_NEG', ML_OFFS_NEG, 'R', ML_R, 'K', ML_K, 'N', ML_N);

% Motor Right
MR_Tau = MR_L/MR_R*1.5;
MR_Kp = 2*MR_L/MR_Tau - MR_R;
MR_Ti = MR_L/MR_Tau^2;

MR_ctrl = tf(-MR_Ti, [MR_Kp^2, MR_Kp*MR_Ti]);
MR_ctrl_ss = ss(MR_ctrl);
MR_ctrl_dss = c2d(MR_ctrl_ss, T_motor);

MR_T2C = 1/(MR_K*MR_N*MR_Eta);
MR_HFG = MR_Kp;
MR_OFFS_POS = MR_Tm_pos/MR_K;
MR_OFFS_NEG = MR_Tm_neg/MR_K;

MR_PARAM = struct('T2C', MR_T2C, 'HFG', MR_HFG, 'OFFS_POS', MR_OFFS_POS, 'OFFS_NEG', MR_OFFS_NEG, 'R', MR_R, 'K', MR_K, 'N', MR_N);
K_BITSHIFT = 15;
ExportController(T_main, T_kfcorr, K_BITSHIFT, Chassis_ctrl_dss, ML_ctrl_dss, ML_PARAM, MR_ctrl_dss, MR_PARAM);
KalmanExport(T_kfcorr, T_motor, K_BITSHIFT);