clear;
clc;
%% Import Pend Data

[t_pend,theta_pend] = ImportSwingTest('Swing Test.csv', 100, 1000);

theta_pend = theta_pend/CPR * 2*pi;
theta_pend = theta_pend - theta_pend(end);
t_pend = t_pend-t_pend(1);

% figure;
% hold on;
% plot(t_pend, theta_pend, 'b-');
% legend('Data');
% grid on;

theta_pend = theta_pend(1:(end - 300));
t_pend = t_pend(1:(end-300));

%% Estimating theta.. and theta. for cart
T = 1/200;
N = 23; % 2*N+1 points are used as local neighbourhood for quad fit
[~,dtheta_pend,ddtheta_pend] = quadraticSavitzkyGolay(theta_pend',T,N);

%% Estimating parameters
Phi = [ddtheta_pend((N+1:end-N))', dtheta_pend((N+1:end-N))'];
Y = -M*l*g*sin(theta_pend(N+1:end-N));

Theta = lsqr(Phi, Y);
I = Theta(1)
c = Theta(2)

I = 0.0092;
c = 0.0044;

%% Sim
sim('Q2_2sim')

%% Plot Results

figure;
hold on;
subplot(2,1,1);
plot(t_pend, theta_pend, 'b-', tsim_pend, thetasim_pend(:, 1), 'r--');
legend('Data', 'Sim');
xlim([min([t_pend; tsim_pend]) max([t_pend; tsim_pend])]);
grid on;
subplot(2,1,2);
plot(t_pend, dtheta_pend, 'b-', tsim_pend, dthetasim_pend(:, 1), 'r--');
legend('Data', 'Sim');
xlim([min([t_pend; tsim_pend]) max([t_pend; tsim_pend])]);
grid on;
hold off;