%% Load Data
global T;

LOWDATA = 4;
HIGHDATA = 12000;
N = 17;
T = 1/200;

[t, ENC, ax, az, gy] = ImportKalmanData('KalmanTuningData.csv', LOWDATA, HIGHDATA);

theta = ENC*2*pi/660;
dtheta = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(theta, 1));

gy = -gy/2^15*500;

[~,dtheta] = quadraticSavitzkyGolay(theta',T,N);
t = t(N+1:end-N);
theta = theta(N+1:end-N);
dtheta = dtheta(N+1:end-N)';

ax = ax(N+1:end-N);
az = az(N+1:end-N);
gy = gy(N+1:end-N);

%[res, cost] = fminunc(@(x) KalmanTuningTest(x(1), x(2), x(3), x(4), x(5), x(6), theta, dtheta, az, ax, gy), [100, 100, 100, 1, 100, 1]);

%pw = res(1)
% %pb = res(2)
% qw = res(3)
% qb = res(4)
% rt = res(5)
% rw = res(6)
% cost
% %%

pw = 1000;
pb = 1000;
qw = 1e6;
qb = 0.001;
rt = 1e6;
rw = 0.001;

Theta_k = zeros(size(theta, 1), 1);
dTheta_k = zeros(size(dtheta, 1), 1);
Bias_k = zeros(size(dtheta, 1), 1);
P_k = [T*pw, 0.5*T^2*pw, 0; 0.5*T^2*pw, T^3*pw/3, 0; 0, 0, T*pb];

for i = 2:size(theta, 1)
    [Theta_k(i), dTheta_k(i), Bias_k(i), P_k] = IMUKalmanFilter(Theta_k(i-1), dTheta_k(i-1), Bias_k(i-1), P_k, gy(i - 1), ax(i-1), az(i-1), qw, qb, rw, rt);
end

figure;
subplot(2,1,1);
plot(t, theta, 'b', t, Theta_k, 'r');
grid on;
subplot(2,1,2);
plot(t, dtheta, 'b', t, dTheta_k, 'r');
grid on;

figure;
plot(t, ax, t, az, t, gy)