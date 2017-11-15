%% Params
global ADC_GAIN ADC_MAX ADC_VREF ADC_RESISTOR;
CPR = 660;
LOWDATA = 100;
HIGHDATA = 400;

T = 1/100;

%%
ML_R = 6.2;
ML_N = 30;

[~,Encoder,ADC] = load_freerun('Motor - Free Run/LEFT_FORWARD.csv', LOWDATA, HIGHDATA);

phi = abs(Encoder/CPR * 2 * pi);

[~, DTHETA, ~] = quadraticSavitzkyGolay(phi',T,17);  
DTHETA = DTHETA(17+1:end-17)';
CURRENT = abs(ADC(17+1:end-17));

w = mean(DTHETA);
i = mean(CURRENT);

ML_K_F = (8-ML_R*i)/(ML_N*w);
ML_Tm_pos = i*ML_K_F;

%%
[~,Encoder,ADC] = load_freerun('Motor - Free Run/LEFT_BACKWARD.csv', LOWDATA, HIGHDATA);

phi = abs(Encoder/CPR * 2 * pi);

[~, DTHETA, ~] = quadraticSavitzkyGolay(phi',T,17);  
DTHETA = DTHETA(17+1:end-17)';
CURRENT = abs(ADC(17+1:end-17));

w = mean(DTHETA);
i = mean(CURRENT);

ML_K_B = (8-ML_R*i)/(ML_N*w);

ML_Tm_neg = i*ML_K_B;

ML_K = (ML_K_F+ML_K_B)/2;

%%
LOWDATA = 100;
HIGHDATA = 400;

data = {'Motor - Load Run/L_8V_50g_FORWARD.csv', 'Motor - Load Run/L_8V_100g_FORWARD.csv', 'Motor - Load Run/L_8V_150g_FORWARD.csv','Motor - Load Run/L_8V_200g_FORWARD.csv', 'Motor - Load Run/L_8V_250g_FORWARD.csv', 'Motor - Load Run/L_8V_300g_FORWARD.csv'};
m = [50, 100, 150, 200, 250, 300]*10^-3;

for j = 1:6
    [~, ~,~,~,ADC] = import_motor_load(data{j}, LOWDATA, HIGHDATA);
    To = m(j)*9.81*(25.5e-3);
    i = mean(abs(ADC/10));
    Eta(j) = To/(ML_N*(ML_K*i + ML_Tm_pos));
end

ML_Eta_f = mean(Eta);

%%
LOWDATA = 100;
HIGHDATA = 400;

data = {'Motor - Load Run/L_8V_50g_BACK.csv', 'Motor - Load Run/L_8V_100g_BACK.csv', 'Motor - Load Run/L_8V_150g_BACK.csv','Motor - Load Run/L_8V_200g_BACK.csv', 'Motor - Load Run/L_8V_250g_BACK.csv', 'Motor - Load Run/L_8V_300g_BACK.csv'};
m = [50, 100, 150, 200, 250, 300]*10^-3;

for j = 1:6
    [~, ~,~,~,ADC] = import_motor_load(data{j}, LOWDATA, HIGHDATA);
    To = m(j)*9.81*(25.5e-3);
    i = mean(abs(ADC/10));
    Eta(j) = To/(ML_N*(ML_K*i + ML_Tm_neg));
end

ML_Eta_b = mean(Eta);
ML_Eta = (ML_Eta_f+ML_Eta_b)/2;

%%
MR_R = 6.7;
MR_N = 30;

[~,Encoder,ADC] = load_freerun('Motor - Free Run/RIGHT_FORWARD.csv', LOWDATA, HIGHDATA);

phi = abs(Encoder/CPR * 2 * pi);

[~, DTHETA, ~] = quadraticSavitzkyGolay(phi',T,17);  
DTHETA = DTHETA(17+1:end-17)';
CURRENT = abs(ADC(17+1:end-17));

w = mean(DTHETA);
i = mean(CURRENT);

MR_K_F = (8-MR_R*i)/(MR_N*w);
MR_Tm_pos = i*MR_K_F;

%%
[~,Encoder,ADC] = load_freerun('Motor - Free Run/RIGHT_BACKWARD.csv', LOWDATA, HIGHDATA);

phi = abs(Encoder/CPR * 2 * pi);

[~, DTHETA, ~] = quadraticSavitzkyGolay(phi',T,17);  
DTHETA = DTHETA(17+1:end-17)';
CURRENT = abs(ADC(17+1:end-17));

w = mean(DTHETA);
i = mean(CURRENT);

MR_K_B = (8-MR_R*i)/(MR_N*w);

MR_Tm_neg = i*MR_K_B;

MR_K = (MR_K_F+MR_K_B)/2;

%%
LOWDATA = 100;
HIGHDATA = 400;

data = {'Motor - Load Run/R_8V_50g_FORWARD.csv', 'Motor - Load Run/R_8V_100g_FORWARD.csv', 'Motor - Load Run/R_8V_150g_FORWARD.csv','Motor - Load Run/R_8V_200g_FORWARD.csv', 'Motor - Load Run/R_8V_250g_FORWARD.csv', 'Motor - Load Run/R_8V_300g_FORWARD.csv'};
m = [50, 100, 150, 200, 250, 300]*10^-3;

for j = 1:6
    [~, ~,~,~,ADC] = import_motor_load(data{j}, LOWDATA, HIGHDATA);
    To = m(j)*9.81*(25.5e-3);
    i = mean(abs(ADC/10));
    Eta(j) = To/(MR_N*(MR_K*i + MR_Tm_pos));
end

MR_Eta_f = mean(Eta);

%%
LOWDATA = 100;
HIGHDATA = 400;

data = {'Motor - Load Run/R_8V_50g_BACK.csv', 'Motor - Load Run/R_8V_100g_BACK.csv', 'Motor - Load Run/R_8V_150g_BACK.csv','Motor - Load Run/R_8V_200g_BACK.csv', 'Motor - Load Run/R_8V_250g_BACK.csv', 'Motor - Load Run/R_8V_300g_BACK.csv'};
m = [50, 100, 150, 200, 250, 300]*10^-3;

for j = 1:6
    [~, ~,~,~,ADC] = import_motor_load(data{j}, LOWDATA, HIGHDATA);
    To = m(j)*9.81*(25.5e-3);
    i = mean(abs(ADC/10));
    Eta(j) = To/(MR_N*(MR_K*i + MR_Tm_neg));
end

MR_Eta_b = mean(Eta);

MR_Eta = (MR_Eta_f+MR_Eta_b)/2;
%%
ML_R
ML_K
ML_N
ML_Eta
ML_Tm_pos
ML_Tm_neg

MR_R
MR_K
MR_N
MR_Eta
MR_Tm_pos
MR_Tm_neg






