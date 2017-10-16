function [Theta, dTheta, Bias, P] = IMUKalmanFilter(Theta, dTheta, Bias, P, Gyro, At, Ar, qw, qb, rw, rt)
    [Theta, dTheta, Bias, P] = TimeUpdate(Theta, dTheta, Bias, P, qw, qb);
    [Theta, dTheta, Bias, P] = GyroCorrection(Theta, dTheta, Bias, P, Gyro, rw);
    [Theta, dTheta, Bias, P] = AccCorrection(Theta, dTheta, Bias, P, At, Ar, rt);
end

function [Theta, dTheta, Bias, P] = TimeUpdate(Theta_p, dTheta_p, Bias_p, P_p, qw, qb)
    global T;
    
    % State Update
    dTheta = dTheta_p;
    Theta  = T*dTheta_p + Theta_p;
    Bias = Bias_p;
    
    % Covariance Update
    Pbb_p = P_p(3,3);
    Ptb_p = P_p(2,3);
    Ptt_p = P_p(2,2);
    Pwb_p = P_p(1,3);
    Pwt_p = P_p(1,2);
    Pww_p = P_p(1,1);
    
    Pbb = Pbb_p + T*qb;
    Ptb = T*Pwb_p+Ptb_p;
    Ptt = Ptt_p + T^2*Pww_p + 2*T*Pwt_p + T^3*qw/3;
    Pwb = Pwb_p;
    Pwt = T*Pww_p + Pwt_p + 1/2*T^2*qw;
    Pww = Pww_p + T*qw;
    
    P = [Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb];
end

function [Theta, dTheta, Bias, P] = GyroCorrection(Theta_p, dTheta_p, Bias_p, P_p, yw, rw)
    global T;
    Pbb_p = P_p(3,3);
    Ptb_p = P_p(2,3);
    Ptt_p = P_p(2,2);
    Pwb_p = P_p(1,3);
    Pwt_p = P_p(1,2);
    Pww_p = P_p(1,1);    

    %% Kalman Gain Update
    k = [Pww_p + Pwb_p; Pwt_p + Ptb_p; Pwb_p + Pbb_p]/(Pww_p + 2*Pwb_p + Pbb_p + rw/T);
    kw = k(1);
    kt = k(2);
    kb = k(3);
    
    %% State Update
    dTheta = dTheta_p + kw*(yw-dTheta_p-Bias_p);
    Theta =  Theta_p  + kt*(yw-dTheta_p-Bias_p);
    Bias =   Bias_p + kb*(yw-dTheta_p-Bias_p);
    
    %% Covariance Update
    Pww = (kw - 1)^2*Pww_p + (2*kw*(kw - 1))*Pwb_p + kw^2*Pbb_p + (kw^2*rw)/T;
    Pwt = (kt*(kw - 1))*Pww_p + (1 - kw)*Pwt_p + (kt*kw + kt*(kw - 1))*Pwb_p + (-kw)*Ptb_p + (kt*kw)*Pbb_p + (kt*kw*rw)/T;
    Pwb = (kb*(kw - 1))*Pww_p + (kb*kw + (kb - 1)*(kw - 1))*Pwb_p + (kw*(kb - 1))*Pbb_p + (kb*kw*rw)/T;
    Ptt = kt^2*Pww_p + (-2*kt)*Pwt_p + (2*kt^2)*Pwb_p + Ptt_p + (-2*kt)*Ptb_p + kt^2*Pbb_p + (kt^2*rw)/T;
    Ptb = (kb*kt)*Pww_p + (-kb)*Pwt_p + (kb*kt + kt*(kb - 1))*Pwb_p + (1 - kb)*Ptb_p + (kt*(kb - 1))*Pbb_p + (kb*kt*rw)/T;
    Pbb = kb^2*Pww_p + (2*kb*(kb - 1))*Pwb_p + (kb - 1)^2*Pbb_p + (kb^2*rw)/T;
    
    P = [Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb];
end

function [Theta, dTheta, Bias, P] = AccCorrection(Theta_p, dTheta_p, Bias_p, P_p, at, ar, rt)
    global T;
    Pbb_p = P_p(3,3);
    Ptb_p = P_p(2,3);
    Ptt_p = P_p(2,2);
    Pwb_p = P_p(1,3);
    Pwt_p = P_p(1,2);
    Pww_p = P_p(1,1);    

    %% Kalman Gain Update
    k = [Pwt_p; Ptt_p; Ptb_p]/(Ptt_p + rt/T);
    kw = k(1);
    kt = k(2);
    kb = k(3);
    
    %% State Update
    yt = atan2(at, -ar);
    dTheta = dTheta_p + kw*(yt-Theta_p);
    Theta =  Theta_p  + kt*(yt-Theta_p);
    Bias =   Bias_p   + kb*(yt-Theta_p);
    
    %% Covariance Update
    Pww = Pww_p + (-2*kw)*Pwt_p + kw^2*Ptt_p + (kw^2*rt)/T;
    Pwt = (1 - kt)*Pwt_p + (kw*(kt - 1))*Ptt_p + (kt*kw*rt)/T;
    Pwb = (-kb)*Pww_p + (kb*kw)*Pwt_p + (1 - kb)*Pwb_p + (kw*(kb - 1))*Ptb_p + (kb*kw*rt)/T;
    Ptt = (kt - 1)^2*Ptt_p + (kt^2*rt)/T;
    Ptb = (kb*(kt - 1))*Pwt_p + ((kb - 1)*(kt - 1))*Ptb_p + (kb*kt*rt)/T;
    Pbb = kb^2*Pww_p + (2*kb*(kb - 1))*Pwb_p + (kb - 1)^2*Pbb_p + (kb^2*rt)/T;
    
    P = [Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb];
end