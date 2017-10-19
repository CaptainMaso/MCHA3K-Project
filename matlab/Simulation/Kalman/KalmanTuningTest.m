function cost = KalmanTuningTest(pw, pb, qw, qb, rt, rw, Theta, dTheta, ar, at, gy)

    %% Kalman Initialisation
    global T;

    Theta_k = zeros(size(Theta, 1), 1);
    dTheta_k = zeros(size(dTheta, 1), 1);
    Bias_k = zeros(size(dTheta, 1), 1);
    P_k = [T*pw, 0.5*T^2*pw, 0; 0.5*T^2*pw, T^3*pw/3, 0; 0, 0, T*pb];
    
    for i = 2:size(Theta, 1)
        yt = atan2(at(i-1), ar(i-1))/(1.065264436e-3);
        [Theta_k(i), dTheta_k(i), Bias_k(i), P_k] = IMUKalmanFilter(Theta_k(i-1), dTheta_k(i-1), Bias_k(i-1), P_k, gy(i - 1), yt, qw, qb, rw, rt);
    end
    
    cost = meansqr([Theta-Theta_k*(1.065264436e-3)]);
end