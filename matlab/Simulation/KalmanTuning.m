%% Load Data
[t, ENC, ax, az, gy] = ImportKalmanData('KalmanTuningData', 0, 12000);

theta = ENC*2*pi/CPR;
dtheta = zeros(HIGHDATA - LOWDATA - 2*N + 1, size(data,1));

[~,dtheta] = quadraticSavitzkyGolay(theta_tmp',T,N);
t(:) = t_tmp(N+1:end-N);
theta(:) = theta_tmp(N+1:end-N);
dtheta(:) = dtheta_tmp(N+1:end-N)';


%% Kalman Initialisation
Theta = 0;
dTheta = 0;
Bias = 0;
P = [T*pw, 0.5*T^2*pw, 0; 0.5*T^2*pw, T^3*pw/3, 0; 0, 0, T*pb];

%% 