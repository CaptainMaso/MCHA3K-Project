clear;
clc;
format long;
%% Parameters
global CPR M l g T N;
CPR = 4096;
M = 552e-3;
l = 95e-3;
g = 9.81;
T = 1/200;
N = 5;

LOWDATA = 3000;
HIGHDATA = 6000;

data = ['Swing Tests/1.csv';'Swing Tests/2.csv'; 'Swing Tests/3.csv'; 'Swing Tests/4.csv'; 'Swing Tests/5.csv'];

t = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
theta = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
dtheta = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));
ddtheta = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));

%% Load Data into array
for i = 1:size(data,1)
    %% Get Data
    [t_tmp, theta_tmp] = ImportSwingTest(data(i,:), LOWDATA, HIGHDATA);
    
    theta_tmp = theta_tmp/CPR * 2*pi;
    t_tmp = t_tmp-t_tmp(1);
    
    [~,dtheta_tmp, ddtheta_tmp] = quadraticSavitzkyGolay(theta_tmp',T,N);  
    t(:, i) = t_tmp(N+1:end-N);
    theta(:, i) = theta_tmp(N+1:end-N);
    dtheta(:, i) = dtheta_tmp(N+1:end-N)';
    ddtheta(:, i) = ddtheta_tmp(N+1:end-N)';
end

%% Get Initial Guess
for i = 1:size(data,1)
    [Itmp(i), ctmp(i)] = pend_lsqf(theta(:, i), dtheta(:, i), ddtheta(:, i));
end

I0 = harmmean(Itmp)
c0 = harmmean(ctmp)

lsqf_cost = pend_cost(I0, c0, t,theta,dtheta)

%% Minimising Error Function
minfunc = @(x) pend_cost(x(1), x(2), t, theta, dtheta);
[res, cost] = fminunc(minfunc, [I0 c0]);
I = res(1)
c = res(2)

%% Plotting
for i = 1:size(data,1)    
	variablecost  = [i, pend_cost(I, c, t(:, i), theta(:, i), dtheta(:, i))]
    pend_plot(I, c, t(:, i), theta(:, i), dtheta(:, i));
end