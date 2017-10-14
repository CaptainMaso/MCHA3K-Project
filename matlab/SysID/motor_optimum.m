clc; clear;
%% Initial Parameters
global T CPR ADC_GAIN ADC_MAX ADC_VREF ADC_RESISTOR Vmax GearRatio;
T = 1/200;

Vmax = 12;
CPR = 660;
ADC_GAIN = 10;
ADC_MAX  = 512;
ADC_VREF = 5;
ADC_RESISTOR = 0.1;

GearRatio = 30;

%% Free Test
ML_free =  {'Motor - Free Run/LEFT_1.csv'; 'Motor - Free Run/LEFT_2.csv'};
MR_free = {'Motor - Free Run/RIGHT_1.csv'; 'Motor - Free Run/RIGHT_2.csv'};

[ML_R0, ML_L0, ML_K0] = motor_freerun(ML_free);
[MR_R0, MR_L0, MR_K0] = motor_freerun(MR_free);

%% Load Test
ML_load_150g = {'Motor - Load Run/150g/LEFT_3V.csv';'Motor - Load Run/150g/LEFT_4.5V.csv';'Motor - Load Run/150g/LEFT_6V.csv';'Motor - Load Run/150g/LEFT_8V.csv'};
ML_load_250g = {'Motor - Load Run/250g/LEFT_5V.csv';'Motor - Load Run/250g/LEFT_6V.csv';'Motor - Load Run/250g/LEFT_7V.csv';'Motor - Load Run/250g/LEFT_8V.csv'};
ML_load_300g = {'Motor - Load Run/300g/LEFT_5V.csv';'Motor - Load Run/300g/LEFT_6V.csv';'Motor - Load Run/300g/LEFT_8V.csv';'Motor - Load Run/300g/LEFT_10V.csv'};

[ML_Eta_lsqf(1), ML_Tm_lsqf(1)] = motor_loadrun(ML_load_150g, ML_K0, 0.150);
[ML_Eta_lsqf(2), ML_Tm_lsqf(2)] = motor_loadrun(ML_load_250g, ML_K0, 0.250);
[ML_Eta_lsqf(3), ML_Tm_lsqf(3)] = motor_loadrun(ML_load_300g, ML_K0, 0.300)
ML_Eta0 = mean(ML_Eta_lsqf);
ML_Tm0 = mean(ML_Tm_lsqf);

MR_load_150g = {'Motor - Load Run/150g/RIGHT_3V.csv';'Motor - Load Run/150g/RIGHT_4.5V.csv';'Motor - Load Run/150g/RIGHT_6V.csv';'Motor - Load Run/150g/RIGHT_8V.csv'};
MR_load_250g = {'Motor - Load Run/250g/RIGHT_4V.csv';'Motor - Load Run/250g/RIGHT_6V.csv';'Motor - Load Run/250g/RIGHT_8V.csv';'Motor - Load Run/250g/RIGHT_10V.csv'};
MR_load_300g = {'Motor - Load Run/300g/RIGHT_4V.csv';'Motor - Load Run/300g/RIGHT_6V.csv';'Motor - Load Run/300g/RIGHT_8V.csv';'Motor - Load Run/300g/RIGHT_10V.csv'};

[MR_Eta_lsqf(1), MR_Tm_lsqf(1)] = motor_loadrun(MR_load_150g, MR_K0, 0.150);
[MR_Eta_lsqf(2), MR_Tm_lsqf(2)] = motor_loadrun(MR_load_250g, MR_K0, 0.250);
[MR_Eta_lsqf(3), MR_Tm_lsqf(3)] = motor_loadrun(MR_load_300g, MR_K0, 0.300);
MR_Eta0 = mean(MR_Eta_lsqf);
MR_Tm0 = mean(MR_Tm_lsqf);

%% Free Test - Inertia
ML_freeinertia =  ['Motor - Free Run/LEFT_1.csv'; 'Motor - Free Run/LEFT_2.csv'];
MR_freeinertia = ['Motor - Free Run/RIGHT_1.csv'; 'Motor - Free Run/RIGHT_2.csv'];

ML_J0 = motor_freeinertia(ML_freeinertia, ML_K0, ML_Eta0, ML_Tm0);
MR_J0 = motor_freeinertia(MR_freeinertia, MR_K0, MR_Eta0, MR_Tm0);

%% Optimisation and Simulation
ML_R0
ML_L0
ML_K0
ML_Eta0
ML_Tm0
ML_Eta0
ML_J0

[ML_R, ML_L, ML_K, ML_Neta, ML_Tm, ML_J] = minimiseMotorCost(ML_free(1,:), ML_R0, ML_L0, ML_K0, ML_Eta0, ML_J0, ML_Tm0);

MR_R0
MR_L0
MR_K0
MR_Eta0
MR_Tm0
MR_Eta0
MR_J0
