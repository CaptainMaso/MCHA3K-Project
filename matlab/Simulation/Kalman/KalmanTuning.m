clc;
%% Load Data
global T;

LOWDATA = 4;
HIGHDATA = 12000;
N = 17;
T = 1/200;

[t, ENC, ax, az, gy] = ImportKalmanData('KalmanTuningData.csv', LOWDATA, HIGHDATA);

theta = ENC*pi/330;
dtheta = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(theta, 1));

% Implicit gain = 1.065264436e-3
%gy = gy/16.384*pi/180;

[~,dtheta] = quadraticSavitzkyGolay(theta',T,N);
t = t(N+1:end-N);
theta = theta(N+1:end-N);
dtheta = dtheta(N+1:end-N)';

ax = ax(N+1:end-N);
az = az(N+1:end-N);
gy = gy(N+1:end-N);
% 
% figure;
% plot(t, wrapToPi(atan2(az, ax))*180/pi, t, wrapToPi(theta)*180/pi);
% figure;
% plot(t, gy*1.065264436e-3, t, dtheta);

%%
Aeq = eye(6) *-1;

IC = [1000, 1000, 1000, 1, 1000, 1];
[res, cost] = fmincon(@(x) KalmanTuningTest(x(1), x(2), x(3), x(4), x(5), x(6), theta, dtheta, az, ax, gy), IC, Aeq, [0;0;0;0;0;0]);


pw = res(1)
pb = res(2)
qw = res(3)
qb = res(4)
rt = res(5)
rw = res(6)
cost

%%
pw_hat = pw;%*2^28
pb_hat = pb;%*2^28
qw_hat = qw;%*2^28
qb_hat = qb;%*2^28
rt_hat = rt;%*2^28
rw_hat = rw;%*2^28

global Tinv;
Tinv = 200;

Theta_k = zeros(size(theta, 1), 1);
dTheta_k = zeros(size(dtheta, 1), 1);
Bias_k = zeros(size(dtheta, 1), 1);
P_k = zeros(3, 3, size(dtheta, 1));
P_k(:,:,1) = [T*pw_hat, 0.5*T^2*pw_hat, 0; 0.5*T^2*pw_hat, T^3*pw_hat/3, 0; 0, 0, T*pb_hat];

for i = 2:size(theta, 1)
    yt = atan2(az(i-1), ax(i-1))/(1.065264436e-3);
    [Theta_k(i), dTheta_k(i), Bias_k(i), P_k(:,:,i)] = IMUKalmanFilter(Theta_k(i-1), dTheta_k(i-1), Bias_k(i-1), P_k(:,:,i-1), gy(i - 1), yt, qw_hat, qb_hat, rw_hat, rt_hat);
end

%%
figure;
subplot(3,1,1);
plot(t, theta*180/pi, 'b', t, Theta_k*1.065264436e-3*180/pi, 'r');
grid on;
subplot(3,1,2);
plot(t, dtheta*180/pi, 'b', t, dTheta_k*1.065264436e-3*180/pi, 'r');
grid on;
subplot(3,1,3);
plot(t, Bias_k*180/pi, 'b');
grid on;


max(max(max(P_k, [], 3), [], 2), [], 1)
