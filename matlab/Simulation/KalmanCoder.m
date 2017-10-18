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

Theta_k = zeros(size(theta, 1), 1);
dTheta_k = zeros(size(dtheta, 1), 1);
Bias_k = zeros(size(dtheta, 1), 1);
P_k = zeros(3, 3, size(dtheta, 1));

pw = 747.0042;


pb = 50;


qw = 26.9927;


qb = 1.0418e-07;


rt = 3.2401e+03;


rw = 5.3907e-05;

P_k(:,:,1) = [T*pw, 0.5*T^2*pw, 0; 0.5*T^2*pw, T^3*pw/3, 0; 0, 0, T*pb];

for i = 2:size(theta, 1)
    yt = atan2(az(i-1), ax(i-1))/(1.065264436e-3);
    [Theta_k(i), dTheta_k(i), Bias_k(i), P_k(:,:,i)] = IMUKalmanFilter(Theta_k(i-1), dTheta_k(i-1), Bias_k(i-1), P_k(:,:,i-1), gy(i - 1), yt, qw, qb, rw, rt);
end

%%
figure;
subplot(3,1,1);
plot(t, theta*180/pi, 'b', t, Theta_k*180/pi*1.065264436e-3, 'r');
grid on;
subplot(3,1,2);
plot(t, dtheta*180/pi, 'b', t, dTheta_k*180/pi*1.065264436e-3, 'r');
grid on;
subplot(3,1,3);
plot(t, Bias_k*180/pi, 'b');
grid on;