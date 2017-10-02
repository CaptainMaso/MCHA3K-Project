clear;
%% Import Cart Data
M = 0.2;
g = 9.81;
[t_pend,theta_pend] = ImportPendData('pend_experiment.csv',2, 1002);

%% Estimating x.. and x. for cart
T = 0.01;
N = 12; % 2*N+1 points are used as local neighbourhood for quad fit
[theta_pend,dtheta_pend,ddtheta_pend] = quadraticSavitzkyGolay(theta_pend',T,N);

%% Estimating parameters
Phi = [dtheta_pend((N+1:end-N))', sin(theta_pend(N+1:end-N)')];
Y = ddtheta_pend((N+1:end-N))';

Theta = (Phi'*Phi) \ (Phi'*Y);
l = -g/Theta(2)
c = -M*l^2 * Theta(1)

%% Sim

sim('Q2_2sim')
%% Plot Results
figure;
hold on;
subplot(2,1,1);
plot(t_pend, theta_pend, 'b-', tsim_pend, thetasim_pend(:, 1), 'r--');
legend('Data', 'Sim');
grid on;
subplot(2,1,2);
plot(t_pend, dtheta_pend, 'b-', tsim_pend, dthetasim_pend(:, 1), 'r--');
legend('Data', 'Sim');
grid on;
hold off;