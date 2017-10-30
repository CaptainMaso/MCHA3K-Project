function [Theta, dTheta, Bias, P] = IMUKalmanFilter(Theta, dTheta, Bias, P, Gyro, AccTheta)
    [Theta, dTheta, Bias, P] = TimeUpdate(Theta, dTheta, Bias, P, qw, qb);
    [Theta, dTheta, Bias, P] = GyroCorrection(Theta, dTheta, Bias, P, Gyro, rw);
    [Theta, dTheta, Bias, P] = AccCorrection(Theta, dTheta, Bias, P, AccTheta, rt);
end

function [Theta, dTheta, Bias, P] = TimeUpdate(Theta_p, dTheta_p, Bias_p, P_p, qw, qb)
    global T qw qb;
    
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

function [Theta, dTheta, Bias, P] = GyroCorrection(Theta_p, dTheta_p, Bias_p, P_p, yw)
    global T rw;
    Pbb_p = P_p(3,3);
    Ptb_p = P_p(2,3);
    Ptt_p = P_p(2,2);
    Pwb_p = P_p(1,3);
    Pwt_p = P_p(1,2);
    Pww_p = P_p(1,1);    

    %% Kalman Gain Update
    kw = (Pww_p + Pwb_p)/(Pww_p + 2*Pwb_p + Pbb_p + rw/T);
    kt = (Pwt_p + Ptb_p)/(Pww_p + 2*Pwb_p + Pbb_p + rw/T);
    kb = (Pwb_p + Pbb_p)/(Pww_p + 2*Pwb_p + Pbb_p + rw/T);
    
    %% State Update
    dTheta = dTheta_p + kw*(yw-dTheta_p-Bias_p);
    Theta =  Theta_p  + kt*(yw-dTheta_p-Bias_p);
    Bias =   Bias_p + kb*(yw-dTheta_p-Bias_p);
    
    %% Covariance Update
    Pww = - Pwb_p*kw - Pww_p*(kw - 1);
    Pwt = - Ptb_p*kw - Pwt_p*(kw - 1);
    Pwb = - Pbb_p*kw - Pwb_p*(kw - 1);
    Ptt =   Ptt_p    - Ptb_p*kt - Pwt_p*kt;
    Ptb =   Ptb_p    - Pbb_p*kt - Pwb*kt;
    Pbb = - Pwb_p*kb - Pbb_p*(kb - 1);
    
    P = [Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb];
end

function [Theta, dTheta, Bias, P] = AccCorrection(Theta_p, dTheta_p, Bias_p, P_p, yt)
    global T rt;
    Pbb_p = P_p(3,3);
    Ptb_p = P_p(2,3);
    Ptt_p = P_p(2,2);
    Pwb_p = P_p(1,3);
    Pwt_p = P_p(1,2);
    Pww_p = P_p(1,1);    

    %% Kalman Gain Update
    kw = Pwt_p/(Ptt_p + rt/T);
    kt = Ptt_p/(Ptt_p + rt/T);
    kb = Ptb_p/(Ptt_p + rt/T);
    
    %% State Update
    dTheta = dTheta_p + kw*(yt-Theta_p);
    Theta =  Theta_p  + kt*(yt-Theta_p);
    Bias =   Bias_p   + kb*(yt-Theta_p);
    
    %% Covariance Update
    Pww = Pww_p - Pwt_p*kw;
    Pwt = Pwt_p - Ptt_p*kw;
    Pwb = Pwb_p - Ptb_p*kw;
    Ptt = -Ptt_p*(kt - 1);
    Ptb = -Ptb_p*(kt - 1);
    Pbb = Pbb_p - Ptb_p*kb;
    
    P = [Pww, Pwt, Pwb; Pwt, Ptt, Ptb; Pwb, Ptb, Pbb];
end